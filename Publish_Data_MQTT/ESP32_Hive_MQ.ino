#include <WiFi.h>
#include <PubSubClient.h>
#include "time.h"

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

const char *SSID = "HOANG TRUNG";
const char *PWD = "12345678";

// Hive MQ
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

//Variables
float trueValue = 5;
float temp;
String result;
String t;

//connectToWiFi()===============================
void connectToWiFi() {
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

//callback()====================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

//setupMQTT()===================================
void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

double randomDouble(double minf, double maxf)
{
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);  // use 1ULL<<63 for max double values)
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupMQTT();

  // Init and get the time
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 printLocalTime();
}

//reconnect()==============================
void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("/swa/commands");
      }
      
  }
}

void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();


  temp = trueValue + randomDouble(1.00, 3.00);
  printLocalTime();
  result = String( t + String(temp) + " °C");
  Serial.println(result);

  int str_len = result.length() + 1;
  char char_array[str_len];
  result.toCharArray(char_array, str_len);

    mqttClient.publish("/swa/temperature", char_array);
  delay(5000);
  }


//printLocalTime()==============================
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
 char time_[20];
 strftime(time_,20, "%H:%M:%S - ", &timeinfo);
 t = String(time_);
}
  // Hive MQ
  //Step 1: Run code
  //Step 2: Go to this link and click on connect button
  // Add the subscription topics one for each topic the ESP32 uses
  //http://www.hivemq.com/demos/websocket-client/
