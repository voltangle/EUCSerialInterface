#include "EUCSerialInterface.h"

EUC::EUC(Stream &ReceiverSerial, Stream &TransmitterSerial):
ReceiverSerial(ReceiverSerial), TransmitterSerial(TransmitterSerial) {

}

void EUC::setCallback(void (*eucLoopCallback)(float,float,float,float,float,float,bool)) {
  eucLoop = eucLoopCallback;
}

void EUC::tick() {
  EUC::RawData rawData = this->receiveRawData();
  EUC::UsableData data = this->makeRawDataUsable(rawData);
  this->eucLoop(data.voltage,data.speed,data.tempMileage,data.current,data.temperature,data.mileage,data.dataIsNew);
}

