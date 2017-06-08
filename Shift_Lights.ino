#include <SimpleTimer.h>
#include <SevenSeg.h>

/*
 * Plugin config:
 * 
 * Pot: A0
 * button: 13 & GND
 * Digit: 21
 * Segs: 47
 * Shift light GND: 2
 * shift 1: 23
 * Shift 2:35
 * shift 3: 22
 * Shift 4: 34
 */



// For testing
int i;
unsigned long segPrevMillis = 0;
const long interval = 500;
String msg;

// *************************** PROGRAM CONSTANTS *****************

//RGB use in switches etc...
const int iRed = 1;
const int iGreen = 2;
const int iBlue = 3;

// LED Blink State, use in switches to define blink state and color
const int bRed = 1;
const int bBlue = 2;
const int bGreen = 3;
const int bPitSpeed = 4;

// ARDUINO PIN CONFIG

const int pButton = 13;
const int pBrightness = 0;
const int pDigit = 21;
const int segA = 47;
const int segB = 49;
const int segC = 51;
const int segD = 48;
const int segE = 53;
const int segF = 52;
const int segG = 50;
const int segDP = 46;
const int pLed1Red = 23;
const int pLed1Green = 25;
const int pLed1Blue = 27;
const int pLed2Red = 29;
const int pLed2Green = 31;
const int pLed2Blue = 33;
const int pLed3Red = 35;
const int pLed3Green = 37;
const int pLed3Blue = 39;
const int pLed4Red = 41;
const int pLed4Green = 43;
const int pLed4Blue = 45;
const int pLed5Red = 22;
const int pLed5Green = 24;
const int pLed5Blue = 26;
const int pLed6Red = 28;
const int pLed6Green = 30;
const int pLed6Blue = 32;
const int pLed7Red = 34;
const int pLed7Green = 36;
const int pLed7Blue = 38;
const int pLed8Red = 40;
const int pLed8Green = 42;
const int pLed8Blue = 44;
const int ledGND = 2;

// iRacing SDK Variables
const byte irsdk_pitSpeedLimiter = "0x0010";  //Pit Speed Enabled

// Characters for Nuetral and Reverse
const String neutral = "n";
const String reverse = "r";

// Number of Shift Light patterns
const int totalPatterns = 3;

// ************************* END PROGRAM CONSTANTS **************************
// ************************* ARDUINO OBJECT DEFINITIONS ********************

// 7 Seg Display
SevenSeg Gear(segA, segB, segC, segD, segE, segF, segG);
const int numDigits = 1;
int digitPins[numDigits] = {pDigit};

// Simple timer for Pit speed flash
SimpleTimer tBlink;

// ********************** END ARDUINO OBJECT DEFINITIONS **********************
// *********************** PROGRAM VARIABLES ************************

// Brightness
int val;
int segBrightness;
int ledBrightness;

// Serial Data in
int data;

// Button state
int prevButtonState = 0;
int ButtonState = 0;

// Pattern number
int nPattern = 0;
int cPattern = 0;

// Current led State. Used to set state in ledBlink()
int ledState = LOW;

// ShiftLights converted from Serial data. IR variable: irsdk_ShiftIndicatorPct
int nShiftLight = 0;

// Pit lane speed color
int color = bBlue;

// simple timer ID
int timerID = 0;

byte irsdk_EngineWarnings = "0x0000";                 // Testing only. Pull this variable from Software pit lane speed = 0x0010
float irsdk_ShiftIndicatorPct = 0;                        // Testing only. Pull this variable from Software ** DEPRECATED use DriverCarSLBlinkRPM instead **

// ************************ END PROGRAM VARIABLES ********************
// ******************  FUNCTIONS  *******************************

// Global LED Control
void AllLedsOff() {
  digitalWrite(pLed1Red, LOW);
  digitalWrite(pLed1Green, LOW);
  digitalWrite(pLed1Blue, LOW);
  digitalWrite(pLed2Red, LOW);
  digitalWrite(pLed2Green, LOW);
  digitalWrite(pLed2Blue, LOW);
  digitalWrite(pLed3Red, LOW);
  digitalWrite(pLed3Green, LOW);
  digitalWrite(pLed3Blue, LOW);
  digitalWrite(pLed4Red, LOW);
  digitalWrite(pLed4Green, LOW);
  digitalWrite(pLed4Blue, LOW);
  digitalWrite(pLed5Red, LOW);
  digitalWrite(pLed5Green, LOW);
  digitalWrite(pLed5Blue, LOW);
  digitalWrite(pLed6Red, LOW);
  digitalWrite(pLed6Green, LOW);
  digitalWrite(pLed6Blue, LOW);
  digitalWrite(pLed7Red, LOW);
  digitalWrite(pLed7Green, LOW);
  digitalWrite(pLed7Blue, LOW);
  digitalWrite(pLed8Red, LOW);
  digitalWrite(pLed8Green, LOW);
  digitalWrite(pLed8Blue, LOW);
  cPattern = "off";                     //TESTING ONLY
}

