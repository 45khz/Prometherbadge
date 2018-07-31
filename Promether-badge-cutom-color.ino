#include <Adafruit_NeoPixel.h>
#define PIN 6 // pixel pin
#define NUM_LEDS 24 // amount of pixelz (24)
#define buttonPin 2  // where is the button?

// *************************************************************
// Pick your favoritve hex color for example #00ff00 turns into the following default below
#define favorite 0x00, 0xff, 0x00

// *************************************************************

int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800); // lets make it understand what we are working with

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  strip.begin();
  strip.show(); // lets start teh show
}

void loop() { 
  strip.setBrightness(10); // brightness level default == 10

  buttonState = digitalRead(buttonPin); // read the pushbutton input pin:
   // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {   // if the state has changed, increment the counter
    if (buttonState == HIGH) {   // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
    } else {  // if the current state is LOW then the button went from on to off:
    }
    delay(50);  // Delay a little bit to avoid bouncing
  }
  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop


if(buttonPushCounter < 10) // check if the counter is within a range of 0 - 9, and anything over that, it will reset count back to 0.
      Serial.print(" all good ");
   else
      buttonPushCounter = 0;
 
// ********* LIGHTSHOWS STARTS HERE *********
  if    (buttonPushCounter == 0)
{ 
  rainbowCycle(10);
}
else if(buttonPushCounter == 1)  // Promether colors
{ 
  FadeInOut(0x66, 0x00, 0x66);
  FadeInOut(0xff, 0x30, 0x00);
  FadeInOut(0x00, 0x88, 0xcc);
}
else if(buttonPushCounter == 2)
{
  colorWipe(favorite, 50);
  colorWipe(0x00,0x00,0x00, 50);  
}
else if(buttonPushCounter == 3)
{
  Sparkle(favorite, random(50,500));
}
else if(buttonPushCounter == 4)
{
  Twinkle(favorite, random(10,24), random(50,250), false);
}
else if(buttonPushCounter == 5)
{
  meteorRain(favorite, 4, 85, true, 50);
}
else if(buttonPushCounter == 6)
{
  SnowSparkle(0xff, 0x30, 0x00, 30, 50);
}
else if(buttonPushCounter == 7)
{
  SnowSparkle(0x66, 0x00, 0x66, 30, 50); 
}
else if(buttonPushCounter == 8)
{
  SnowSparkle(favorite, 30, 50);
}
else if(buttonPushCounter == 9)
{
  TwinkleRandom(24, 100, false);
}   
     
  }
// ********* LIGHTSHOWS ENDS HERE *********

// ********* EFFECTS STARTS HERE *********

// ********* EFFECTS #0 *********
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

// ********* EFFECTS #1 *********
void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

// ********* EFFECTS #2 *********
void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
     delay(SpeedDelay);
}

// ********* EFFECTS #3 *********
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

// ********* EFFECTS #4 *********
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

// ********* EFFECTS #5 *********
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red,green,blue);
  
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
}

// ********* EFFECTS #6 *********
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

// ********* EFFECTS #7 *********
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}


// ********* EFFECTS ENDS HERE *********

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
