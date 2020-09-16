/*
 * This is a source file of Veteran Sherman decoder and
 * controller.
 *

#include "EUCSerialInterface.h" // Include main header file

void VeteranSherman::tick()
{
	VeteranSherman::RawData rawData = this->receiveRawData();
	VeteranSherman::UsableData data = this->makeRawDataUsable();
	this->eucLoop(
		data.voltage,
		data.speed,
		data.tempMileage,
		data.current,
		data.temperature,
		data.mileage,
		data.dataIsNew);
}

VeteranSherman::UsableData VeteranSherman::makeRawDataUsable(VeteranSherman::RawData(eucRawData) {
	VeteranSherman::UsableData eucUsableData;

	// volts
	eucUsableData.voltage = (float)((eucRawData.voltage[0] << 8) | eucRawData.voltage[1]) / 100;
	// kilometers per hour
    eucUsableData.speed = (float)((eucRawData.speed[0] << 8) | eucRawData.speed[1]) / 100 * 3.6;
	// kilometers
    eucUsableData.tempMileage = (float)((uint32_t)eucRawData.tempMileage[0] << 24 | (uint32_t)eucRawData.tempMileage[1] << 16 | (uint32_t)eucRawData.tempMileage[2] << 8 | (uint32_t)eucRawData.tempMileage[3]) / 1000.00;
	// amps
    eucUsableData.current = (float)((eucRawData.current[0] << 8) | eucRawData.current[1]) / 100;
	// Degrees Celsius
    eucUsableData.temperature = (float)((eucRawData.temperature[0] << 8) | eucRawData.temperature[1]) / 340 + 36.53;
	// kilometers
    eucUsableData.mileage = (float)((uint32_t)eucRawData.mileage[0] << 24 | (uint32_t)eucRawData.mileage[1] << 16 | (uint32_t)eucRawData.mileage[2] << 8 | (uint32_t)eucRawData.mileage[3]) / 1000.00;
    eucUsableData.dataIsNew = eucRawData.dataIsNew;

	return eucUsableData;
}
*/