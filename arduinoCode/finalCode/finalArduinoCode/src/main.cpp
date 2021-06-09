#include <Arduino.h>
//STEPPER LIBRARY
#include <AccelStepper.h>
#define MotorInterfaceType 4
//RFID READER
#include <MFRC522.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "WIFIconfig.h"

//für esp8266 pin 15 & 0, bei arduino uno 10 & 9
#define SS_PIN 15
#define RST_PIN 0
String backendIP = "http://192.168.178.34:3000";
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
//STEPPER
int stepdir = 0;
AccelStepper stepper1 = AccelStepper(MotorInterfaceType, D0, D1, D2, D4);

void calibration(){
  Serial.println("kalibrierung");
  int endPoint1 = digitalRead(A0);
  Serial.print("Endpoint 1 is: ");
  Serial.println(endPoint1);
  stepdir = 0;
  
  stepper1.setSpeed(-500);
  do {
  stepper1.runSpeed();
  Serial.print(endPoint1);
  yield();
  } while(digitalRead(A0) == 0);
  
  stepper1.setSpeed(500);
  stepper1.runSpeed();
  delay(1000);
  stepper1.stop();

}
void setup() {
  // put your setup code here, to run once:
  //STEPPER
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000);
  pinMode(A0 ,INPUT_PULLUP);
  //RFID & WIFI
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //calibration();
  /*WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.println("Waiting for connection");
  }*/
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
    HTTPClient httpGet;

    //Ziel für GET-Request
    String url = backendIP + "/plant/status/" + rdifUID;
    httpGet.begin(client, url);
    httpGet.addHeader("Content-Type", "application/json");
    // Das Ziel für den Post-Request definieren 
    // Http-Request  über den WifiClient verschicken
    // Achtung IP-Adresse vom Node-Server hinterlegen, IP-Adresse 
    // von dem Geräte hinterlegen auf dem euer Server läuft
    String url2 = backendIP + "/plant/register";
    http.begin(client, url2);

    // Den Content-Type für den Header definieren
    http.addHeader("Content-Type", "application/json");

    // Den Request verschicken, Request Payload 
    // JSON-String: {"temperatur": "0"}
    String jsonString = "{\"uid\": \""+rdifUID+"\"}";
    int httpGETCode = httpGet.GET(); 
    if(httpGETCode != 200){
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
      Serial.println("Plant already exists in db");
    }
    httpGet.end();
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
}
//Pflanzen infos abrufen (falls vorhanden)
void getPlantInfoDB(String rdifUID){
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println(rdifUID);
    WiFiClient client;
    HTTPClient http;
    String url = backendIP + "/plant/status/" + rdifUID;
    http.begin(client, url);

    // Den Content-Type für den Header definieren
    http.addHeader("Content-Type", "application/json");

    // Den Request verschicken, Request Payload
    String jsonString = "{\"uid\": \""+rdifUID+"\"}";
    int httpCode = http.GET(); 

    // Antwort: Payload
    if(httpCode > 0){
    String payload = http.getString(); 
    // Ausgabe des Return Codes
    Serial.println(httpCode);
    if(httpCode == 404){

    }
    else if(httpCode == 200) {
      addNewUIDToDB(rdifUID);
    }
    // Ausgabe des Antwort Payloads
    Serial.println(payload);    //Print request response payload

    // Http_connection schließen
    http.end();
    }          
  } else {
    Serial.println("Error in WiFi connection");
  }
}
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
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
  //addNewUIDToDB(myString);
  myString = "";
}
void searchForRFID(){
  Serial.println("looking for freedom");
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
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
 searchForRFID();
}
//Kalibrierungsfunktion die Motor an Endschalter fährt

//Funktion die Schlitten fahren lässt bis ein RFID code gescannt wird, max bis 2ter Endschalter

//Bei erkannter RFID: DB abfrage neue pflanze, wenn nicht Gießabfrage, danach weiterfahren in selbe richtung

//Funktion neue Pflanze anlegen bei unbekanntem RFID code

//Funktion Gießabfrage

//Funktion um zu gießen, Motor high schalten für ? sekunden