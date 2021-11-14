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
const uint32_t RED  = pixels.Color(125, 0, 0);
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

  int index = 0;
  wl_status_t connectionState = WL_DISCONNECTED;

  while (connectionState != WL_CONNECTED) {
    delay (50);
    pixels.fill(RED, index, SEGMENT_LEDS);
    pixels.setPixelColor(index - SEGMENT_LEDS + 2, 0);
    index++;
    if(index % 10 == 0) {
      connectionState = WiFi.status();
    }
    if(pixels.numPixels() == index) {
      index = 0;
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

  updateMinutes(minutes);
  updateHours(hours);
  flipColons(seconds);
  pixels.show();

  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}

void updateHours(int hours) {
  std::vector<int> digits = getDigits(hours);
  int offset = 2 * DIGIT_OFFSET + 2;// two minute digits + colons
  setDigits(digits, offset);
}

void updateMinutes(int minutes) {
  std::vector<int> digits = getDigits(minutes);
  setDigits(digits, 0);
}

void setDigits(std::vector<int> digits, int offset) {
  for (int i = digits.size(); i >= 0; i--){
    setPixels(digits[i], offset);
    offset = offset + DIGIT_OFFSET;
  }
}

void setPixels(int digit, int offset) {
  clearDigit(offset);
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
     543
  6       2
  7       1
  8       0
    91011
  20      12
  19      13
  18      14
    171615

*/

void setZero(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 3);
  pixels.fill(RED, offset + SEGMENT_LEDS * 4, SEGMENT_LEDS * 3);
}
void setOne(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 4, SEGMENT_LEDS);
}
void setTwo(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 2);
  pixels.fill(RED, offset + SEGMENT_LEDS * 3, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 5, SEGMENT_LEDS * 2);
}
void setThree(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 2);
  pixels.fill(RED, offset + SEGMENT_LEDS * 3, SEGMENT_LEDS * 3);
}
void setFour(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS);
  pixels.fill(RED, offset + SEGMENT_LEDS * 2, SEGMENT_LEDS * 3);
}
void setFive(int offset) {
  pixels.fill(RED, offset + SEGMENT_LEDS, SEGMENT_LEDS * 5);
}
void setSix(int offset) {
  pixels.fill(RED, offset + SEGMENT_LEDS, SEGMENT_LEDS * 6);
}
void setSeven(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 2);
  pixels.fill(RED, offset + SEGMENT_LEDS * 5, SEGMENT_LEDS);
}
void setEight(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 7);
}
void setNine(int offset) {
  pixels.fill(RED, offset, SEGMENT_LEDS * 6);
}

void clearDigit(int offset) {
  pixels.fill(0, offset, DIGIT_OFFSET);
  pixels.show();
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
    pixels.fill(RED, DIGIT_OFFSET * 2, 2);
  } else {
    pixels.fill(0, DIGIT_OFFSET * 2, 2);
  }
}