void AllLedsRed(int ledState) {
    digitalWrite(pLed1Red, ledState);
    digitalWrite(pLed2Red, ledState);
    digitalWrite(pLed3Red, ledState);
    digitalWrite(pLed4Red, ledState);
    digitalWrite(pLed5Red, ledState);
    digitalWrite(pLed6Red, ledState);
    digitalWrite(pLed7Red, ledState);
    digitalWrite(pLed8Red, ledState);
    cPattern = "red";                     //TESTING ONLY
}

void AllLedsGreen(int ledState) {
    digitalWrite(pLed1Green, ledState);
    digitalWrite(pLed2Green, ledState);
    digitalWrite(pLed3Green, ledState);
    digitalWrite(pLed4Green, ledState);
    digitalWrite(pLed5Green, ledState);
    digitalWrite(pLed6Green, ledState);
    digitalWrite(pLed7Green, ledState);
    digitalWrite(pLed8Green, ledState);
    cPattern = "green";                     //TESTING ONLY
}

void AllLedsBlue(int ledState) {
    digitalWrite(pLed1Blue, ledState);
    digitalWrite(pLed2Blue, ledState);
    digitalWrite(pLed3Blue, ledState);
    digitalWrite(pLed4Blue, ledState);
    digitalWrite(pLed5Blue, ledState);
    digitalWrite(pLed6Blue, ledState);
    digitalWrite(pLed7Blue, ledState);
    digitalWrite(pLed8Blue, ledState);
    cPattern = "blue";                     //TESTING ONLY
}

// Shift Light Patterns
void Pattern1(int i) {
 // int num = map(i, 0, 100, 0, 9);
 int num = i;
  AllLedsOff();
  switch (num) {
    case 1:
      digitalWrite(pLed1Green, HIGH);
    break;
    case 2:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
    break;
    case 3:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
    break;
    case 4:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
    break;
    case 5:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Red, HIGH);
    break;
    case 6:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
    break;
    case 7:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
      digitalWrite(pLed7Red, HIGH);
    break;
    case 8:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
      digitalWrite(pLed7Red, HIGH);
      digitalWrite(pLed8Red, HIGH);
    break;
    case 9:
      AllLedsOff();
      AllLedsRed(HIGH);
    break;
  }
  cPattern = "1";                     //TESTING ONLY
}

void Pattern2(int i) {
  AllLedsOff();
  //int num = map(i, 0, 100, 0, 5);
  int num = i;
  AllLedsOff();
  switch (num) {
    case 1:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 2:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 3:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 4:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Red, HIGH);      
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 5:
      AllLedsOff();
      AllLedsRed(HIGH);    break;
  cPattern = "2";                     //TESTING ONLY
}
}

