/*
 * This is a source file of GotWay M0 decoder and
 * controller.
 */

#include "EUCSerialInterface.h" // Include main header file


GotwayM0::GotwayM0(Stream &ReceiverSerial, Stream &TransmitterSerial):
ReceiverSerial(ReceiverSerial), TransmitterSerial(TransmitterSerial) {
}

void GotwayM0::setCallback(void (*eucLoopCallback)(float,float,float,float,float,float,bool)) {
  eucLoop = eucLoopCallback;
}

void GotwayM0::tick()
{
    GotwayM0::RawData rawData = this->receiveRawData();
    GotwayM0::UsableData data = this->makeRawDataUsable(rawData);
    this->eucLoop(
        data.voltage,
        data.speed,
        data.tempMileage,
        data.current,
        data.temperature,
        data.mileage,
        data.dataIsNew);
}

GotwayM0::UsableData GotwayM0::makeRawDataUsable(GotwayM0::RawData eucRawData)
{
    GotwayM0::UsableData eucUsableData;

    eucUsableData.voltage = (float)((eucRawData.voltage[0] << 8) | eucRawData.voltage[1]) / 100;                                                                                                                           // volts
    eucUsableData.speed = (float)((eucRawData.speed[0] << 8) | eucRawData.speed[1]) / 100 * 3.6;                                                                                                                           // kilometers per hour
    eucUsableData.tempMileage = (float)((uint32_t)eucRawData.tempMileage[0] << 24 | (uint32_t)eucRawData.tempMileage[1] << 16 | (uint32_t)eucRawData.tempMileage[2] << 8 | (uint32_t)eucRawData.tempMileage[3]) / 1000.00; // kilometers
    eucUsableData.current = (float)((eucRawData.current[0] << 8) | eucRawData.current[1]) / 100;                                                                                                                           // amps
    eucUsableData.temperature = (float)((eucRawData.temperature[0] << 8) | eucRawData.temperature[1]) / 340 + 36.53;                                                                                                       // Degrees Celsius
    eucUsableData.mileage = (float)((uint32_t)eucRawData.mileage[0] << 24 | (uint32_t)eucRawData.mileage[1] << 16 | (uint32_t)eucRawData.mileage[2] << 8 | (uint32_t)eucRawData.mileage[3]) / 1000.00;                     // kilometers
    eucUsableData.dataIsNew = eucRawData.dataIsNew;

    return eucUsableData;
}

