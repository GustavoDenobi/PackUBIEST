void batteryLife() {
  bool charge;
  sprintf(buff,"Battery level = %d%", LBattery.level() );
  smsSender(phoneNumber, buff);
}

void batteryFeedback() {
//  Serial.println("Retrieving battery data:");
  batteryLife();
}

void batteryCheck() {
  if(batteryCheckMode) {
    int charge = LBattery.level();
    if (batteryCheckCount == 0) {
      if (charge == 0) {
        batteryCheckCount = batteryCheckCountRst;
        smsSender(phoneNumber, "Battery power low!");
      }
      else {
        batteryCheckCount = batteryCheckCount-1;
      }    
    }    
  }
}

