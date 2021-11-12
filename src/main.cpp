#include <Arduino.h>

#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <config.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// wifi config
const char *ssid     = WLAN_SSID;
const char *password = WLAN_PASSWORD;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Neopixel config
#define PIN 2 
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 86 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

const uint32_t RED  = pixels.Color(255, 0, 0);

void updateHours(int);
void updateMinutes(int);
void updateColons(int);



void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  pixels.begin();

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, RED);
    pixels.show();
    delay(30);
  }
  boolean pixelsOn = false;
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    if (pixelsOn){
    pixels.clear();
    pixelsOn = false;
    } else {
      pixels.fill(RED, 0, 30);
      pixelsOn = true;
    }
    pixels.show();
  }
  timeClient.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  updateHours(hours);
  updateMinutes(minutes);
  updateColons(seconds);
  
  pixels.show();
  delay(1000);
}

void updateHours(int hours) {
  Serial.println("" + hours);
}
void updateMinutes(int minutes) {

}

void updateColons(int seconds) {
  if (seconds % 2 == 0){
    pixels.fill(RED, 42, 2);
  } else {
    pixels.fill(0, 42, 2);
  }
}
