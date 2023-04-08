#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MQ135.h>


#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // create an OLED display object connected to I2C

// Network credentials Here
const char* ssid     = "AQI monitoring systen";
const char* password = "everything custom";

float ppm;
#define threshold    1000


//MQ135 mq135_sensor(GAS_SENSOR);

//Set web server port number to 80
WiFiServer server(80);
float t=0;
float h=0;
// Variable to store the HTTP request
String header;
//here we use 14 of ESP32 to read data
#define DHTPIN 4

//our sensor is DHT11 type
#define DHTTYPE DHT11
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

 
void setup()
{
Serial.begin(115200);

 if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

delay(2000);         // wait two seconds for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(0, 10);       // set position to display
  oled.println("Temp:");
  oled.println(t);
  oled.println("Humidity:");
  oled.println(h);
  oled.println("Air value:");
  oled.println(ppm);
  oled.display();              // display on OLED
  
Serial.println("DHT11 sensor!");
//call begin to start sensor
dht.begin();

WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}
  
void loop() {
  String getPage();
 //float rzero = mq135_sensor.getRZero();
   //float ppm = mq135_sensor.getPPM();
  //Serial.println (rzero);
 // MQ135 gasSensor=MQ135(GAS_SENSOR);
  float ppm=analogRead(A0);
  
  Serial.println (ppm);
   if (ppm< threshold ){

  Serial.println ("fresh air");}
  else
  {
     Serial.println ("bed air");
  }
delay(500);
  WiFiClient client = server.available();
//use the functions which are supplied by library.
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Check if any reads failed and exit early (to try again).

  oled.clearDisplay(); // clear display

  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(0, 10);       // set position to display
  oled.println("Temp:");
  oled.println(t);
  oled.println("Humidity:");
  oled.println(h);
  oled.println("AQI:");
  oled.println(ppm);
  oled.display(); 

Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
//we delay a little bit for next read
delay(2000);
   client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>CRETOR: Electronic DIY</h1>"); //change the creator name in this line
            client.println("<p>Temperature:</p>");
            client.println(t);
            client.println("<p>Humidity:</p>");
            client.println(h);
            client.println("<p>Air Quality:</p>");
            client.println(ppm);
            client.println("<p><a href=\"https://everythingcustom.netlify.app\" target=\"_blank\">click hear</a></p>");

       
}
