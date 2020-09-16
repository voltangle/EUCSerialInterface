#ifndef EUC_SERIAL_INTERFACE_H
#define EUC_SERIAL_INTERFACE_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

class GotwayMcm2 {
    public:
        struct RawData {
            unsigned char headerPrimaryPacket[8] = {0x04, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char voltage[2] = {0x00, 0x00};
            unsigned char speed[2] = {0x00, 0x00};
            unsigned char tempMileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char current[2] = {0x00, 0x00};
            unsigned char temperature[2] = {0x00, 0x00};
            unsigned char unknownData[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: figure out what that is //NOTE: only first byte seems to change
            unsigned char headerSecondaryPacket[8] = {0x00, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char mileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char end[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            
            bool dataIsNew = false;
        };
        
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float current;
            float temperature;
            //unsigned char unknownData[4];
            float mileage;
            bool dataIsNew = false;
        };
        
        Stream &ReceiverSerial;
        Stream &TransmitterSerial;
        void (*eucLoop)(float,float,float,float,float,float,bool);
        
        GotwayMcm2::RawData receiveRawData();
        GotwayMcm2::UsableData makeRawDataUsable(GotwayMcm2::RawData eucRawData);
        
        GotwayMcm2(Stream &ReceiverSerial, Stream &TransmitterSerial);
        
        void tick();
        void setCallback(void (*eucLoopCallback)(float, float, float, float, float, float, bool));

        void beep();
        void setRideRigidity(int mode);
        void calibrateAlignment();
        void setAlarms(int level, bool state);
        void set6kmhTiltback(bool state);
};

class GotwayMcm4 {
    public:
        struct RawData {

        };
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float temperature;
            float mileage;
            
            float calculateAcceleration();
            float calculatePower();
            bool dataIsNew();
        };
        float calculateAcceleration();
        float calculatePower();
};

class VeteranSherman {
    public:
        struct RawData {
            unsigned char headerPrimaryPacket[4] = {0xDC, 0x5A, 0x5C, 0x20};
            unsigned char voltage[2] = {0x00, 0x00};
            unsigned char speed[2] = {0x00, 0x00};
            unsigned char tempMileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char mileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char current[2] = {0x00, 0x00};
            unsigned char temperature[2] = {0x00, 0x00};
        };
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float current;
            float temperature;
            float mileage;
            bool dataIsNew = false;
        };
        
        Stream &ReceiverSerial;
        Stream &TransmitterSerial;
        void (*eucLoop)(float,float,float,float,float,float,bool);
        
        VeteranSherman::RawData receiveRawData();
        VeteranSherman::UsableData makeRawDataUsable(VeteranSherman::RawData eucRawData);
        
        VeteranSherman(Stream &ReceiverSerial, Stream &TransmitterSerial);
        
        void tick();
        void setCallback(void (*eucLoopCallback)(float, float, float, float, float, float, bool));

        void beep();
        void setRideRigidity(int mode);
        void calibrateAlignment();
};

class GotwayM0 { // GotWay M0 class
    public:
        struct RawData {
            unsigned char headerPrimaryPacket[8] = {0x04, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char voltage[2] = {0x00, 0x00};
            unsigned char speed[2] = {0x00, 0x00};
            unsigned char tempMileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char current[2] = {0x00, 0x00};
            unsigned char temperature[2] = {0x00, 0x00};
            unsigned char unknownData[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: figure out what that is //NOTE: only first byte seems to change
            unsigned char headerSecondaryPacket[8] = {0x00, 0x18, 0x5A, 0x5A, 0x5A, 0x5A, 0x55, 0xAA};
            unsigned char mileage[4] = {0x00, 0x00, 0x00, 0x00};
            unsigned char end[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            
            bool dataIsNew = false;
        };
        
        struct UsableData {
            float voltage;
            float speed;
            float tempMileage;
            float current;
            float temperature;
            //unsigned char unknownData[4];
            float mileage;
            bool dataIsNew = false;
        };
        
        Stream &ReceiverSerial;
        Stream &TransmitterSerial;
        void (*eucLoop)(float,float,float,float,float,float,bool);
        
        GotwayM0::RawData receiveRawData();
        GotwayM0::UsableData makeRawDataUsable(GotwayM0::RawData eucRawData);
        
        GotwayM0(Stream &ReceiverSerial, Stream &TransmitterSerial);
        
        void tick();
        void setCallback(void (*eucLoopCallback)(float, float, float, float, float, float, bool));

        void beep();
        void setRideRigidity(int mode);
        void calibrateAlignment();
        void setAlarms(int level, bool state);
        void set6kmhTiltback(bool state);
};

#endif // EUC_SERIAL_INTERFACE_H