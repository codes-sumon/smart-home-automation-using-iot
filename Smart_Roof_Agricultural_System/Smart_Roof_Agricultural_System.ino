#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"
// Set these to run example.
#define FIREBASE_HOST "nodemcuconnfirebasetest-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HHkCS1zgUIOAHEvKV2mqUHcLUupWvjfbs9IkseEK"
#define WIFI_SSID "IOT LAB"
#define WIFI_PASSWORD "iotlab@507"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);




// Relay State
bool toggle_Device_Status_1 = LOW;
bool toggle_Device_Status_2 = LOW;
bool toggle_Device_Status_3 = LOW;
bool toggle_Device_Status_4 = LOW;

int sensor_pin = A0; //for soil sensor

int output_value ;
int wifiFlag = 0;
float temperature1 = 0;
float humidity1   = 0;
float moisture1 = 0;



void setup() {


  // define the GPIO connected with Relays and switches
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6
#define wifiLed   15 //D8

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(wifiLed, OUTPUT);

  // digitalWrite(RelayPin1, !toggle_Device_Status_1);
  // digitalWrite(RelayPin2, !toggle_Device_Status_2);
  // digitalWrite(RelayPin3, !toggle_Device_Status_3);
  // digitalWrite(RelayPin4, !toggle_Device_Status_4);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.begin(9600);
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();
}


void chageDeviceStatus()
{
  String  d1 = Firebase.getString("device1");
  String  d2 = Firebase.getString("device2");
  String  d3 = Firebase.getString("device3");
  String  d4 = Firebase.getString("device4");

  if (d1 == "1")
  {
    toggle_Device_Status_1 = HIGH;
    digitalWrite(RelayPin1, !toggle_Device_Status_1);
    Serial.println(toggle_Device_Status_1);
  }
  else if (d1 == "0")
  {
    toggle_Device_Status_1 = LOW;
    digitalWrite(RelayPin1, !toggle_Device_Status_1);
    Serial.println(toggle_Device_Status_1);
  }

  if (d2 == "1")
  {
    toggle_Device_Status_2 = HIGH;
    digitalWrite(RelayPin2, !toggle_Device_Status_2);
    Serial.println(toggle_Device_Status_2);
  }
  else if (d2 == "0")
  {
    toggle_Device_Status_2 = LOW;
    digitalWrite(RelayPin2, !toggle_Device_Status_2);
    Serial.println(toggle_Device_Status_2);
  }

  if (d3 == "1") 
  {
    toggle_Device_Status_3 = HIGH;
    digitalWrite(RelayPin3, !toggle_Device_Status_3);
    Serial.println(toggle_Device_Status_3);
  }
  else if (d3 == "0")
  {
    toggle_Device_Status_3 = LOW;
    digitalWrite(RelayPin3, !toggle_Device_Status_3);
    Serial.println(toggle_Device_Status_3);
  }

  if (d4 == "1")
  {
    toggle_Device_Status_4 = HIGH;
    digitalWrite(RelayPin4, !toggle_Device_Status_4);
    delay(50000);
    digitalWrite(RelayPin4, toggle_Device_Status_4);
    delay(90000);
    Serial.println(toggle_Device_Status_4);
  }
  else if (d4 == "0")
  {
    toggle_Device_Status_4 = LOW;
    digitalWrite(RelayPin4, !toggle_Device_Status_4);
    Serial.println(toggle_Device_Status_4);
  }
}

void getSensorData()
{
  humidity1 = dht.readHumidity();
  Serial.println(humidity1);
  temperature1 = dht.readTemperature();
  Serial.println(temperature1);
  output_value = analogRead(sensor_pin);
  output_value = map(output_value, 550, 0, 0, 100);
  Serial.println(output_value);
}

void DBWrite()
{
  Firebase.setFloat("humidity1", humidity1);
  Firebase.setFloat("temperature1", temperature1);
  Firebase.setFloat("moisture1", output_value);
  delay(1000);
  if(output_value != 0)
  {
      if ( output_value <= 20 )
      {
        Firebase.setString("device2", "1");
      }
      else if (output_value >= 50)
      {
        Firebase.setString("device2", "0");
      }
  }
  
}

void loop()
{
  chageDeviceStatus();
  DBWrite();
  getSensorData();
  digitalWrite(wifiLed, 1);
  Serial.println(output_value);
}
