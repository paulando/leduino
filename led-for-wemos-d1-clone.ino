/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "PauLEDo Stick #3"
#define APPSK  "ayylmao123"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

int mynumber = 1;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     D8

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  72                                        

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.sendHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");

  int r = 0;
  int g = 0;
  int b = 0;
  int w = 0;
  int r2 = 0;
  int g2 = 0;
  int b2 = 0;
  int w2 = 0;
  int splitted = false;
  int pulse= 0;

  if (server.arg("r")!= 0){ r = server.arg("r").toInt(); }
  if (server.arg("g")!= 0){ g = server.arg("g").toInt(); }
  if (server.arg("b")!= 0){ b = server.arg("b").toInt(); }
  if (server.arg("w")!= 0){ w = server.arg("w").toInt(); }
  
  if (server.arg("r2")!= 0){ r2 = server.arg("r2").toInt(); }
  if (server.arg("g2")!= 0){ g2 = server.arg("g2").toInt(); }
  if (server.arg("b2")!= 0){ b2 = server.arg("b2").toInt(); }
  if (server.arg("w2")!= 0){ w2 = server.arg("w2").toInt(); }
  
  if (server.arg("splitted").toInt() == 1){ splitted = true; }
  if (server.arg("pulse") == "1"){ pulse = 1; }
  
  if (server.arg("r2")!= 0) {
    colorWipe1(strip.Color(r, g, b), 50);
    colorWipe2(strip.Color(r2, g2, b2), 50);
  }
  else {
    colorWipe(strip.Color(r, g, b), 50); 
  }

  if (pulse == 1) {
    pulseWhite(5);
  }
  
  if (w == 1) {
//    colorWipe(strip.Color(0, 0, 0, 255), 50);
    http_white();
  }

  //server.send(200, "application/json", '{"status": "1"}');
  server.send(200, "text/html", "<h1>Choose color:</h1><a href='/red'>Red</a><br/><a href='/green'>Green</a><br/><a href='/blue'>Blue</a><br/><a href='/rainbow'>Rainbow</a><div></div>");
}

void http_white() {
  String html = "<h1>Color: White</h1>";

  colorWipe(strip.Color(0, 0, 0, 255), 50);
  
  server.send(200, "text/html", html);
}

void http_red() {
  String html = "<h1>Color: Red</h1>";

  colorWipe(strip.Color(255,   0,   0)     , 50);
  
  server.send(200, "text/html", html);
}

void http_green() {
  String html = "<h1>Color: Green</h1>";

  colorWipe(strip.Color(0,   255,   0)     , 50);
  
  server.send(200, "text/html", html);
}

void http_blue() {
  String html = "<h1>Color: Blue</h1>";

  colorWipe(strip.Color(0,   0,   255)     , 50);
  
  server.send(200, "text/html", html);
}

void http_rainbow() {
  String html = "<h1>Color: Rainbow</h1>";

  whiteOverRainbow(75, 5);
  
  server.send(200, "text/html", html);
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void colorWipe1(uint32_t color, int wait) {
  for(int i=0; i<(strip.numPixels()/2); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
void colorWipe2(uint32_t color, int wait) {
  for(int i=(strip.numPixels()/2); i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void setup() {

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/white", http_white);
  server.on("/red", http_red);
  server.on("/green", http_green);
  server.on("/blue", http_blue);
  server.on("/rainbow", http_rainbow);
  server.begin();
  Serial.println("HTTP server started");

  
}

void loop() {

//  colorWipe(strip.Color(  0, 255,   0)     , 50);
  
  server.handleClient();
}