#include <Arduino.h>

#include <Arduino.h>
//STEPPER LIBRARY
#include <AccelStepper.h>
#define MotorInterfaceType 4
//RFID READER

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "WIFIconfig.h"

//rfid reaDER
#include <MFRC522.h>
#define SS_PIN D1
#define RST_PIN D3

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

String backendIP = "http://192.168.1.209:3000";
int rfidStatus = 0;

//STEPPER
int stepdir = 0;
AccelStepper stepper1 = AccelStepper(MotorInterfaceType, D0, D1, D2, D4);
void setup() {
  // put your setup code here, to run once:
  //STEPPER
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  stepper1.setMaxSpeed(1000);
  //calibration();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
}
void waterPlant(int duration){
  Serial.print("Watering plant for ");
  Serial.print(duration);
  Serial.println(" milliseconds! :)");
  //digitalWrite(pumpPin, HIGH);
  delay(duration);
  //digitalWrite(pumpPin, LOW);
  Serial.println("Watered plant sucessfully");
  Serial.println(digitalRead(SS_PIN));
  rfidStatus = 0;
}
//Pflanze in unsere Datenbank hinzufügen
void addNewUIDToDB(String rdifUID) {
  
  // Wenn eine Wifi-Verbindung besteht
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println(rdifUID);
    // EIn Objekt der Klasse WifiClient erzeugen
    WiFiClient client;
    // Eine Objekt der Klasse HTTPClient erzeugen
    HTTPClient http;
    // Das Ziel für den Post-Request definieren 
    // Http-Request  über den WifiClient verschicken
    // Achtung IP-Adresse vom Node-Server hinterlegen, IP-Adresse 
    // von dem Geräte hinterlegen auf dem euer Server läuft
    String url = backendIP + "/plant/register";
    http.begin(client, url);
    // Den Content-Type für den Header definieren
    http.addHeader("Content-Type", "application/json");

    // Den Request verschicken, Request Payload 
    // JSON-String: {"temperatur": "0"}
    String jsonString = "{\"uid\": \""+rdifUID+"\"}";
    int httpCode = http.POST(jsonString); 
    // Antwort: Payload
    String payload = http.getString();                  //Get the response payload

    // Ausgabe des Return Codes
    Serial.println(httpCode);   //Print HTTP return code

    // Ausgabe des Antwort Payloads
    Serial.println(payload);    //Print request response payload

    // Http_connection schließen
    
    http.end();
    }
    else {
    Serial.println("Error in WiFi connection");
  }
}
//Pflanzen infos abrufen (falls vorhanden)
void getPlantInfoDB(String rdifUID){
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println(rdifUID);
    HTTPClient http;
    String url = backendIP + "/plant/status/" + rdifUID;
    Serial.println(url);
    http.begin(url.c_str());
    int httpCode = http.GET(); 
    // Antwort: Payload
    if(httpCode > 0){
    String payload = http.getString(); 
    // Ausgabe des Return Codes
    Serial.println(httpCode);
    if(httpCode == 200){
    Serial.println(payload); 
    waterPlant(payload.toInt());
    //waterPlant(payload) for ... ms
    }
    else if(httpCode == 404) {
      addNewUIDToDB(rdifUID);
    }
    // Ausgabe des Antwort Payloads   //Print request response payload
    // Http_connection schließen
    http.end();
    }       
  } else {
    Serial.println("Error in WiFi connection");
  }
}
void printDec(byte *buffer, byte bufferSize) {
  String myString;
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    myString = myString + (String)buffer[i];
  }
  Serial.println();
  Serial.print("in String: ");
  Serial.println(myString);
  getPlantInfoDB(myString);
  myString = "";
}
void lookForNewRFIDUID(){
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//Wenn wir eine neue Karte erkennen
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));
    rfidStatus = 1; //pause stepper
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void loop() {
  // stepper1.setSpeed(400);
  //stepper1.runSpeed();
  //delay(10000);
  if(rfidStatus == 0){
    //run Stepper
    lookForNewRFIDUID();
  }
  else{
    //Serial.println("Doing Something");
    //do something
  }
}
//Kalibrierungsfunktion die Motor an Endschalter fährt

//Funktion die Schlitten fahren lässt bis ein RFID code gescannt wird, max bis 2ter Endschalter

//Bei erkannter RFID: DB abfrage neue pflanze, wenn nicht Gießabfrage, danach weiterfahren in selbe richtung

//Funktion neue Pflanze anlegen bei unbekanntem RFID code

//Funktion Gießabfrage

//Funktion um zu gießen, Motor high schalten für ? sekunden