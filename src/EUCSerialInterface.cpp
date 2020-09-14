#include "EUCSerialInterface.h"

Euc::Euc(Stream &ReceiverSerial, Stream &TransmitterSerial):
ReceiverSerial(ReceiverSerial), TransmitterSerial(TransmitterSerial) {

}

void Euc::setCallback(void (*eucLoopCallback)(float,float,float,float,float,float,bool)) {
  eucLoop = eucLoopCallback;
}

void Euc::tick() {
  Euc::RawData rawData = this->receiveRawData();
  Euc::UsableData data = this->makeRawDataUsable(rawData);
  this->eucLoop(data.voltage,data.speed,data.tempMileage,data.current,data.temperature,data.mileage,data.dataIsNew);
}

int Euc::getRideState() {
  return _rideState;
}
