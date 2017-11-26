#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
  Name: Gabriel Lucas Ferreira 
  Email: gabriel@vrp.com.br
  Date: 26/11/17 19:46
*/

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Constants
#define DHTPIN 8     // what pin we're connected to
#define RELAYPIN 10
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define XON 17
#define XOFF 19
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data
boolean newData = false;

void getHumidity();
void getTemperature();
void getTemperatureHumidity();
void showHumidity(int);
void showTemperature(int);
void openRelay();
void closeRelay();
void writeSerialData(String parameter, float data);
void writeSerialData(String parameter, String data);

void recvWithEndMarker();
void showNewData();

void setup()
{

  lcd.begin (16, 2);
  dht.begin();

  lcd.print("i ai vacilao");

  pinMode(RELAYPIN, OUTPUT);  // Set Pin connected to Relay as an OUTPUT
  digitalWrite(RELAYPIN, LOW);  // Set Pin to LOW to turn Relay OFF

  delay(5000);
  lcd.clear();
  Serial.begin(115200);
}

void loop()
{

  //  recvWithEndMarker();
  //  showNewData();
  if (Serial.available() > 0) {
    //    char c = (char) Serial.read();
    //    if (c == XON){
    //
    //    } else if ( )

    char command = Serial.readStringUntil('-').charAt(0);
    char parameter = Serial.readStringUntil('#').charAt(0);

    switch (command) {
      case 'T':
        // Return the current (T)emperature and (H)umidity
        getTemperatureHumidity();
        Serial.flush();
        break;
      case 'R':
        if (parameter == 'T') {
          openRelay();
        } else {
          closeRelay();
        }
        Serial.flush();
        break;
    }
  }
}

void openRelay() {
  digitalWrite(RELAYPIN, HIGH);  // Turn Relay ON
  writeSerialData("R", "true");
}

void closeRelay() {
  digitalWrite(RELAYPIN, LOW);  // Turn Relay ON
  writeSerialData("R", "false");
}

void getTemperature() {
  temp = dht.readTemperature();
  writeSerialData("T", temp);
  showTemperature(temp);
}

void getHumidity() {
  hum = dht.readHumidity();
  writeSerialData("H", hum);
  showHumidity(hum);
}

void getTemperatureHumidity() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  String value = "";
  value += temp;
  value += "/";
  value += hum;
  writeSerialData("TH", value);
  showTemperature(temp);
  showHumidity(hum);
}

void showTemperature(float t) {
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t, 1);

  lcd.print("c");

  //Mostra o simbolo do grau quadrado
  lcd.print((char)223);
}

void showHumidity(float h) {
  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(h, 1);
  lcd.print("%");
}

void writeSerialData(String parameter, float data) {
  String stringdata = "";
  stringdata += data;
  writeSerialData(parameter, stringdata);
}

void writeSerialData(String parameter, String data) {
  Serial.print("$"); // Send XON
  Serial.print(parameter + "-" + data);
  Serial.print("#"); // Send XOFF
}

