#include <LBattery.h>      // From Battery.
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGSM.h>          // From SMS.
#include <LGPS.h>
#include <LTask.h>
#include <SPI.h>
#include "PubNub.h"

////////////////////////////////////////////////////////
// SETTINGS:
int delayTime = 5; // In seconds, defines how long the code will wait to repeat the loop.
int gpsCheckCount = 20; // Defines how many times the gps will attempt to find GPS connection before giving up.
int gpsCheckDelay = 30; // In seconds, defines how long the code will wait to attempt to find GPS connection again.
int awareConnectAttempts = 60;
int awareConnectPause = 2000;
int authSetupDelay = 60; // In seconds, defines how long the authSetup function will wait for the authKey.
int batteryCheckCount = 30; // Number of loops it will wait until performing a battery level check
int batteryCheckCountRst = batteryCheckCount; // Number of loops it will wait until performing a battery level check
bool batteryCheckMode = true;
String authPassword = "Auth Arroz96"; // Password for granting access in authSetup.
char pubkey[] = "pub-c-ac4d7b48-054e-423f-9c20-bea57ea1aeb5";
char subkey[] = "sub-c-92365414-1269-11e8-b32f-5ea260837941";
char channel[] = "PackVRESTO";
////////////////////////////////////////////////////////

String message = "";
char phoneNumber[20];
char adminPhoneNumber[20] = "043999605857";
char authPhoneNumber[20] = "043999605857";
String incomingSMS;
int awareStatus; // 1- active    2 - lost connection    3 - unable to connect    4 - deactivated by user
char buff[256];

//GPS exclusive
gpsSentenceInfoStruct info;
double latitude;
double longitude;
char buffer_latitude[8];
char buffer_longitude[8];
//char buffer_speed[8];

//Battery exclusive
int chargeLevel;

void setup()  {
//  Serial.begin(9600);
  LGPS.setMode(GPS_HOT_START);
  LGPS.powerOn();    // Turns GPS ON
    while (!LSMS.ready()) // Waits for GSM to be available
    {
      delay(100);
    }
}


void loop()
{
  message = "";
  batteryCheck(); //Checks if battery level is above 0;

  if (LSMS.available()) // Check if there is new SMS
  {
    smsReceiver(); // receives the incoming message
    smsAnswer(incomingSMS); // answers the request contained in the message
  }    
  delay(delayTime*1000);
}
