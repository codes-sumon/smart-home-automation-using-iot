#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "DHT.h"
// Set these to run example.
#define FIREBASE_HOST "nodemcuconnfirebasetest-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HHkCS1zgUIOAHEvKV2mqUHcLUupWvjfbs9IkseEK"
#define WIFI_SSID "Wrong"
#define WIFI_PASSWORD "1234567b"

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
  Serial.begin(9600);
  dht.begin();

  // define the GPIO connected with Relays and switches
  #define RelayPin1 5  //D1
  #define RelayPin2 4  //D2
  #define RelayPin3 14 //D5
  #define RelayPin4 12 //D6
  #define wifiLed   1   //TX

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
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");
    digitalWrite(wifiLed, 1);
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  digitalWrite(wifiLed, 0);
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void chageDeviceStatus()
{
  toggle_Device_Status_1 = Firebase.getFloat("device1");
  digitalWrite(RelayPin1, !toggle_Device_Status_1);
  Serial.println(toggle_Device_Status_1);

  toggle_Device_Status_2 = Firebase.getFloat("device2");
  digitalWrite(RelayPin2, !toggle_Device_Status_2);
  Serial.println(toggle_Device_Status_2);

  toggle_Device_Status_3 = Firebase.getFloat("device3");
  digitalWrite(RelayPin3, !toggle_Device_Status_3);
  Serial.println(toggle_Device_Status_3);

  toggle_Device_Status_4 = Firebase.getFloat("device4");
  digitalWrite(RelayPin4, !toggle_Device_Status_4);
  Serial.println(toggle_Device_Status_4);

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

  if ( output_value <= 20 )
  {
    Firebase.setInt("device1", 1);
  }
  else if (output_value >= 50)
  {
    Firebase.setInt("device1", 0);
  }
}

void loop()
{
  chageDeviceStatus();
  DBWrite();
  getSensorData();
  Serial.println(output_value);
}