GotwayM0::RawData GotwayM0::receiveRawData()
{
    static unsigned int curPos = 0; // stores the current position the packet parser
    static GotwayM0::RawData eucRawDataReceived;
    static GotwayM0::RawData eucRawDataReceivedValid;
    // TODO: there has to be a better way than this
    static unsigned int hppEnd = sizeof GotwayM0::RawData().headerPrimaryPacket;
    static unsigned int hppSize = sizeof GotwayM0::RawData().headerPrimaryPacket;
    static unsigned int voltageEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage;
    static unsigned int voltageSize = sizeof GotwayM0::RawData().voltage;
    static unsigned int speedEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed;
    static unsigned int speedSize = sizeof GotwayM0::RawData().speed;
    static unsigned int tempMileageEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage;
    static unsigned int tempMileageSize = sizeof GotwayM0::RawData().tempMileage;
    static unsigned int currentEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current;
    static unsigned int currentSize = sizeof GotwayM0::RawData().current;
    static unsigned int temperatureEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current + sizeof GotwayM0::RawData().temperature;
    static unsigned int temperatureSize = sizeof GotwayM0::RawData().temperature;
    static unsigned int unknownDataEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current + sizeof GotwayM0::RawData().temperature + sizeof GotwayM0::RawData().unknownData;
    static unsigned int unknownDataSize = sizeof GotwayM0::RawData().unknownData;
    static unsigned int hspEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current + sizeof GotwayM0::RawData().temperature + sizeof GotwayM0::RawData().unknownData + sizeof GotwayM0::RawData().headerSecondaryPacket;
    static unsigned int hspSize = sizeof GotwayM0::RawData().headerSecondaryPacket;
    static unsigned int mileageEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current + sizeof GotwayM0::RawData().temperature + sizeof GotwayM0::RawData().unknownData + sizeof GotwayM0::RawData().headerSecondaryPacket + sizeof GotwayM0::RawData().mileage;
    static unsigned int mileageSize = sizeof GotwayM0::RawData().mileage;
    static unsigned int endEnd = sizeof GotwayM0::RawData().headerPrimaryPacket + sizeof GotwayM0::RawData().voltage + sizeof GotwayM0::RawData().speed + sizeof GotwayM0::RawData().tempMileage + sizeof GotwayM0::RawData().current + sizeof GotwayM0::RawData().temperature + sizeof GotwayM0::RawData().unknownData + sizeof GotwayM0::RawData().headerSecondaryPacket + sizeof GotwayM0::RawData().mileage + sizeof GotwayM0::RawData().end - 1;
    static unsigned int endSize = sizeof GotwayM0::RawData().end;

    eucRawDataReceivedValid.dataIsNew = false;
    if (ReceiverSerial.available() > 0)
    {                                                      // if a new byte has been received
        unsigned char currentByte = ReceiverSerial.read(); // read the incoming byte

        if (curPos < hppEnd)
        { // expected byte of the primary header was received
            if (currentByte == eucRawDataReceived.headerPrimaryPacket[curPos])
            {
                curPos++;
            }
            else
            {
                curPos = 0;
            }
            if (curPos < voltageEnd)
            {
                eucRawDataReceived.voltage[curPos - (voltageEnd - voltageSize)] = currentByte;
                curPos++;
            }
            else if (curPos < speedEnd)
            {
                eucRawDataReceived.speed[curPos - (speedEnd - speedSize)] = currentByte;
                curPos++;
            }
            else if (curPos < tempMileageEnd)
            {
                eucRawDataReceived.tempMileage[curPos - (tempMileageEnd - tempMileageSize)] = currentByte;
                curPos++;
            }
            else if (curPos < currentEnd)
            {
                eucRawDataReceived.current[curPos - (currentEnd - currentSize)] = currentByte;
                curPos++;
            }
            else if (curPos < temperatureEnd)
            {
                eucRawDataReceived.temperature[curPos - (temperatureEnd - temperatureSize)] = currentByte;
                curPos++;
            }
            else if (curPos < unknownDataEnd)
            {
                eucRawDataReceived.unknownData[curPos - (unknownDataEnd - unknownDataSize)] = currentByte;
                curPos++;
            }
            else if (curPos < hspEnd)
            { // expected byte of the secondary header was received
                if (currentByte == eucRawDataReceived.headerSecondaryPacket[curPos - (hspEnd - hspSize)])
                {
                    curPos++;
                }
                else
                {
                    curPos = 0;
                }
                if (curPos < mileageEnd)
                {
                    eucRawDataReceived.mileage[curPos - (mileageEnd - mileageSize)] = currentByte;
                    curPos++;
                }
                else if (curPos < endEnd)
                { // expected byte of the end was received
                    if (currentByte == eucRawDataReceived.end[curPos - (endEnd - endSize)])
                    {
                        curPos++;
                    }
                    else
                    {
                        curPos = 0;
                    };
                }
                else if (curPos == endEnd)
                { // all data received
                    eucRawDataReceivedValid = eucRawDataReceived;
                    eucRawDataReceivedValid.dataIsNew = true;
                    curPos = 0;
                }
            }
            return eucRawDataReceivedValid;
        }
    }
}

void GotwayM0::beep()
{
    this->TransmitterSerial.println("b");
}

void GotwayM0::setRideRigidity(int mode)
{
    switch (mode)
    {
    case 1:
        this->TransmitterSerial.println("s");
        break;
    case 2:
        this->TransmitterSerial.println("f");
        break;
    case 3:
        this->TransmitterSerial.println("h");
    default:
        break;
    }
}

void GotwayM0::calibrateAlignment()
{
    // TODO: check if delays are necessary
    this->TransmitterSerial.println(",");
    this->TransmitterSerial.println("c");
    this->TransmitterSerial.println("y");
    this->TransmitterSerial.println("c");
    this->TransmitterSerial.println("y");
    this->TransmitterSerial.println("cy");
    this->TransmitterSerial.println("y");
}

void GotwayM0::setAlarms(int level, bool state)
{
    switch (level)
    {
    case 1:
        if (state == false)
        {
            this->TransmitterSerial.println("u"); // Disable level 1 alarm
        }
        else if (state == true)
        {
            this->TransmitterSerial.println("o"); // Enable all alarms
            this->TransmitterSerial.println("i"); // Disable level 2 alarms
        }
        break;
    case 2:
        if (state == false)
        {
            this->TransmitterSerial.println("i"); // Disable level 2 alarm
        }
        else if (state == true)
        {
            this->TransmitterSerial.println("o"); // Enable all alarms
            this->TransmitterSerial.println("u"); // Disable level 1 alarm
        }
        break;
    case 0:
        this->TransmitterSerial.println("o");

    default:
        break;
    }
}

void GotwayM0::set6kmhTiltback(bool state)
{
    if (state == true)
    {
        this->TransmitterSerial.println("O");
    }
    else
    {
        this->TransmitterSerial.println("I");
    }
}