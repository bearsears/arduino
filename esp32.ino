//part of the code was from programmingboss.com
//setting it up for esp32 main program.
#include "WiFi.h"
#define RXp2 16
#define TXp2 17

String a = "";
const char* ssid = "";
const char* password = "";

void get_network_info(){
  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("[*] Network information for ");
    Serial.println(ssid);

    Serial.println("[+] BSSID : " + WiFi.BSSIDstr());
    Serial.print("[+] Gateway IP : ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("[+] Subnet Mask : ");
    Serial.println(WiFi.subnetMask());
    Serial.println((String)"[+] RSSI : " + WiFi.RSSI() + " dB");
    Serial.print("[+] ESP32 IP : ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  // put your setup code here, to run once:
  //setting up wifi
  Serial.begin(921600);
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(3000);
  }
  Serial.println("\nConnected to the Wifi network");
  get_network_info();

  //setting up connection to arduino
  Serial.println("Connecting to Arduino");
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}

void loop() {
  Serial.println("Waiting for a signal");
  while (a == "") {
    a = Serial2.readString();
    delay(100);
  Serial.println(a);
  }
}
