#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip
// Not compatible with Trinket/Gemma due to limited RAM

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 64;

// Chose 2 pins for output; can be any valid output pins:
/*int dataPin  = 2;
int clockPin = 3;
*/
// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

#include <Wire.h>

boolean data;
unsigned long start;

void setup() {
  Serial.begin(9600);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);

  // Start up the LED strip
  strip.begin();
  // Update the strip, to start they are all 'off'
  strip.show();
}

int recievedNum = 0;
int prevRecieved = recievedNum;
boolean recieved = false;


void loop() {
  int time = 20;
  
  functionArray(recievedNum, time);
  //Serial.print("Recived: ");
  //Serial.println(recieved);
  recieved = false;
}

void receiveEvent(int howMany)
{
  while (Wire.available() > 0)
  {
    data = true;
    byte n = (byte)Wire.read();
    recievedNum = (int) n;
  }
  //Serial.println(recievedNum);
  if(recievedNum != prevRecieved) recieved = true;
  prevRecieved = recievedNum;
}
/*
*functionArray
*  Parameters:
*    choice is the choice of the function array
*    time is the int value passed to some functions for delays
*
*  Information:
*    This is an array of preset functions for control
*    by a master device which will send an integer 
*    through I2C communication. That integer will be
*    passed to this function to choose a prespecified
*    function or group of functions
*    
*   Usage/Adding/Deleting:
*      To add a function or group of functions, 
*      simply add another else if with the choice int
*      you want to correspond to that function/function
*      group
*/
void functionArray(int choice, int time){
  if (choice == 0)
  {
    singleColor(strip.Color(127,0,0)); 
  }
  else if (choice == 1)
  {
    explode(time);
  }
  else if (choice == 2)
  {
    dualBounce(strip.Color(0,0,100), strip.Color(80,20,0), time);
    dualBounce(strip.Color(80,20,0), strip.Color(0,0,100), time);
  }
  else if (choice == 3)
  {
    randPixelGen();
  }
  else if (choice == 4)
  {
    wave(0, 0, 100, 0, true);
    wave(80, 20, 0, 0, false);
  }
  else if (choice == 5)
  {
    colorChase(strip.Color(0,20,80), time);
  }
  else if(choice == 6)
  {
    jumpAround(strip.Color(127,0,0), 1); 
  }
  else if(choice == 7){
    uint32_t colors[] = {strip.Color(80,20,0), strip.Color(0,0,100) };
    multicolor(colors, 2, 4, true, 12, true);
  }
  else if(choice == 8){
    bounce(strip.Color(80,20,0), time, true);
  }
  else if(choice == 9){
    bounce(strip.Color(0,0,100), time, false);
  }
  else if(choice >= 10 && choice <= 110){
   speedometer(choice-10); 
  }
}

void singleColor(uint32_t color){
  // Start by turning all pixels off:
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  // Set all pixels to 'color'
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, color);
  // Show until a new function is called by I2C
  while(!recieved){ strip.show();}
}
/*
*returnColor
*  Parameters:
*    c is a color to be decoded
*  Information:
*    This is for help debugging using the Serial
*    Monitor
*  
*  DO NOT USE THIS WHEN IT'S ON THE ROBOT
*  FOR DEBUGGING ONLY
*/
String returnColor(uint32_t c){
  if(c == strip.Color(100,0,0)) return "Red";
  else if(c == strip.Color(0,100,0)) return "Green";
  else if(c == strip.Color(0,0,100))return "Blue";
  else if(c == strip.Color(80,20,0)) return "Orange";
  else if(c == strip.Color(0,0,0)) return "Blank ";
  else return "Cnf";


}

/*
* --NOT TESTED--
*Speedometer
*  Parameters:
*    percent percent of max speed
*/
void speedometer(int percent){
  int pixels;
  if(percent < 0 || percent > 100) return;
  else pixels = (int) (strip.numPixels() * (percent/100.0)) / 2;
  if(pixels == 0) pixels++;
  for(int i = 0; i < pixels; i++){
    if(i < strip.numPixels()/4){
      strip.setPixelColor(strip.numPixels()/2 - i,strip.Color(0,100,0));
      strip.setPixelColor(strip.numPixels()/2 + 1 + i,strip.Color(0,100,0));
    }
    else if(i < 7*strip.numPixels()/16){
      strip.setPixelColor(strip.numPixels()/2 - i,strip.Color(100,20,0));
      strip.setPixelColor(strip.numPixels()/2 + 1 + i,strip.Color(100,20,0));
    }
    else{
      strip.setPixelColor(strip.numPixels()/2 - i,strip.Color(100,0,0));
      strip.setPixelColor(strip.numPixels()/2 + 1 + i,strip.Color(100,0,0));
    }
  }
  for(int i = pixels; i < strip.numPixels(); i++){
      strip.setPixelColor(strip.numPixels()/2 - i,0);
      strip.setPixelColor(strip.numPixels()/2 + 1 + i, 0);
  }
  strip.show();
 while(!recieved) strip.show(); 
}

