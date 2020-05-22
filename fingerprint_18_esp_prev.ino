#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal595.h>    // LCD library
LiquidCrystal595 lcd(23, 5, 18);    // data_pin, latch_pin, clock_pin
#include <Adafruit_Fingerprint.h>
#include <ctype.h>

HardwareSerial mySerial(2);

const char* ssid     = "Home Network_EXT";
const char* password = "********";

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Unique IFTTT URL resource
const char* resource = "/trigger/ds3231_readings/with/key/5cJx86kGkz3CqwAPVIATaS70qbjER8d2gocXLxcV-S";

int signoutButton = 4; 
int deleteButton = 0; 
int signinButton = 34;
//int lavButton = 35;

boolean deleteButtonWasLow = false;
boolean signoutbuttonWasLow = false;
boolean signinbuttonWasLow = false;

int led = 35;

String roomNum = "";


const byte ROWS = 5;
const byte COLS = 4;

/*char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}

  };*/

char keys[ROWS][COLS] = {
  {'D', '#', '9', '6'},
  {'C', '0', '8', '5'},
  {'B', '*', '7', '4'},
  {'A', '3', '2', '1'},
  {'E', 'F', 'G', 'H'}
};


//byte rowPins[ROWS] = {14, 27, 26, 25, 35};
//byte colPins[COLS] = {32, 13, 12, 33};

byte rowPins[ROWS] = {14, 27, 26, 25, 2};
byte colPins[COLS] = {33, 12, 13, 32};

//byte rowPins[ROWS] = {35, 14, 27, 26, 25};
//byte colPins[COLS] = {33, 12, 13, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


String Names[] = {"L.Ring", "R.Pointer", "R.Middle", "R.Ring", "L.Pinky", "L.Thumb", "R.Pinky", "R.Thumb", "L.Pointer", "This is string 1"};

int signoutbuttonState = 0;         // current state of the button
int signoutlastButtonState = 0;

int deleteButtonState = 0;         // current state of the button
int deleteButtonlastButtonState = 0;

int signinbuttonState = 0;         // current state of the button
int signinlastButtonState = 0;

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 30 minutes = 1800 seconds
uint64_t TIME_TO_SLEEP = 20;

String jsonObject = String();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  finger.begin(57600);
  //lcd.print("place finger on sensor");
  //delay(2000);

  pinMode(signoutButton, INPUT); //set the button pin as INPUT
  pinMode(signinButton, INPUT); //set the button pin as INPUT
  pinMode(deleteButton, INPUT); //set the button pin as INPUT
  pinMode(led, OUTPUT); //set led as OUTPUT
  //digitalWrite(signoutButton, HIGH);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  lcd.print("Type room number");
  lcd.setCursor(0, 1);

}

void loop() {
  // put your main code here, to run repeatedly:
  //lcd.clear();
  signoutbuttonState = digitalRead(signoutButton);
  deleteButtonState = digitalRead(deleteButton);
  signinbuttonState = digitalRead(signinButton);

  /*if (digitalRead(deleteButton) == HIGH)  {
          deleteButtonWasLow = true;
      }*/
  char key = keypad.getKey();
  if (roomNum == "#") {
    roomNum = "LAVATORY";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }

  if (roomNum == "*") {
    roomNum = "LOCKER";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }

  if (roomNum == "A") {
    roomNum = "OFFICE";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }

  if (roomNum == "B") {
    roomNum = "AUDITORIUM";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }
  if (roomNum == "C") {
    roomNum = "GYM";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }
  if (roomNum == "D") {
    roomNum = "CAFETERIA";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }

  if (roomNum == "E") {
    roomNum = "GUIDANCE";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }
  if (roomNum == "F") {
    roomNum = "CHORUS ROOM";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }
  if (roomNum == "G") {
    roomNum = "LGI";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }
  if (roomNum == "H") {
    roomNum = "LIBRARY";
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
  }

  if (key) {

    roomNum = roomNum + key;
    if (roomNum.length() <= 3) {


      lcd.print(key);
      Serial.println(key);

      Serial.println(roomNum);

    } else if (roomNum == "LOCKER" or roomNum == "OFFICE" or roomNum == "AUDITORIUM" or roomNum == "CAFATERIA" or roomNum == "LAVATORY" or roomNum == "GYM" or roomNum == "LIBRARY"  or roomNum == "LGI"  or roomNum == "CHORUS ROOM"  or roomNum == "GUIDANCE") {
      lcd.print(key);
      Serial.println(key);

      Serial.println(roomNum);
    }
    else {
      roomNum.remove(3);
    }

  }

  if (deleteButtonState == 0)  {

    roomNum = "";
    lcd.clear();
    lcd.print("Type room number");
    lcd.setCursor(0, 1);


  }

  /*if (digitalRead(signoutButton) == HIGH)  {
        signoutbuttonWasLow = true;


    }*/

  /*if (digitalRead(signinButton) == HIGH)  {
      signinbuttonWasLow = true;


    }*/

  if (roomNum != "" && signinbuttonState == 1)  {


    lcd.setCursor(0, 0);
    lcd.print("Clear room num");
    lcd.setCursor(0, 1);
    lcd.print("before sign in");
    delay(1500);
    lcd.clear();
    lcd.print("Type room number");
    lcd.setCursor(0, 1);
    lcd.print(roomNum);
    //Serial.println(roomNum);



  }

  getFingerprintIDez();
  getFingerprintIDez1();

  /*#ifdef ESP32
    // enable timer deep sleep
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Going to sleep now");
    // start deep sleep for 3600 seconds (60 minutes)
    esp_deep_sleep_start();
    #else
    // Deep sleep mode for 3600 seconds (60 minutes)
    Serial.println("Going to sleep now");
    ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    #endif*/

}






