unsigned char getComma(unsigned char num, const char *str)
{
  unsigned char i, j = 0;
  int len = strlen(str);
  for (i = 0; i < len; i ++)
  {
    if (str[i] == ',')
      j++;
    if (j == num)
      return i + 1;
  }
  return 0;
}

float getFloatNumber(const char *s){
  char buf[10];
  unsigned char i;
  float rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atof(buf);
  return rev;
}

double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atof(buf);
  return rev;
}

double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;

  i = getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev = atoi(buf);
  return rev;
}


void parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
     Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
     Where:
      GGA          Global Positioning System Fix Data
      123519       Fix taken at 12:35:19 UTC
      4807.038,N   Latitude 48 deg 07.038' N
      01131.000,E  Longitude 11 deg 31.000' E
      1            Fix quality: 0 = invalid
                                1 = GPS fix (SPS)
                                2 = DGPS fix
                                3 = PPS fix
                                4 = Real Time Kinematic
                                5 = Float RTK
                                6 = estimated (dead reckoning) (2.3 feature)
                                7 = Manual input mode
                                8 = Simulation mode
      08           Number of satellites being tracked
      0.9          Horizontal dilution of position
      545.4,M      Altitude, Meters, above mean sea level
      46.9,M       Height of geoid (mean sea level) above WGS84
                       ellipsoid
      (empty field) time in seconds since last DGPS update
      (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
  */

  int tmp, hour, minute, second, num, lat, longit ;
  if (GPGGAstr[0] == '$')
  {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');


    sprintf(buff, "UTC timer %2d-%2d-%2d", hour, minute, second);
    //    Serial.println(GPGGAstr);

    tmp = getComma(2, GPGGAstr);
    //Serial.println(getDoubleNumber(&GPGGAstr[tmp]));
    latitude = getDoubleNumber(&GPGGAstr[tmp]) / 100.0;
    int latitude_int = floor(latitude);
    double latitude_decimal = (latitude - latitude_int) * 100.0 / 60.0;
    latitude = (latitude_int + latitude_decimal);
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp]) / 100.0;
    int longitude_int = floor(longitude);
    double longitude_decimal = (longitude - longitude_int) * 100.0 / 60.0;
    longitude = (longitude_int + longitude_decimal);


    sprintf(buff, "latitude = %10.6f, longitude = %10.6f", latitude, longitude);
    //Serial.println(buff);

    if (GPGGAstr[28] == 'S')
    {
      latitude = -latitude;
    } else {
    }

    if (GPGGAstr[41] == 'W')
    {
      longitude = -longitude;
    } else {
    }

    //    Serial.println(latitude);
    //    Serial.println(longitude);

    tmp = getComma(7, GPGGAstr);
    num = getIntNumber(&GPGGAstr[tmp]); // number of satellites connected
    sprintf(buff, "%d", num);
    //    Serial.println(buff);
    //    message = buff;
  }
  else
  {
    //    Serial.println("Not get data");
  }
}

float readSpeed(const char* GPVTGstr)
{
  //GPVTG
  /*
    example string. $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K

    054.7,T      True track made good
    034.4,M      Magnetic track made good
    005.5,N      Ground speed, knots
    010.2,K      Ground speed, Kilometers per hour

    eg3. $GPVTG,t,T,,,s.ss,N,s.ss,K*hh
    1    = Track made good
    2    = Fixed text 'T' indicates that track made good is relative to true north
    3    = not used
    4    = not used
    5    = Speed over ground in knots
    6    = Fixed text 'N' indicates that speed over ground in in knots
    7    = Speed over ground in kilometers/hour
    8    = Fixed text 'K' indicates that speed over ground is in kilometers/hour
    9    = Checksum
  */

  float speed_temp;
  int tmp = getComma(7, GPVTGstr);
  speed_temp = getFloatNumber(&GPVTGstr[tmp]);
//  sprintf(buffer_speed, speed_temp);
  return speed_temp;
}

void GPS_receive() {
  LGPS.getData(&info);
  parseGPGGA((const char*)info.GPGGA);
}

void writeGPS() {
  float latitude_post = latitude;
  float longitude_post = longitude;
  if (latitude > -90.0000 && latitude <= 90.0000 && longitude > -180.0000 && longitude <= 180.0000) {
    sprintf(buffer_latitude, "%.6f", latitude);
    sprintf(buffer_longitude, "%.6f", longitude);
  }
  String gpsData = "Location:\n\nhttps://www.google.com.br/maps/place/" + String(buffer_latitude) + "," + String(buffer_longitude) + "\n\nNumber of satellites: " + String(buff); //null altitude
  smsSender(phoneNumber, gpsData);
}

void awareGPS() {
  float latitude_post = latitude;
  float longitude_post = longitude;
  if (latitude > -90.0000 && latitude <= 90.0000 && longitude > -180.0000 && longitude <= 180.0000) {
    sprintf(buffer_latitude, "%.6f", latitude);
    sprintf(buffer_longitude, "%.6f", longitude);
  }
  String upload_GPS = "[{\"latlng\":[" + String(buffer_latitude) + "," + String(buffer_longitude) + "], \"data\":\"" + String(readSpeed((const char*)info.GPVTG)) + "\"}]";
  const char* upload_char = upload_GPS.c_str();
  LGPRSClient *client;
  client = PubNub.publish(channel, upload_char, 80);
  if (!client) {
    awareStatus = 3;
    delay(100);
    return;
  }
  if (client->connected()) { //shouldnt it be a while?
    char c = client->read();
  }
  else {
    client->stop();
    awareStatus = 2;    
  }
  delay(100);
}

// Writes the location to the message.
void gpsFeedback() {
  GPS_receive();
  writeGPS();    // Writes the 'message' string with GPS info
}

