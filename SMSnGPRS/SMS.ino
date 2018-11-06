void smsReceiver() {
  LSMS.remoteNumber(phoneNumber, 20); // display Number part
  incomingSMS = smsReader(); // reads the incoming message
  LSMS.flush(); // delete message
}

String smsReader() {
  String readSMS;
  int len = 0;
  while (true) {
    int letter = LSMS.read();
    if (letter < 0) {
      break;
    }
    readSMS += (char)letter;
  }
  return readSMS;
}

void smsSender(char recipient[20], String text) {
  LSMS.beginSMS(recipient);    // number that receives the feedback
  LSMS.print(text); // Prepares the message to be sent.
  LSMS.endSMS(); // Sends the SMS.
}

////////////////////////////////////////////////////////////
// COMMANDS AVAILABLE:
String gpsKey = "Where?";
String batteryKey = "Juice?";
String helpKey = "Help";
String awareKey = "Aware";
//Auth key defined in main page
////////////////////////////////////////////////////////////

void smsAnswer(String x) {
  if(strcmp(phoneNumber, authPhoneNumber) + strcmp(phoneNumber, adminPhoneNumber) < 2) {
    if (x == gpsKey) {
      gpsFeedback();
    }
    else if (x == batteryKey) {
      batteryFeedback();
    }
    else if (x == helpKey) {
      helpFeedback();
    }
    else if (x == awareKey) {
      awareMode();
    }
    else {
      message = incomingSMS + ": command unknown.";
    }       
  }
  else if (x == authPassword) {
    authSetup();
  }
  else {
    message = "";
    memcpy(phoneNumber, adminPhoneNumber, strlen(phoneNumber)+1);
  }
}