/*
*Multicolor 
 *  Parameters: 
 *   colors[] array of colors to be shown
 *   sizeOfArray is the size of the color array
 *   spacing is the width of each section of a color on the strip
 *   spacers, when true, will put a blank segment of length spacing
 *    between each color
 *   time is the millisecond refresh rate
 * forward is whether the algorithm runs forward or in reverse
 */

void multicolor(uint32_t colors[], int sizeOfArray, int spacing, boolean spacers, int time, boolean forward){

  int section = sizeOfArray*spacing;
  if(spacers) section*=2;
  uint32_t animation[strip.numPixels()];


  int i = 0;
  int prevI = i;
  int selection = 0;
  uint32_t color = colors[selection];
  while(i < strip.numPixels()){
    prevI = i;
    while(i < prevI + spacing && i < strip.numPixels()){
      animation[i] = color;
      i++;
    }
    if(spacers && color != strip.Color(0,0,0)){
      {
        color = strip.Color(0,0,0); 
      }

    }
    else{
      if(selection < sizeOfArray -1){
        selection++; 
      }
      else{
        selection = 0;
      }
      color = colors[selection];
    } 

  }
  while(!recieved){
    if(forward){
      for(int i = 0; i < section; i++){

        if(i == 0){ 
          for(int j = 0; j < strip.numPixels(); j++){
            strip.setPixelColor(j, animation[j]);
          }
        }
        else{
          for(int j = 0; j < strip.numPixels(); j++){
            if(j+i >= strip.numPixels()){
              strip.setPixelColor(j, animation[i + j - strip.numPixels()]);
            }
            else {
              strip.setPixelColor(j, animation[j + i]);
            }
          } 
        }
        strip.show();
        if(recieved) return;
        delay(time);
      }  
    }
    else {
      for(int i = 0; i < section; i++){

        if(i == 0){ 
          for(int j = 0; j < strip.numPixels(); j++){
            strip.setPixelColor(strip.numPixels() - 1 - j, animation[j]);
          }
        }
        else{
          for(int j = 0; j < strip.numPixels(); j++){
            if(j+i >= strip.numPixels()){
              strip.setPixelColor(strip.numPixels() - 1 - j, animation[i + j - strip.numPixels()]);
            }
            else {
              strip.setPixelColor(strip.numPixels() - 1 - j, animation[j + i]);
            }
          } 
        }
        strip.show();
        if(recieved) return;
        delay(time);
      }  
    }
  }



} 

/*
 *MirroredMulticolor 
 *  Parameters: 
 *   colors[] array of colors to be shown
 *   sizeOfArray is the size of the color array
 *   spacing is the width of each section of a color on the strip
 *   spacers, when true, will put a blank segment of length spacing
 *    between each color
 *   time is the millisecond refresh rate
 * forward is whether the algorithm runs forward orin reverse
 */

void mirroredMulticolor(uint32_t colors[], int sizeOfArray, int spacing, boolean spacers, int time, boolean forward){

  int section = sizeOfArray*spacing;
  if(spacers) section*=2;
  uint32_t animation[strip.numPixels()];


  int i = 0;
  int prevI = i;
  int selection = 0;
  uint32_t color = colors[selection];
  while(i < strip.numPixels()){
    prevI = i;
    while(i < prevI + spacing && i < strip.numPixels()){
      animation[i] = color;
      i++;
    }
    if(spacers && color != strip.Color(0,0,0)){
      {
        color = strip.Color(0,0,0); 
      }

    }
    else{
      if(selection < sizeOfArray -1){
        selection++; 
      }
      else{
        selection = 0;
      }
      color = colors[selection];
    } 

  }
  while(!recieved){
    if(forward){
      for(int i = section - 1; i >= 0; i--){

        if(i == 0){ 
          for(int j = 0; j < strip.numPixels()/2; j++){
            strip.setPixelColor(j, animation[j]);
            strip.setPixelColor(strip.numPixels()-1-j, animation[j]);
          }
        }
        else{
          for(int j = 0; j < strip.numPixels()/2; j++){
            if(j+i >= strip.numPixels()){
              strip.setPixelColor(j, animation[i + j - strip.numPixels()]);
              strip.setPixelColor(strip.numPixels()-1-j, animation[i + j - strip.numPixels()]);
            }
            else {
              strip.setPixelColor(j, animation[j + i]);
              strip.setPixelColor(strip.numPixels()-1-j, animation[j + i]);
            }
          } 
        }
        strip.show();
        if(recieved) return;
        delay(time);
      }  
    }
    else {
      for(int i = 0; i < section; i++){

        if(i == 0){ 
          for(int j = 0; j < strip.numPixels()/2; j++){
            strip.setPixelColor(j, animation[j]);
            strip.setPixelColor(strip.numPixels()-1-j, animation[j]);
          }
        }
        else{
          for(int j = 0; j < strip.numPixels()/2; j++){
            if(j+i >= strip.numPixels()){
              strip.setPixelColor(j, animation[i + j - strip.numPixels()]);
              strip.setPixelColor(strip.numPixels()-1-j, animation[i + j - strip.numPixels()]);
            }
            else {
              strip.setPixelColor(j, animation[j + i]);
              strip.setPixelColor(strip.numPixels()-1-j, animation[j + i]);
            }
          } 
        }
        strip.show();
        if(recieved) return;
        delay(time);
      }  
    }
  }



} 