void Pattern3(int i) {
  AllLedsOff();
  //int num = map(i, 0, 100, 0, 16);
  int num = i;
  switch (num) {
    case 1:
      digitalWrite(pLed1Green, HIGH);
    break;
    case 2:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
    break;
    case 3:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
    break;
    case 4:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
    break;
    case 5:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
    break;
    case 6:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
    break;
    case 7:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
    break;
    case 8:
      digitalWrite(pLed1Green, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 9:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Green, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 10:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Green, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 11:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Green, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 12:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Green, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 13:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Green, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 14:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
      digitalWrite(pLed7Green, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 15:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
      digitalWrite(pLed7Red, HIGH);
      digitalWrite(pLed8Green, HIGH);
    break;
    case 16:
      AllLedsOff();
      digitalWrite(pLed1Red, HIGH);
      digitalWrite(pLed2Red, HIGH);
      digitalWrite(pLed3Red, HIGH);
      digitalWrite(pLed4Red, HIGH);
      digitalWrite(pLed5Red, HIGH);
      digitalWrite(pLed6Red, HIGH);
      digitalWrite(pLed7Red, HIGH);
      digitalWrite(pLed8Red, HIGH);
    break;
  }
  
  cPattern = "3";                     //TESTING ONLY
}

void ledBlink() {
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  switch (color) {
    case bRed:
      AllLedsRed(ledState);
    break;
    case bGreen:
      AllLedsGreen(ledState);
    break;
    case bBlue:
      AllLedsBlue(ledState);
    break;
  }
}

//  ********************** END FUNCTIONS *****************
// ************************* ARDUINO SETUP FUNCTION *********************

void setup() {
Serial.begin(9600);
// configure pins
pinMode(pDigit, OUTPUT);
pinMode(pLed1Red, OUTPUT);
pinMode(pLed1Green, OUTPUT);
pinMode(pLed1Blue, OUTPUT);
pinMode(pLed2Red, OUTPUT);
pinMode(pLed2Green, OUTPUT);
pinMode(pLed2Blue, OUTPUT);
pinMode(pLed3Red, OUTPUT);
pinMode(pLed3Green, OUTPUT);
pinMode(pLed3Blue, OUTPUT);
pinMode(pLed4Red, OUTPUT);
pinMode(pLed4Green, OUTPUT);
pinMode(pLed4Blue, OUTPUT);
pinMode(pLed5Red, OUTPUT);
pinMode(pLed5Green, OUTPUT);
pinMode(pLed5Blue, OUTPUT);
pinMode(pLed6Red, OUTPUT);
pinMode(pLed6Green, OUTPUT);
pinMode(pLed6Blue, OUTPUT);
pinMode(pLed7Red, OUTPUT);
pinMode(pLed7Green, OUTPUT);
pinMode(pLed7Blue, OUTPUT);
pinMode(pLed8Red, OUTPUT);
pinMode(pLed8Green, OUTPUT);
pinMode(pLed8Blue, OUTPUT);
pinMode(pButton, INPUT);
pinMode(ledGND, OUTPUT);
// configure Seven Seg
Gear.setDigitPins(numDigits, pDigit);
Gear.setCommonCathode();
Gear.setDPPin(segDP);
// configure TImers
timerID = tBlink.setInterval(interval, ledBlink);
}

// ********************* END SETUP ********************
// ********************* ARDUINO PROGRAM LOOP **************************

void loop() {
  tBlink.run();
  data = Serial.read();

nShiftLight = i;  // TESTING ONLY Increment shift light pattern

  if (Serial.available()){

// COnvert irsdk shift data to percentage as int (0 - 100) HERE ses variable nShiftLight
// Get current Gear from irsdk HERE if gear < 1 set 0 = neutral and -1 = reverse

  }

// Set correct gear value here
//convert shift data from float percentage to int (0 - 100)
  
  ButtonState = digitalRead(pButton);
  if (ButtonState != prevButtonState) {
    if (ButtonState == HIGH) {
      if (nPattern >= totalPatterns) {
        nPattern = 0;
      } else {
        nPattern++;
      }
    }
    prevButtonState = ButtonState;
  }


// Turn on Pit SPeed Pattern if flag is set
if (irsdk_EngineWarnings == irsdk_pitSpeedLimiter) {
  if (nPattern != 0) {
    nPattern = bPitSpeed;
  }
} 

// Run SHift Pattern
  switch (nPattern) {
    case 1:
      Pattern1(nShiftLight);
      msg = "Pattern 1";
      break;
    case 2:
      Pattern2(nShiftLight);
      msg = "Pattern 2";
    break;
    case 3:
      Pattern3(nShiftLight);
      msg = "Pattern 3";
    break;
    case 0:
      AllLedsOff();
      tBlink.disable(timerID);
      msg = "All off";
     break;
     default:
      tBlink.enable(timerID);
      msg = "Pit Speed";
     break;
  }


// For testing ****************
unsigned long Millis = millis();
unsigned long segMillis = millis();
if (segMillis - segPrevMillis > interval) {
  segPrevMillis = segMillis;
  if (i < 16) {
    i++;
  } else {
    i = 0;
  }
  Serial.println(msg);                      // TESTING ONLY
}
// End testing block   *************************

// Read pot value
  val = analogRead(pBrightness);  
// led brightness
  ledBrightness = map(val, 0, 1023, 245, 254);
// sev seg brightness
  segBrightness = map(val, 0, 1023, 100, 5);
// set led brightness 
  analogWrite(ledGND, ledBrightness);

// Set 7 Seg Character
Gear.setDutyCycle(segBrightness);
Gear.write(i);                            // Change variable i for current gear string or int
Gear.clearDisp();
}

// ************************************* END PROGRAM *******************************
