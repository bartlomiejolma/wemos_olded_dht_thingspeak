#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include "DHT.h"
#include "my_credentials.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient client;
int counter = 0;

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
 
#define DHTPIN D4
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);


const int led = 2;
 
void setup()   
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  dht.begin();
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.display();

    WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
}
 
 
void loop() 
{
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(64,0);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //temp in c
  display.println("temp");
  display.print(t);
  display.println(" c");
  display.println("humidity");
  display.print(h);
  display.println(" %");
  display.display();

  counter += 1;

  if(counter >= 10)
  {
    counter = 0;
    if (client.connect(server,80)) 
    {
      String postStr = apiKey;
      postStr +="&field4=";
      postStr += String(t);
      postStr +="&field5=";
      postStr += String(h);

       
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
           
   }
   client.stop();
  }


        

}
 
