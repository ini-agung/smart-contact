#include "arduino_secrets.h"
#include "thingProperties.h"
TaskHandle_t CheckTime;
UnixTime stamp(0);

void CheckTimeCode(void *parameter) {
  for (;;) {
    delay(100);
    while (WiFi.status() != WL_CONNECTED) {}
    Serial.print("Timestamp : ");
    stamp.getDateTime(timestamp);
    int sYear = stamp.year;
    int sMonth = stamp.month;
    int sDay = stamp.day;
    int sHour = stamp.hour;
    int sMinute = stamp.minute;
    int sSecond = stamp.second;
    String sy, smn, sd, sh, sm, ss;
    sy = String(sYear);
    smn = String(sMonth);
    sd = String(sDay);
    sh = String(sHour);
    sm = String(sMinute);
    ss = String(sSecond);
    if(sYear < 2000){
      sy = "2000";
    }
    if(sMonth<10){
      smn = "0" + String(sMonth);
    }
    if(sDay < 10){
      sd = "0" + String(sDay);
    }
    if(sHour<10){
      sh = "0" + String(sHour);
    }
    if(sMinute < 10){
      sm = "0" + String(sMinute);
    }
    if(sSecond < 10){
      ss = "0" + String(sSecond);
    }

    strcpy(SetYear, sy.c_str());
    strcpy(SetMonth, smn.c_str());
    strcpy(SetDate, sd.c_str());
    strcpy(SetHour, sh.c_str());
    strcpy(SetMinute, sm.c_str());
    strcpy(SetSecond, ss.c_str());
        
    static int ms = millis();
    if (millis() - ms > 100) {
      printLocalTime();
      String setTS = String(SetYear) + "-" + String(SetMonth) + "-" + String (SetDate) + " " + String(SetHour) + ":" + String(SetMinute) + ":" + String(SetSecond);
      String getTS = String(GetYear) + "-" + String(GetMonth) + "-" + String (GetDate) + " " + String(GetHour) + ":" + String(GetMinute) + ":" + String(GetSecond);
      message = getTS;
       if (setTS.equals(getTS)) {
        Serial.println("===SAMA===");
        relay = !relay;
        delay(1000);
      }

      Serial.print("Set TS : "); Serial.println(setTS);
      Serial.print("Get TS : "); Serial.println(getTS);
      ms = millis();
    }
  }
}


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  xTaskCreate(CheckTimeCode, "Check Time", 10000, NULL, 0, &CheckTime);
}

void loop() {
  ArduinoCloud.update();
}


String timestampToDate() {

  //  struct tm * timeinfo;
  //  char buffer[80];
  //  timeinfo = localtime(&timestamp);
  //  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  //  strftime(SetSecond, sizeof(SetSecond), "%S", timeinfo);
  //  strftime(SetMinute, sizeof(SetMinute), "%M", timeinfo);
  //  strftime(SetHour, sizeof(SetHour), "%H", timeinfo);
  //  strftime(SetDate, sizeof(SetDate), "%d", timeinfo);
  //  strftime(SetMonth, sizeof(SetMonth), "%m", timeinfo);
  //  strftime(SetYear, sizeof(SetYear), "%Y", timeinfo);
  //  Serial.println(buffer);


  //  return buffer;
}

void printLocalTime() {
  configTime(0, 25200, "pool.ntp.org");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(GetSecond, sizeof(GetSecond), "%S", &timeinfo);
  strftime(GetMinute, sizeof(GetMinute), "%M", &timeinfo);
  strftime(GetHour, sizeof(GetHour), "%H", &timeinfo);
  strftime(GetDate, sizeof(GetDate), "%d", &timeinfo);
  strftime(GetMonth, sizeof(GetMonth), "%m", &timeinfo);
  strftime(GetYear, sizeof(GetYear), "%Y", &timeinfo);
}

/*
  Since Led is READ_WRITE variable, onLedChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onLedChange()  {
  // Add your code here to act upon Led change
  Serial.print("LED : "); Serial.println(led);
}

/*
  Since Relay is READ_WRITE variable, onRelayChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRelayChange()  {
  // Add your code here to act upon Relay change
  Serial.print("Relay : "); Serial.println(relay);
  if (relay) {
    led = relay;
  } else {
    led = relay;
  }
}

/*
  Since SetTs is READ_WRITE variable, onSetTsChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSetTsChange()  {
  // Add your code here to act upon SetTs change
  //  Serial.print("Set TS : ");Serial.println(set_ts);
}

/*
  Since GetTs is READ_WRITE variable, onGetTsChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onGetTsChange()  {
  // Add your code here to act upon GetTs change
  //  Serial.print("Get TS : ");Serial.println(get_ts);
}

/*
  Since Timestamp is READ_WRITE variable, onTimestampChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTimestampChange()  {
  // Add your code here to act upon Timestamp change
  Serial.print("timestamp : "); Serial.print(timestamp);
}
