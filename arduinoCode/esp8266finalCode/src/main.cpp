#include <Arduino.h>
//STEPPER LIBRARY
#include <AccelStepper.h>
#define MotorInterfaceType 4
//RFID READER

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "WIFIconfig.h"

//für esp8266 pin 15 & 0, bei arduino uno 10 & 9
#define SS_PIN D8
#define RST_PIN D3
String backendIP = "http://192.168.178.34:3000";
// Set AP credentials
const char* ssid = "ESPRemoteGardening";
const char* password = "aperoli";

AsyncWebServer server(80);
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
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
 
  // Start server
  server.begin();
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

void loop() {
  // stepper1.setSpeed(400);
  //stepper1.runSpeed();
  //delay(10000);
}
//Kalibrierungsfunktion die Motor an Endschalter fährt

//Funktion die Schlitten fahren lässt bis ein RFID code gescannt wird, max bis 2ter Endschalter

//Bei erkannter RFID: DB abfrage neue pflanze, wenn nicht Gießabfrage, danach weiterfahren in selbe richtung

//Funktion neue Pflanze anlegen bei unbekanntem RFID code

//Funktion Gießabfrage

//Funktion um zu gießen, Motor high schalten für ? sekunden