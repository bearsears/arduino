//part of the code was from programmingboss.com
//setting it up for esp32 main program.
#include "WiFi.h"
#include "ThingSpeak.h"
//#define RXp2 16
//#define TXp2 17

//pin for receive and transmit between arduino
const int RXp2 = 16;
const int TXp2 = 17;

//testing variable
String a = "";

//wifi id and password, put your own credentials, and always never save
//this part.
const char* ssid = "fakeap"; //write ap name
const char* password = "fakepass"; //write ap password
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "fakeapi";   // Write API KEY
WiFiClient client;

long int grandPa = 0;


//if you call function in setup() you need to declare it before.
//if you call function in loop() you can declare it after.
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
  delay(500);
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
  ThingSpeak.begin(client);
}

void loop() {
  //did he already fall?
  
  Serial.println("Waiting for a signal");
  while (a == "") {
    a = Serial2.readString();
    Serial.print(a);
    delay(1000);
  }
  int x = 0;
  int l = a.length();
  x = ThingSpeak.writeField(myChannelNumber, 1, l, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful");
    delay(10000);
  }

  a = "";
  /* else {
    Serial.println("Didn't get to the server, try again");
  }
  delay(10000);
  */
}