// Chase one dot down the full strip.  Good for testing purposes.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    if(recieved) return;
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}


/*
*wave
*  Parameters:
*    red/green/blue are the max red/green/blue values (0-127) which will
*      be on the front end of the wave
*    wait is the delay time for the function
*    forward determines the direction of the wave, true being forward
*      false being backward
*
*  Information:
*    Runs a line of pixels fading from the front to the back of the line,
*    which cascades from one side to the other
 */
void wave(int red, int green, int blue, int wait, boolean forward){  
  uint32_t c = strip.Color(0,0,0);
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  int redIncrement = (int) (red + 1) / (strip.numPixels() / 2);
  int greenIncrement = (int) (green + 1) /(strip.numPixels() / 2);
  int blueIncrement = (int) (blue + 1) / (strip.numPixels() / 2);

  int increment = (int) 128/(strip.numPixels()/2);
  boolean clearing = false;
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  for(int i = 0; i < strip.numPixels() + strip.numPixels()/2; i++){
    int r = red;
    int g = green;
    int b = blue;
    if(i >= strip.numPixels()/2)clearing = true;
    for(int j = i; j >= 0; j--){
      r-=redIncrement;
      g-=greenIncrement;
      b-=blueIncrement;

      c = strip.Color(r,g,b);

      if(forward){
        if(j < strip.numPixels()) strip.setPixelColor(j,c);
        if((j < strip.numPixels() && clearing && i+1 >= strip.numPixels()/2 + j)) strip.setPixelColor(j,0);
      }
      else{
        if(j < strip.numPixels()) strip.setPixelColor(strip.numPixels() - j - 1,c);
        if((j < strip.numPixels() && clearing && i+1 >= strip.numPixels()/2 + j)) strip.setPixelColor(strip.numPixels() - j - 1,0);
      }
    }
    strip.show();
    if(recieved) return;
    delay(wait);
  }
}

/*
*breathe
*  Parameters:
*    red/green/blue are the max values (0-127) of the respective colors in the function
*    time is the delay 
*
*  Information:
*    Makes the whole strip "breathe" with a uniform color which fades
*    to fully off then brightens back to the max value
*/
void breathe(int red, int green, int blue, int time){

  int maxChange = max(max(red,green),blue);
  int redIncrement = (int) ((red + 1) / maxChange);
  int greenIncrement = (int) ((green + 1) / maxChange);
  int blueIncrement = (int) ((blue + 1) / maxChange);

  boolean up = false;
  int r = red;
  int g = green;
  int b = blue;
  int completions = 0;
  while(!recieved){
    if(up){
      if((r < red && redIncrement > 0)|| (g < green && greenIncrement > 0)|| (b < blue && blueIncrement > 0)){
        if (r + redIncrement <= red) r+=redIncrement;
        if (g + greenIncrement <= green) g+=greenIncrement;
        if (b + blueIncrement <= blue) b+=blueIncrement;
      }
      else{
        up = false;
        r = red;
        g = green;
        b = blue;
      }
    }
    else{
      if((r > redIncrement + 1 && redIncrement > 0)|| (g > greenIncrement + 1 && greenIncrement > 0)|| (b > blueIncrement + 1 && blueIncrement > 0)){
        if (r - redIncrement >= 0) r-=redIncrement;
        if (g - greenIncrement >= 0) g-=greenIncrement;
        if (b - blueIncrement >= 0) b-=blueIncrement;
      }
      else{
        up = true;
        int r = 0;
        int g = 0;
        int b = 0;
      }
    }
    for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(r, g, b));
    strip.show();
    if(recieved) return;
    delay(time);
  }
}

