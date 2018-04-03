#include <ADCTouch.h>
#include<FastLED.h>
#define NUM_LEDS 100

CRGBArray<NUM_LEDS> leds;

// Delays
const int delay_time(3000);
// Buttons
const int buttonConstantPin = 2;
const int buttonManualRandomPin = 3;     // the number of the pushbutton pin
const int buttonAutoRandomPin = 4;

// Pumps
const int pump1 = 12;
const int pump2 = 11;

// LED
const int led = 5;

// State of the buttons: pressed or not
int randomPump = 0;
int buttonManualRandomState = 0;         // variable for reading the pushbutton status
int buttonConstantState = 0;         // variable for reading the pushbutton status
int buttonAutoRandomState = 0;

// State of the touch sensors: touched or not
bool autoOn = false;
bool touchedPump1 = false;
bool touchedPump2 = false;

// Reference value for the touch sensors
int refPump1 = 0;
int refPump2 = 0;

void setup() {
  // initialize the LED pins:
  FastLED.addLeds<NEOPIXEL,led>(leds, NUM_LEDS); 

  // initialize the button pins:
  pinMode(buttonManualRandomPin, INPUT_PULLUP);
  pinMode(buttonConstantPin, INPUT_PULLUP);
  pinMode(buttonAutoRandomPin, INPUT_PULLUP);

  // initialize the pump pins
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);

  // get the reference value for the touch sensor
  refPump1 = ADCTouch.read(A0, 500);    //create reference values to
  refPump2 = ADCTouch.read(A1, 500);    //create reference values to
  
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  buttonManualRandomState = digitalRead(buttonManualRandomPin);
  buttonConstantState = digitalRead(buttonConstantPin);
  buttonAutoRandomState = digitalRead(buttonAutoRandomPin);

  // read the value of the touch sensors
  int touchValuePump1 = ADCTouch.read(A0);
  int touchValuePump2 = ADCTouch.read(A1);
  
  touchValuePump1 -= refPump1;
  touchValuePump2 -= refPump2;

  Serial.print("Touch value pump 1: ");
  Serial.println(touchValuePump1);
  Serial.print("Touch value pump 2: ");
  Serial.println(touchValuePump2);
  
  if(touchValuePump1 > 20){
    touchedPump1 = true;
  }else{
    touchedPump1 = false;
  }

  if(touchValuePump2 > 20){
    touchedPump2 = true;
  }else{
    touchedPump2 = false;
  }
 
  if(buttonAutoRandomState == LOW){
    if(autoOn == false){
      autoOn = true;
      Serial.println("Auto Mode Activated"); 
      randomizePump();
    }else{
      autoOn = false;
      Serial.println("Auto Mode Deactivated");
      delay(3000);
    }
  }

  if(autoOn == true){
    bool touched;
    if(randomPump == 0){
      touched = touchedPump1;
    }else{
      touched = touchedPump2;
    }
    
    if(touched == true){
      Serial.println("Auto Mode Continued"); 
      randomizePump();
    }
  }

  if (buttonConstantState == LOW  ) {
    Serial.println("Constant Mode Activated");
    digitalWrite(pump1, HIGH);
    digitalWrite(pump2, HIGH);
    light_led(0);
    light_led(1);
    
  } else {
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, LOW);
    off_led(0);
    off_led(1);
  }
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonManualRandomState == LOW  ) {
    Serial.println("Single Random Mode Activated");
    randomizePump();
  }
}

void activatePump(int pump){
  digitalWrite(pump, HIGH);
  light_led(randomPump);
  delay(delay_time);
  digitalWrite(pump, LOW);
  off_led(randomPump);
}

void randomizePump(){
  randomPump = random(2);
  Serial.print("Pump number: ");
  Serial.println(randomPump + 1);
  if (randomPump == 0){
    activatePump(pump1);
  }else{
    activatePump(pump2);
  }
  delay(delay_time);
}

void light_led(int pump){
  if(pump == 0){
    for(int i = 0; i < 10; i++) {   
      leds[i] = CRGB::Red;
    }
    FastLED.show();
  }else{
    for(int i = 25; i < 35; i++) {   
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
  }
}

void off_led(int pump){
  if(pump == 0){
    for(int i = 0; i < 10; i++) {   
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }else{
    for(int i = 25; i < 35; i++) {   
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}
