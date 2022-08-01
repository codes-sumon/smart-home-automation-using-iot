
//url for google assistent voice control.
//https://sgp1.blynk.cloud/external/api/update?token=nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg&V1=1
//https://sgp1.blynk.cloud/external/api/update?token=nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg&V1=0
//https://sgp1.blynk.cloud/external/api/update?token=nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg&V2=1
//https://sgp1.blynk.cloud/external/api/update?token=nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg&V2=0
//https://sgp1.blynk.cloud/external/api/update?token=nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg&V5=1

#define BLYNK_TEMPLATE_ID "TMPL5nbz6-yA"
#define BLYNK_DEVICE_NAME "Smart Plant Management"
#define BLYNK_AUTH_TOKEN "nkmF8Ntxc4aNa6JmCjWWB7yrCywbx5tg"

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wrong";
char pass[] = "1234567b";

bool fetch_blynk_state = true;  //true or false



//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>        
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>  


#define DHTPIN              D4 //D4  pin connected with DHT

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11

//decode_results results;
  
// define the GPIO connected with Relays and switches
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 14 //D5
#define RelayPin4 12 //D6


#define wifiLed   16   //D0

//Change the virtual pins according the rooms
#define B_V_PIN_BUTTON_1    V1 
#define B_V_PIN_BUTTON_2    V2
#define B_V_PIN_BUTTON_3    V3 
#define B_V_PIN_BUTTON_4    V4

#define B_V_PIN_BUTTON_ALL_OFF    V5
#define VPIN_TEMPERATURE V6
#define VPIN_HUMIDITY    V7
#define VPIN_MOISTURE    V8

// Relay State
bool toggle_Device_Status_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggle_Device_Status_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggle_Device_Status_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggle_Device_Status_4 = LOW; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;
float temperature1 = 0;
float humidity1   = 0;
float moisture1 = 0;

char auth[] = BLYNK_AUTH_TOKEN;


BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

// When App button is pushed - switch the state

BLYNK_WRITE(B_V_PIN_BUTTON_1) {
  toggle_Device_Status_1 = param.asInt();
  digitalWrite(RelayPin1, !toggle_Device_Status_1);
  Serial.println(!toggle_Device_Status_1);
}

BLYNK_WRITE(B_V_PIN_BUTTON_2) {
  toggle_Device_Status_2 = param.asInt();
  digitalWrite(RelayPin2, !toggle_Device_Status_2);
  Serial.println(!toggle_Device_Status_2);
}

BLYNK_WRITE(B_V_PIN_BUTTON_3) {
  toggle_Device_Status_3 = param.asInt();
  digitalWrite(RelayPin3, !toggle_Device_Status_3);
  Serial.println(!toggle_Device_Status_3);
}

BLYNK_WRITE(B_V_PIN_BUTTON_4) {
  toggle_Device_Status_4 = param.asInt();
  digitalWrite(RelayPin4, !toggle_Device_Status_4);
  Serial.println(!toggle_Device_Status_4);
}

BLYNK_WRITE(B_V_PIN_BUTTON_ALL_OFF) {
  int a = param.asInt();
  Serial.println(a);
  if (a == 1)
  {
    all_SwitchOff();
    Blynk.virtualWrite(B_V_PIN_BUTTON_ALL_OFF, 0);
  }
}

void all_SwitchOff(){
  toggle_Device_Status_1 = 0; digitalWrite(RelayPin1, HIGH); Blynk.virtualWrite(B_V_PIN_BUTTON_1, toggle_Device_Status_1); delay(100);
  toggle_Device_Status_2 = 0; digitalWrite(RelayPin2, HIGH); Blynk.virtualWrite(B_V_PIN_BUTTON_2, toggle_Device_Status_2); delay(100);
  toggle_Device_Status_3 = 0; digitalWrite(RelayPin3, HIGH); Blynk.virtualWrite(B_V_PIN_BUTTON_3, toggle_Device_Status_3); delay(100);
  toggle_Device_Status_4 = 0; digitalWrite(RelayPin4, HIGH); Blynk.virtualWrite(B_V_PIN_BUTTON_4, toggle_Device_Status_4); delay(100);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_MOISTURE, moisture1);
  
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    //Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, HIGH);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(B_V_PIN_BUTTON_1, toggle_Device_Status_1);
    Blynk.virtualWrite(B_V_PIN_BUTTON_2, toggle_Device_Status_2);
    Blynk.virtualWrite(B_V_PIN_BUTTON_3, toggle_Device_Status_3);
    Blynk.virtualWrite(B_V_PIN_BUTTON_4, toggle_Device_Status_4);
    }
    digitalWrite(wifiLed, LOW);
    //Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(B_V_PIN_BUTTON_1);
    Blynk.syncVirtual(B_V_PIN_BUTTON_2);
    Blynk.syncVirtual(B_V_PIN_BUTTON_3);
    Blynk.syncVirtual(B_V_PIN_BUTTON_4);
  }
    Blynk.syncVirtual(VPIN_TEMPERATURE);
    Blynk.syncVirtual(VPIN_HUMIDITY);
    Blynk.syncVirtual(VPIN_MOISTURE);
}

void readSensor(){
  
  float h = 56; //dht.readHumidity();
  float t = 33; //dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  float m = 70;
  if (isnan(h) || isnan(t)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    humidity1 = h;
    temperature1 = t;
    moisture1 = m;
    //Serial.println(temperature1);
    //Serial.println(humidity1);
    //Serial.println(moisture1);
  }  
}

void sendSensor()
{
  readSensor();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VPIN_MOISTURE, moisture1);
}

void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);


  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggle_Device_Status_1);
  digitalWrite(RelayPin2, !toggle_Device_Status_2);
  digitalWrite(RelayPin3, !toggle_Device_Status_3);
  digitalWrite(RelayPin4, !toggle_Device_Status_4);

  dht.begin();    // Enabling DHT sensor

  digitalWrite(wifiLed, HIGH);

  //Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  timer.setInterval(1000L, sendSensor); // Sending Sensor Data to Blynk Cloud every 1 second
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(B_V_PIN_BUTTON_1, toggle_Device_Status_1);
    Blynk.virtualWrite(B_V_PIN_BUTTON_2, toggle_Device_Status_2);
    Blynk.virtualWrite(B_V_PIN_BUTTON_3, toggle_Device_Status_3);
    Blynk.virtualWrite(B_V_PIN_BUTTON_4, toggle_Device_Status_4);
  }
}

void loop()
{ 

  Blynk.run();
  timer.run(); // Initiates SimpleTimer

}
