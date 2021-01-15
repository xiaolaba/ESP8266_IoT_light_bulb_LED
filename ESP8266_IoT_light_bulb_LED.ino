
//REF : https://create.arduino.cc/projecthub/ROBINTHOMAS/esp8266-esp-01-webserver-7248ca
//2017-02-19, test done, xiaolaba
// ESP-01 module
// gpio0 connect to GND, power up, ESP8266 is then in flash mode
// burn code to ESP-01 module

#include <ESP8266WiFi.h>

const char* ssid = "your wifi name";//change and type your WIFI ssid
const char* password = "your wifi password";//change and type your password

IPAddress ip(192, 168, 1, 188);  //Node static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

int onbroadledPin = 1; // GPIO1 of ESP-01, connect to onboard blue LED
int ledPin = 2; // GPIO2 of ESP8266, ESP-01
WiFiServer server(80);

void led_on (){
digitalWrite(ledPin, LOW);
digitalWrite(onbroadledPin, LOW);
}

void led_off (){
digitalWrite(ledPin, HIGH);
digitalWrite(onbroadledPin, HIGH);
}

void setup() {
Serial.begin(115200);
delay(10);

// diable gpio1, no use onboard blue LED
//  PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1); //use pin as GPIO1 instead of UART TXD
//  pinMode(onbroadledPin, OUTPUT);

pinMode(ledPin, OUTPUT);
//digitalWrite(ledPin, LOW);
led_off();

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

// Static IP Setup Info Here...
WiFi.config(ip, gateway, subnet);
//WiFi.config(ip, dns, gateway, subnet); //If you need Internet Access You should Add DNS also..

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
led_on(); //fast LED blinking loop, until WIFI is connected
delay(50);
Serial.print(".");
led_off();
delay(50);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
delay(500);

// Start the server
server.begin();
Serial.println("Server started");

// slow blinking 5 times, indicate ESP-01 server is started
for (int i= 0; i<5; i++) {
led_on();
delay(300);
led_off();
delay(300);
}


// Print the IP address
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");

}

void loop() {
// Check if a client has connected
WiFiClient client = server.available();
if (!client) {
return;
}

// Wait until the client sends some data
Serial.println("new client");
while(!client.available()){
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("/LED=ON") != -1) {
//digitalWrite(ledPin, HIGH);
led_on();
value = HIGH;
}
if (request.indexOf("/LED=OFF") != -1){
//digitalWrite(ledPin, LOW);
led_off();
value = LOW;
}

// Set ledPin according to the request
//digitalWrite(ledPin, value);


// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  do not forget this one
client.println("<!DOCTYPE HTML>");
client.println("<html>");

if(value == HIGH) {
client.print("LED is OFF");
client.println("<br><br>");
client.println("Click <a href=\"/LED=OFF\">here</a> to turn ON LED on pin 2. hello xiaolaba. ESP8266 web server<br>");
} else {
client.print("LED is ON");
client.println("<br><br>");
client.println("Click <a href=\"/LED=ON\">here</a> to turn OFF LED on pin 2. hello xiaolab. ESP8266 web server<br>");

}
client.println("</html>");

delay(1);
Serial.println("Client disonnected");
Serial.println("");

}
