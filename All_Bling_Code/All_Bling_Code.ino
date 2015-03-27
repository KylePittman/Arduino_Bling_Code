#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip
// Not compatible with Trinket/Gemma due to limited RAM

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs =64;


/* Uncomment to use non spi on pins 2 and 3
int dataPin  = 2;
int clockPin = 3;
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
*/

//Comment out to use nonSPI (much slower writing to strip)
//Pins 11 and 13 are data and clock, respectiviely

LPD8806 strip = LPD8806(nLEDs); 


void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  
  //bouncingBall(strip.Color(0,0,100), 100);
  
  
  //jumpAround(strip.Color(100,0,0), 5, 1000);
  randPixelGen(25);
  uint32_t test[] = {
    strip.Color(100,20,0), strip.Color(0,100,0), strip.Color(100,0,0)    };
  for(int i = 0; i < 12; i++)
  mirroredMulticolor(test, 3, 4, false, 25, false);
  for(int i = 0; i < 12; i++)
  mirroredMulticolor(test, 3, 4, false, 25, true);
  
  uint32_t test2[] = {
    strip.Color(100,100,100), strip.Color(100,0,0), strip.Color(0,100,0), strip.Color(0,0,100), strip.Color(80,20,0), strip.Color(50,50,100), strip.Color(20,0,80), strip.Color(100,50,50)  };
  //randPixelGen(50);
  singleColor(strip.Color(80,20,0));
  delay(500);
  multicolor(test2, 8, 4, true, 25, true);
  multicolor(test2, 8, 4, true, 25, false);
  wave(0, 0, 100, 10, true);
  wave(80, 20, 0, 10, false);
  breathe(100, 0, 0, 5);
  colorChase(strip.Color(  0,127,  0), 25); // Green*/
  bounce(strip.Color(80,20,0), 20, true);
  //delay(100);
}

void singleColor(uint32_t color){ //copied
  // Start by turning all pixels off:
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, color);
  strip.show();
}

String returnColor(uint32_t c){
  if(c == strip.Color(100,0,0)) return "Red";
  else if(c == strip.Color(0,100,0)) return "Green";
  else if(c == strip.Color(0,0,100))return "Blue";
  else if(c == strip.Color(80,20,0)) return "Orange";
  else if(c == strip.Color(0,0,0)) return "Blank ";
  else return "Cnf";


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
 * forward is whether the algorithm runs forward orin reverse
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
  for(int k = 0; k < 1; k++){
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
  for(int k = 0; k < 1; k++){
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
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}


/*
*ints red, green, blue are 0-127 values for the brightest of each of those colors
 *wait is the refresh rate of the algorithm
 *if forward is true, the lights will run away from the side with wire
 *if false it will run from the other side towards it
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
    delay(wait);
  }
}

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
  while(completions < 2){
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
        completions++;
      }
    }
    else{
      if((r > redIncrement + 1 && redIncrement > 0)|| (g > greenIncrement + 1 && greenIncrement > 0)|| (b > blueIncrement + 1 && blueIncrement > 0)){
        if (r - redIncrement >= 0) r-=redIncrement;
        if (g - greenIncrement >= 0) g-=greenIncrement;
        if (b - blueIncrement >= 0) b-=blueIncrement;
      }
      else{
        ;
        up = true;
        int r = 0;
        int g = 0;
        int b = 0;
      }
    }
    for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(r, g, b));
    strip.show();
    delay(time);
  }
}

void jumpAround(uint32_t color, int wait, int iterations){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  int location = strip.numPixels()/2;
  for (int k = 0; k < iterations; k++){
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
    delay(wait);
  }
}

void randPixelGen(int iterations){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  for(int k = 0; k < iterations; k++){
    //uint32_t lll = strip.Color(random(0, 127), random(0,127), random(0,127));
    for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(random(1,127), random(1,127), random(1,127)));
    strip.show();
    //delay(random(1, 100));
    /*for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(random(1,30), random(1,30), random(1,90)));
     strip.show();
     delay(random(1,50));*/
  }
}

void bounce(uint32_t c, int wait, boolean boom){ 
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  for (int i = 0; i < strip.numPixels()/2; i++){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, 0); // Erase pixel, but don't refresh!
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
      strip.show();              // Refresh LED states
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
    delay(wait);
  }
  c = strip.Color(127,10,10);
  for (int i = strip.numPixels()/2; i >= 0; i--){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i+1, 0); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-2-i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }
}

void dualBounce(uint32_t c, uint32_t c2, int wait){
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, c2);
  for (int i = 0; i < strip.numPixels()/2; i++){
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, c2); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, c2); // Erase pixel, but don't refresh!
    delay(wait);
  }
  for (int i = strip.numPixels()/2; i < strip.numPixels(); i++){
    strip.setPixelColor(i, c2); // Set new pixel 'on'
    strip.setPixelColor(strip.numPixels()-1-i, c2); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, c); // Erase pixel, but don't refresh!
    strip.setPixelColor(strip.numPixels()-1-i, c); // Erase pixel, but don't refresh!
    delay(wait);
  }
}

void bouncingBall(uint32_t c, int time){
 for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
 int y = 0;
 int lastY = 0;
 int noChangeCount = 0;
 for(float i = .01; !(i > 25.05 && y == 0) ; i+=.01){
   
   y = (int) abs(strip.numPixels()*pow(2.17, -.1*i)*cos(3.14 *i));
   if(noChangeCount < 50){
   strip.setPixelColor(y, c);
   if(i > .01 && lastY != y) strip.setPixelColor(lastY, 0);
   strip.show();
   delay(time);
   }
   if(y == lastY) noChangeCount++;
   else noChangeCount = 0;
   lastY = y;
 }
}


