#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ArduinoIoTCloudTCP.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "time.h"
#include <UnixTime.h>
const char DEVICE_LOGIN_NAME[]  = "a3c51e4e-9074-4499-918b-926751d622f2";
const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]         = SECRET_DEVICE_KEY;    // Secret device password

char SetSecond[5];
char SetMinute[5];
char SetHour[5];
char SetDate[5];
char SetMonth[5];
char SetYear[5];

char GetSecond[5];
char GetMinute[5];
char GetHour[5];
char GetDate[5];
char GetMonth[5];
char GetYear[5];

void onGetTsChange();
void onSetTsChange();
void onLedChange();
void onRelayChange();
void onTimestampChange();

CloudSchedule get_ts;
CloudSchedule set_ts;
bool led;
bool relay;
CloudTime timestamp;
String message;

void initProperties() {

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(get_ts, READWRITE, ON_CHANGE, onGetTsChange);
  ArduinoCloud.addProperty(set_ts, READWRITE, ON_CHANGE, onSetTsChange, 1);
  ArduinoCloud.addProperty(led, READWRITE, ON_CHANGE, onLedChange);
  ArduinoCloud.addProperty(relay, READWRITE, ON_CHANGE, onRelayChange);
  ArduinoCloud.addProperty(message, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(timestamp, READWRITE, ON_CHANGE, onTimestampChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
