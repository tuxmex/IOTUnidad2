
#include <Arduino.h>
#include "pitches.h"
#include <WiFi.h>
#include <WebServer.h>

#include <SPI.h>
#include "LedMatrix.h"


#define NUMBER_OF_DEVICES 8 //number of led matrix connect in series
#define CS_PIN 15
#define CLK_PIN 18
#define MISO_PIN 19 //we do not use this pin just fill to match constructor
#define MOSI_PIN 23

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);


const int BUZZER_PIN = 5; // GIOP05 pin connected to piezo buzzer
const int LED_PIN = 4;

WebServer server(80);

#define WIFI_SSID "PANIRED_2.4"
#define WIFI_PASSWORD "Malenarg01"

int status = 0;
String user = "Anonimo";

void getStatus(){
 String data = "{\"status\":\""+String(status)+"\", \"user\":\""+String(user)+"\"}";
 server.send(200, "application/json", data);
  
}

void powerOn(){
  status = 1;
  user="TODO";
  initDisplay("Atender a: "+String(user));
  String data = "{\"status\":\""+String(status)+"\", \"user\":\""+String(user)+"\"}";
  server.send(200, "application/json", data);

}
void powerOff(){
  status = 0;
  initDisplay("Atendido a: "+String(user));
  String data = "{\"status\":"+String(status)+"}";
  server.send(200, "application/json", data);
}

void attendOn(){
  status = 1;
  song();
  String data = "{\"status\":"+String(status)+"}";
  server.send(200, "application/json", data);
  

}
void attendOff(){
  status = 0;
  noSound();
  String data = "{\"status\":"+String(status)+"}";
  server.send(200, "application/json", data);
}

void setupWifi() {
  Serial.print("Start connect WIFi Access");
  //WiFi.begin("UTNG_Docencia", "UtNg2020@#$");
  //WiFi.begin("PANIRED_2.4", "Malenarg01");
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("..");
    
  }
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

}



void setupWebServer(){
  Serial.print("Start server setup");
  server.on("/", getStatus);
  server.on("/on", powerOn);
  server.on("/off", powerOff);
  server.on("/att-on", attendOn);
  server.on("/att-off", attendOff);
  


  server.begin();
  Serial.println("... started");
}

void initDisplay(String text){
   ledMatrix.init();
   ledMatrix.setText(text);
}

void  display(){
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(50);
}
  
void setup() {
     // Configuration serial
  Serial.begin(115200);
  //Setup WiFi
  setupWifi();
  delay(100);

  //Setup web server
  setupWebServer();
  delay(100);

  Serial.println("Setup done");

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  

  
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(BUZZER_PIN, channel);

  //Display
  initDisplay("Display Iniciado!");

 
}



void loop() {

  //Relay working
  if(status==1){
    digitalWrite(LED_PIN, HIGH);
    display();
   
  }else if(status == 0){
      digitalWrite(LED_PIN, LOW);
      display();
  }

  //Server
  server.handleClient();
  //delay(100);

  //Wait...
  //delay(1000);
}

 