int getFingerprintIDez() {
  if (signoutbuttonState == 1 )  {


    if (roomNum != "")
    {
      uint8_t p = finger.getImage();
      if (p != FINGERPRINT_OK)  return -1;

      p = finger.image2Tz();
      if (p != FINGERPRINT_OK)  return -1;

      p = finger.fingerFastSearch();
      if (p != FINGERPRINT_OK)  return -1;

      // found a match!
      lcd.clear();
      lcd.print("Signed out!");
      lcd.setCursor(0, 1);
      //rtc.begin();

      //lcd.print(finger.fingerID);           //If a match is found it shows the ID of the finger
      //lcd.setCursor(2,1);
      lcd.print(Names[finger.fingerID - 1] + " to " + roomNum); //Then the name we gave it to it and the -1 is to remove the shift



      digitalWrite(led, HIGH); // Turn the LED on
      delay(2000);
      digitalWrite(led, LOW); // Turn the LED on
      lcd.clear();
      lcd.print("Type room number");
      lcd.setCursor(0, 1);
      //lcd.print("Scan ready");
      initWifi();
      makeIFTTTRequest();

      roomNum = "";

      return finger.fingerID;
    }
  }
}


int getFingerprintIDez1() {
  if (signinbuttonState == 1)  {


    if (roomNum == "")
    {
      uint8_t p = finger.getImage();
      if (p != FINGERPRINT_OK)  return -1;

      p = finger.image2Tz();
      if (p != FINGERPRINT_OK)  return -1;

      p = finger.fingerFastSearch();
      if (p != FINGERPRINT_OK)  return -1;

      // found a match!
      lcd.clear();
      lcd.print("Signed In!");
      lcd.setCursor(0, 1);
      //rtc.begin();

      //lcd.print(finger.fingerID);           //If a match is found it shows the ID of the finger
      //lcd.setCursor(2,1);
      lcd.print(Names[finger.fingerID - 1]); //Then the name we gave it to it and the -1 is to remove the shift



      digitalWrite(led, HIGH); // Turn the LED on
      delay(2000);
      digitalWrite(led, LOW); // Turn the LED on
      lcd.clear();
      lcd.print("Type room number");
      lcd.setCursor(0, 1);
      //lcd.print("Scan ready");
      initWifi();
      makeIFTTTRequest1();

      roomNum = "";

      return finger.fingerID;
    }
  }
}

void initWifi() {
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20 * 4; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: ");
  Serial.print(millis());
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {

  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 10;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);


  // Temperature in Celsius
  //String jsonObject = String("Testing");
  //String jsonObject = String(" at:" + now.hour(), DEC + ':' + now.second(), DEC + "on" + daysOfTheWeek[now.dayOfTheWeek()] + " " + now.month(), DEC + '/' + now.day(), DEC + '/' + now.year(), DEC);

  // Comment the previous line and uncomment the next line to publish temperature readings in Fahrenheit
  String jsonObject = String("{\"value1\":\"") + (Names[finger.fingerID - 1]) + "\",\"value2\":\""
                      + ("Signed Out") + "\",\"value3\":\"" + (roomNum) + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
  //delay(2000);
  ESP.deepSleep(0);
  //Serial.print("it reaches this far");

}



void makeIFTTTRequest1() {

  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 10;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  //lcd.clear();
  //lcd.print("Signed Out!");
  //lcd.setCursor(0,1);
  //lcd.print(Names[finger.fingerID-1]);

  // Temperature in Celsius
  //String jsonObject = String("Testing");
  //String jsonObject = String(" at:" + now.hour(), DEC + ':' + now.second(), DEC + "on" + daysOfTheWeek[now.dayOfTheWeek()] + " " + now.month(), DEC + '/' + now.day(), DEC + '/' + now.year(), DEC);

  // Comment the previous line and uncomment the next line to publish temperature readings in Fahrenheit
  String jsonObject = String("{\"value1\":\"") + (Names[finger.fingerID - 1]) + "\",\"value2\":\""
                      + ("Signed in") + "\",\"value3\":\"" + ("") + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
  //delay(2000);
  ESP.deepSleep(0);
  //Serial.print("it reaches this far");
}