/*
*jumpAround
*  Parameters:
*    color is the color
*    wait is the delay
*
*  Information:
*    This is a dot that randomly goes right or left
*/

void jumpAround(uint32_t color, int wait){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  int location = strip.numPixels()/2;
  while(!recieved){
    int getDirection = random(2); // 1 is right 0 is left
    if(getDirection == 0 && location < strip.numPixels()){
      location++;
      strip.setPixelColor(location,color);
      strip.setPixelColor(location-1,0);
    }
    else if(getDirection == 1 && location > 0){
      location--;
      strip.setPixelColor(location,color);
      strip.setPixelColor(location+1,0);
    }
    strip.show();
    if(recieved) return;
    delay(wait);
  }
}

/*
*randPixelGen
* Information:
*  This just chooses a random RGB value for each pixel
*
*/

void randPixelGen(){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  while(!recieved){
    for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(random(1,127), random(1,127), random(1,127)));
    strip.show();
  }
}
/*
*bounce
*  Parameters:
*    c is the color
*    wait is the delay time
*    boom is whether to use rainbow explosion or not
*
*  Information:
*    Two pixels come towards the center and when they meet in the
*    middle they either raindbow explode or reverse direction and
*    move from the inside out
*/
void bounce(uint32_t c, int wait, boolean boom){ 
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  for (int i = 0; i < strip.numPixels()/2; i++){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, 0); // Erase pixel, but don't refresh!
    if(recieved) return;
    delay(wait);
  }
  if(boom) explode(wait);
  else{
    for (int i = strip.numPixels()/2; i < strip.numPixels(); i++){
      strip.setPixelColor(i, c); // Set new pixel 'on'
      strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
      strip.show();              // Refresh LED states
      strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
      strip.setPixelColor(strip.numPixels()-1-i, 0); // Erase pixel, but don't refresh!
      if(recieved) return;
      delay(wait);
    }
  }
}

void explode(int wait){
  uint32_t c = strip.Color(0,0,0);
  int interval = (int) 128/(strip.numPixels()/6)-1; //1/2 the strip, 3 colors, 128 values per color: 128values * 1/2 strip / 3 colors = 128/(strip/6) the "-1" accounts for rounding lost in integer counts
  
  for (int i = strip.numPixels()/2; i < strip.numPixels(); i++){
      strip.setPixelColor(i, strip.Color(100,100,100)); // Set new pixel 'on'
      strip.setPixelColor(strip.numPixels()-1-i, strip.Color(70,70,70)); // Set new pixel 'on'
      strip.show();      // Refresh LED states
      if(recieved) return;
      delay(2);
    }
  //for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  for(int i = 0; i <= strip.numPixels()/2; i++){
    int colorFading = 0; // color fading from: 0 is red, 1 is blue, 2 is green
    int red = 127;
    int green = 0;
    int blue = 0;
    for(int j = strip.numPixels()/2 - i; j < strip.numPixels()/2; j++){
      if(colorFading == 0){
        red-=interval;
        blue+=interval;
        if(red < interval) colorFading++;
      }
      else if(colorFading == 1){
        blue-=interval;
        green+=interval;
        if(blue < interval) colorFading++;
      }
      else{
        green-=interval;
        if(green < interval) colorFading = 0;
      }
      c = strip.Color(red,green,blue);
      strip.setPixelColor(j,c);
      strip.setPixelColor(strip.numPixels()-j-1,c);
    }
    strip.show();
    if(recieved) return;
    delay(wait);
  }
  c = strip.Color(127,10,10);
  for (int i = strip.numPixels()/2; i >= 0; i--){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i+1, 0); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-2-i, 0); // Erase pixel, but don't refresh!
    if(recieved) return;
    delay(wait);
  }
}
/*
*dualBounce
*  parameters:
*    c is the first color
*    c2 is the second color
*    wait is the delay time
*
*  Information:
*    Same as bounce, but with a colored backround
*    no rainbow explosion
*/
void dualBounce(uint32_t c, uint32_t c2, int wait){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, c2);
  for (int i = 0; i < strip.numPixels()/2; i++){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, c2); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, c2); // Erase pixel, but don't refresh!
    if(recieved) return;
    delay(wait);
  }
  for (int i = strip.numPixels()/2; i < strip.numPixels(); i++){
    strip.setPixelColor(i, c2); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c2); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, c); // Erase pixel, but don't refresh!
    if(recieved) return;
    delay(wait);
  }
}
