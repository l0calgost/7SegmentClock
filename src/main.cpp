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
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600);

// Neopixel config
#define PIN 2 
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 86 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// color we use
const uint32_t RED  = pixels.Color(255, 0, 0);
// number of leds per segment
const int SEGMENT_LEDS = 3;
// digit offset (number of leds per digit)
const int DIGIT_OFFSET = 7 * SEGMENT_LEDS;


void updateHours(int);
void updateMinutes(int);
void flipColons(int);
std::vector<int> getDigits(int);
void setDigits(std::vector<int>, int);
void setPixels(int, int);
void setZero(int);
void setOne(int);
void setTwo(int);
void setThree(int);
void setFour(int);
void setFive(int);
void setSix(int);
void setSeven(int);
void setEight(int);
void setNine(int);
void clearDigit(int);

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
  flipColons(seconds);
  pixels.show();

  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}

void updateHours(int hours) {
  std::vector<int> digits = getDigits(hours);
  setDigits(digits, 0);
}

void updateMinutes(int minutes) {
  std::vector<int> digits = getDigits(minutes);
  int offset = 2 * DIGIT_OFFSET + 2;// two hour digits + colons
  setDigits(digits, offset);
}

void setDigits(std::vector<int> digits, int offset) {
  for (int i = 0; i < digits.size(); i++){
    setPixels(digits[i], offset);
    offset = offset + DIGIT_OFFSET;
  }
}

void setPixels(int digit, int offset) {
  if (digit == 1){
    setOne(offset);
  }else if (digit == 2){
    setTwo(offset);
  }else if (digit == 3){
    setThree(offset);
  }else if (digit == 4){
    setFour(offset);
  }else if (digit == 5){
    setFive(offset);
  }else if (digit == 6){
    setSix(offset);
  }else if (digit == 7){
    setSeven(offset);
  }else if (digit == 8){
    setEight(offset);
  }else if (digit == 9){
    setNine(offset);
  } else {
    setZero(offset);
  }
}

/*
Segment is build like this:
    151617
  14      18
  13      19
  12      20
    11109
  0       8
  1       7
  2       6
     345

*/
void setZero(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset, SEGMENT_LEDS * 3);
  pixels.fill(RED, offset + SEGMENT_LEDS * 4, SEGMENT_LEDS * 3);
}
void setOne(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS * 2, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 6, SEGMENT_LEDS);
}
void setTwo(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset, SEGMENT_LEDS * 2);
  pixels.fill(RED, offset + SEGMENT_LEDS * 3, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 5, SEGMENT_LEDS * 2);
}
void setThree(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS, SEGMENT_LEDS * 3);
  pixels.fill(RED, offset + SEGMENT_LEDS * 5, SEGMENT_LEDS * 2);
}
void setFour(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS * 2, SEGMENT_LEDS * 3);
  pixels.fill(RED, offset + SEGMENT_LEDS * 6, SEGMENT_LEDS);
}
void setFive(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS, SEGMENT_LEDS * 5);
}
void setSix(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset, SEGMENT_LEDS * 6);
}
void setSeven(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS * 2, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 5, SEGMENT_LEDS * 2);
}
void setEight(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset, SEGMENT_LEDS * 7);
}
void setNine(int offset) {
  clearDigit(offset);
  pixels.fill(RED, offset + SEGMENT_LEDS, SEGMENT_LEDS * 6);
}

void clearDigit(int offset) {
  pixels.fill(0, offset, SEGMENT_LEDS);
}

std::vector<int> getDigits(int value) {
  std::vector<int> digits;
  if (value < 10){
    digits.push_back(0);
    digits.push_back(value);
  } else {
    digits.push_back(value / 10);
    digits.push_back(value % 10);
  }
  return digits;
}
void flipColons(int seconds) {
  if (seconds % 2 == 0){
    pixels.fill(RED, 42, 2);
  } else {
    pixels.fill(0, 42, 2);
  }
}
