//!LGPRS.attachGPRS("timbrasil.br", "tim", "tim")

void awareMode() {
  awareStatus = 1;
  int attempts = awareConnectAttempts;
  if(attempts > 0) {
    while (!LGPRS.attachGPRS("timbrasil.br", "tim", "tim")) 
    {
      delay(awareConnectPause);
      attempts -= 1;
    }
  }
  else {
    awareStatus == 3;
  }
  PubNub.begin(pubkey, subkey);
  
  smsSender(phoneNumber, "Aware mode activated.");
  while (awareStatus == 1) {
    GPS_receive();
    awareGPS();
    if (LSMS.available()) {
      smsReceiver();
      if(strcmp(phoneNumber, authPhoneNumber) + strcmp(phoneNumber, adminPhoneNumber) < 2) {
          awareStatus = 4;
          LGPRSClient stop();
      }
    }
    delay(100);
  }
  if(awareStatus == 2) {
    smsSender(phoneNumber, "Lost connection. Aware mode deactivated.");
  }
  else if(awareStatus == 3) {
    smsSender(phoneNumber, "Cound not stablish connection. Aware mode deactivated.");
  }
  else if(awareStatus == 4) {
    smsSender(phoneNumber, "Aware mode deactivated");
  }
}
