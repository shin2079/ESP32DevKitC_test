#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// WiFi network name and password:
const char * networkName = "xxx";
const char * networkPswd = "xxx";

const char * udpAddress = "xxx.xxx.xxx.xxx";
const int udpPort = 6666;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
Serial.begin(115200);
//Connect to the WiFi network
connectToWiFi(networkName, networkPswd);
}

void loop(){
//only send data when connected
if(connected){

int sensorValue = analogRead(36);

OSCMessage msg("/Dis");
msg.add(sensorValue);
//msg.add("salut c'est BEV");
udp.beginPacket(udpAddress, udpPort);
msg.send(udp);
udp.endPacket();
msg.empty();
/*
//Send a packet
udp.beginPacket(udpAddress,udpPort);
udp.printf("Seconds since boot: %u", millis()/1000);
udp.endPacket();
*/
}
//Wait for 1 second
delay(2);
}

void connectToWiFi(const char * ssid, const char * pwd){
Serial.println("Connecting to WiFi network: " + String(ssid));

// delete old config
WiFi.disconnect(true);
//register event handler
WiFi.onEvent(WiFiEvent);

//Initiate connection
WiFi.begin(ssid, pwd);

Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
switch(event) {
case SYSTEM_EVENT_STA_GOT_IP:
//When connected set
Serial.print("WiFi connected! IP address: ");
Serial.println(WiFi.localIP());
//initializes the UDP state
//This initializes the transfer buffer
udp.begin(WiFi.localIP(),udpPort);
connected = true;
break;
case SYSTEM_EVENT_STA_DISCONNECTED:
Serial.println("WiFi lost connection");
connected = false;
break;
}
}
