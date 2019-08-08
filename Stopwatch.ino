/** 
 *  Board circuit: https://www.arduino.cc/en/Tutorial/HelloWorld
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int ledPin = 13;                    // LED connected to digital pin 13
int buttonPin = 8;                  // button on pin 8

int lap = 0;                        // Store current lap
int value = LOW;                    // previous value of the LED
int buttonState;                    // variable to store button state
int lastButtonState;                // variable to store last button state
int blinking;                       // condition for blinking - timer is timing
int frameRate = 100;                // the frame rate (frames per second) at which the stopwatch runs - Change to suit
long interval = (1000/frameRate);   // blink interval
long previousMillis = 0;            // variable to store last time LED was updated
long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch
int fractional;                     // variable used to store fractional part of Frames
int fractionalSecs;                 // variable used to store fractional part of Seconds
int fractionalMins;                 // variable used to store fractional part of Minutes
int elapsedFrames;                  // elapsed frames for stop watch
int elapsedSeconds;                 // elapsed seconds for stop watch
int elapsedMinutes;                 // elapsed Minutes for stop watch
char buf[10];                       // string buffer for itoa function
String lapString = "Lap ";           // Initialize lap string for concatenation

int bpmInterval = 10 * 1000;            // How long to measure BPM (in millis)
unsigned long startPrintBpm = 0;              // Measure BPM for bpmInterval

void lcdSetup()
{
  lcd.begin(16, 2);                // intialise the LCD.
  pinMode(ledPin, OUTPUT);         // sets the digital pin as output
  pinMode(buttonPin, INPUT);       // not really necessary, pins default to INPUT anyway
  digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
  
  lcd.setCursor(0,0);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("to start");

}

void printTime() {
     // Routine to report elapsed time            
     elapsedTime =   millis() - startTime;                // store elapsed time
     elapsedMinutes = (elapsedTime / 60000L);
     elapsedSeconds = (elapsedTime / 1000L);              // divide by 1000 to convert to seconds - then cast to an int to print
     elapsedFrames = (elapsedTime / interval);            // divide by 100 to convert to 1/100 of a second - then cast to an int to print
     fractional = (int)(elapsedFrames % frameRate);       // use modulo operator to get fractional part of 100 Seconds
     fractionalSecs = (int)(elapsedSeconds % 60L);        // use modulo operator to get fractional part of 60 Seconds
     fractionalMins = (int)(elapsedMinutes % 60L);        // use modulo operator to get fractional part of 60 Minutes
     lcd.clear();                                         // clear the LDC

     lcd.print(lapString + lap);   
     lcd.setCursor(0, 1);
     
     // Minutes
     if (fractionalMins < 10)                           // pad in leading zeros
     {                            
        lcd.print("0");                                 // add a zero
     }

    lcd.print(itoa(fractionalMins, buf, 10));       // convert the int to a string and print a fractional part of 60 Minutes to the LCD
    lcd.print(":");                                 //print a colan. 

    // Seconds
    if (fractionalSecs < 10){                            // pad in leading zeros
      lcd.print("0");                                 // add a zero
    }

    lcd.print(itoa(fractionalSecs, buf, 10));          // convert the int to a string and print a fractional part of 60 Seconds to the LCD
    lcd.print(":");                                    //print a colan. 

    // Milli
    if (fractional < 10){                                // pad in leading zeros 
      lcd.print("0");                                 // add a zero
    }     

    lcd.print(itoa(fractional, buf, 10));              // convert the int to a string and print a fractional part of 25 Frames to the LCD
}

void lcdLoop()
{

  digitalWrite(ledPin, LOW);            // Initiate LED and Step Pin States

  buttonState = digitalRead(buttonPin); // Check for button press, read the button state and store

   // check for a high to low transition if true then found a new button press while clock is not running - start the clock    
   if (buttonState == LOW && lastButtonState == HIGH  &&  blinking == false && isStopTrainning == false)
   {

      if (lap && lap % 3 == 0 && isPrintBpm == false) {
        startPrintBpm = millis();
        isStopTrainning = true;
        isPrintBpm = true;
        lcd.clear();
      }
      else 
      {
        lap += 1;
        isPrintBpm = false;
        startTime = millis();                               // store the start time
        blinking = true;                                  // turn on blinking while timing
        delay(10);                                         // short delay to debounce switch
      }
      
   } 
   // check for a high to low transition if true then found a new button press while clock is running - stop the clock and report
   else if (buttonState == LOW && lastButtonState == HIGH && blinking == true)
   {
     blinking = false;                                    // turn off blinking, all done timing

     printTime();
  }
  
  
      lastButtonState = buttonState;                  // store buttonState in lastButtonState, to compare next time
  

// run commands at the specified time interval
// blink routine - blink the LED while timing
// check to see if it's time to blink the LED; that is, the difference
// between the current time and last time we blinked the LED is larger than
// the interval at which we want to blink the LED.

 if ( (millis() - previousMillis > interval) ) {

    if (blinking == true){
       previousMillis = millis();                    // remember the last time we blinked the LED

       digitalWrite(ledPin, HIGH);                   // Pulse the LED for Visual Feedback

       printTime();
       
 }

    else{
          digitalWrite(ledPin, LOW);                 // turn off LED when not blinking 
          }
 }

}

void bpmToLcd(int bpm) {
  if (isPrintBpm) {
    lcd.setCursor(0,0);
    lcd.print("BPM");
    
    if (millis() - startPrintBpm > bpmInterval) {
      lcd.setCursor(4,0);
      lcd.print("- Click button");
      isStopTrainning = false;
    }

    if (bpm < 130) {    
      lcd.setCursor(0,1);
      lcd.print("-  ");
      lcd.setCursor(0,1);
      lcd.print(bpm);
    }
  }
}
