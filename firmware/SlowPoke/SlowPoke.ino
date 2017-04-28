//read this https://learn.adafruit.com/multi-tasking-the-arduino-part-3/put-it-all-together-dot-dot-dot


// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library




#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      28

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 200; // delay for half a second

byte forwardSequence[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13};
byte forwardSequenceLarge[] = {12,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13};
byte forwardSequenceLarge2[] = {16,15,14,13,12,27,26,25,24,23,22,21,20,19,18,17};
byte reverseSequence[] = {13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,12,11,10,9,8,7,6,5,4,3,2,1,0};
byte greens[] = {2, 4, 16, 32, 64, 128, 180, 128, 64, 32, 16, 4, 2};
int delays[] = {25, 25, 25, 25, 25, 25, 50, 50, 50, 100, 200, 300, 500};

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(128);
  
}


//void test(int delayval, byte red, byte green, byte blue) {
//  for(byte i=0;i<NUMPIXELS;i++){
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(i, pixels.Color(red,green,blue)); // Moderately bright green color.
//
//    pixels.show(); // This sends the updated pixel color to the hardware.
//
//    delay(delayval); // Delay for a period of time (in milliseconds).
//  }
//}

//void circleAround(byte sequence[], int delayval, byte offset, byte skip, byte red, byte green, byte blue) {
//  skip = skip + 1;
//  
//  for(byte i=offset;i<NUMPIXELS;i++){
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    if (((i+1+offset) % skip) == 0) {
//      pixels.setPixelColor(sequence[i], pixels.Color(red,green,blue)); // Moderately bright green color.
//    }
//
//    if (delayval > 0) {
//      pixels.show(); // This sends the updated pixel color to the hardware.
//      delay(delayval); // Delay for a period of time (in milliseconds).
//    }
//  }
//  if (delayval <= 0) {
//    pixels.show();
//  }
//}

void pulse(byte sequence[], byte delta, int delayval, boolean updown, boolean pulsered, boolean pulsegreen, boolean pulseblue, byte red, byte green, byte blue) {
  byte diff;
  
  if (delta > 0) {
    byte largest = max(red, green);
    largest = max(blue, largest);
    diff = 255-largest;
  } else {
    byte smallest = min(red, green);
    smallest = min(blue, smallest);
    diff = smallest;   
  }
  
  diff = round((diff*1.0) / abs(delta)); // handle deltas larger than 1;
  
  if (updown) {
    diff = diff * 2;
  }
  
  byte tred = red;
  byte tgreen = green;
  byte tblue = blue;
  
  for (byte j=0;j<=diff;j++) {
    for(byte i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(sequence[i], pixels.Color(tred,tgreen,tblue)); // Moderately bright green color.
    }
  
    pixels.show();
  
    boolean changed = false;
    if (delta>0){
      if ((tred+delta <=255) and (pulsered)) {
        tred = tred + delta;
        changed = true;
      }
      if ((tgreen+delta<=255) and (pulsegreen)) {
        tgreen = tgreen + delta;
        changed = true;
      }
      if ((tblue+delta<=255) and (pulseblue)) {
        tblue = tblue + delta;
        changed = true;
      }
    } else {
      if ((tred+delta>=0) and (pulsered)) {
        tred = tred + delta;
        changed = true;
      }
      if ((tgreen+delta>=0) and (pulsegreen)) {
        tgreen = tgreen + delta;
        changed = true;
      }
      if ((tblue+delta>=0) and (pulseblue)) {
        tblue = tblue + delta;
        changed = true;
      }
    }
    if (!changed) {
      delta = delta * -1;
    }
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

//void colorCycle(byte sequence[]) { 
//  for (byte tred = 0; tred < 255; tred++) {
//    for (byte tgreen = 0; tgreen < 255; tgreen++) {
//      for (byte tblue = 0; tblue < 255; tblue++) {
//        uint32_t tcolor = pixels.Color(tred,tgreen,tblue);
//        for(byte i=0;i<NUMPIXELS;i++){
//          // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//          pixels.setPixelColor(sequence[i], tcolor); 
//        }
//        pixels.show();
////        delay(10);
////        pixels.clear();
//      }
//    }
//  }
//}

void slowRotate(byte sequence[]) {
  byte winoffset = 0;
  for (byte j=0;j<1;j++) {  // repetions of the sequence
    for (byte i=0;i<8;i++) { // Turtle goes around
      winoffset = 0;
      for (byte k=0;k<sizeof(greens);k++) { // Turtle goes around
        pixels.setPixelColor(sequence[2*i+winoffset], pixels.Color(0, greens[k], 0));   
        pixels.setPixelColor(sequence[2*i+1+winoffset], pixels.Color(0, greens[k], 0));   
        pixels.show();
        delay(200);
      }
      pixels.setPixelColor(sequence[2*i+winoffset], pixels.Color(0, 0, 0));   
      pixels.setPixelColor(sequence[2*i+1+winoffset], pixels.Color(0, 0, 0));   
    }
  }
}

void shell(byte sequence1[], byte sequence2[]) {
  byte numdelays = 13;
  for (byte k=0; k < numdelays; k++) { // iterations
    for (byte i=0; i<2; i++) { // half the lights at a time
      for (byte j=0; j<8; j++) {  // each light in the half
        pixels.setPixelColor(sequence1[(8*i)+j], pixels.Color(0, 64, 0));   
      }
      pixels.show();
      delay(delays[k]);
      for (byte j=0; j<8; j++) {
        pixels.setPixelColor(sequence1[(8*i)+j], pixels.Color(0, 0, 0));   
      }
      pixels.show();
      delay(delays[k]);    
    }
  }
  
  // shift 45 degrees and repeat
  for (byte k=0; k < numdelays; k++) { // iterations
    for (byte i=0; i<2; i++) {
      for (byte j=0; j<8; j++) {
        pixels.setPixelColor(sequence2[(8*i)+j], pixels.Color(0, 64, 0));   
      }
      pixels.show();
      delay(delays[k]);
      for (byte j=0; j<8; j++) {
        pixels.setPixelColor(sequence2[(8*i)+j], pixels.Color(0, 0, 0));   
      }
      pixels.show();
      delay(delays[k]);    
    }
  }
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  
  // Start Tree
//  pulse(forwardSequence, 5, 100, false, true, true, true, 60, 7,0 );  
//  colorCycle(forwardSequence);
  slowRotate(forwardSequenceLarge);
  shell(forwardSequenceLarge, forwardSequenceLarge2);
  pixels.clear();
  pixels.show();
//  delay(50);
//  pulse(forwardSequence, 5, 10, true, true, true, false, 20, 15, 0 );  
//  pixels.clear();
//  pixels.show();
//  delay(50);
//  pulse(forwardSequence, 5, 10, false, false, true, false, 0, 10, 0 );  
//  delay(1000);
//  pixels.clear();
//  pixels.show();
//
//  race(reverseSequence);
  
//  pixels.clear();
//  pixels.show();
//  // Final flag
//  for (byte i=0; i<10;i++) {
//    circleAround(forwardSequence, 0, 0, 1, 9, 9, 9);
//    delay(200);
//    pixels.clear();
//    pixels.show();
//    circleAround(forwardSequence, 0, 1, 1, 9, 9, 9);
//    delay(200);
//    pixels.clear();
//    pixels.show();
//  }
//  delay(1000);
  pixels.clear();
  pixels.show();
}
