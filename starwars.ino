#include <ADCTouch.h>
#include<FastLED.h>
#define NUM_LEDS 100

CRGBArray<NUM_LEDS> leds1;
CRGBArray<NUM_LEDS> leds2;

// Delays
const int delay_time = 10000;
const int pump_time = 3000;

// Buttons
const int buttonConstantPin = 2;
const int buttonManualRandomPin = 3;     // the number of the pushbutton pin
const int buttonAutoRandomPin = 4;

// Pumps
const int pump1 = 12;
const int pump2 = 11;
const int pump3 = 10;
const int pump4 = 9;

// LED
const int led1 = 5;
const int led2 = 6;
const int pump_1_start = 33;
const int pump_1_end = 53;
const int pump_2_start = 83;
const int pump_2_end = 100;
const int pump_3_start = 33;
const int pump_3_end = 53;
const int pump_4_start = 83;
const int pump_4_end = 100;

// State of the buttons: pressed or not
int randomPump = 0;
int buttonManualRandomState = 0;         // variable for reading the pushbutton status
int buttonConstantState = 0;         // variable for reading the pushbutton status
int buttonAutoRandomState = 0;

// State of the touch sensors: touched or not
bool autoOn = false;
bool touchedPump1 = false;
bool touchedPump2 = false;
bool touchedPump3 = false;
bool touchedPump4 = false;

// Reference value for the touch sensors
int refPump1 = 0;
int refPump2 = 0;
int refPump3 = 0;
int refPump4 = 0;

void setup() {
  // initialize the LED pins:
  FastLED.addLeds<NEOPIXEL,led1>(leds1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL,led2>(leds2, NUM_LEDS); 

  // initialize the button pins:
  pinMode(buttonManualRandomPin, INPUT_PULLUP);
  pinMode(buttonConstantPin, INPUT_PULLUP);
  pinMode(buttonAutoRandomPin, INPUT_PULLUP);

  // initialize the pump pins
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  pinMode(pump4, OUTPUT);

  // get the reference value for the touch sensor
  refPump1 = ADCTouch.read(A0, 500);    //create reference values to pump 1
  refPump2 = ADCTouch.read(A1, 500);    //create reference values to pump 2
  refPump3 = ADCTouch.read(A2, 500);    //create reference values to pump 3
  refPump4 = ADCTouch.read(A3, 500);    //create reference values to pump 4
 
  Serial.begin(9600);
  setup_light();
}

void loop() {
  // read the state of the pushbutton value:
  buttonManualRandomState = digitalRead(buttonManualRandomPin);
  buttonConstantState = digitalRead(buttonConstantPin);
  buttonAutoRandomState = digitalRead(buttonAutoRandomPin);

  // read the value of the touch sensors
  int touchValuePump1 = ADCTouch.read(A0);
  int touchValuePump2 = ADCTouch.read(A1);
  int touchValuePump3 = ADCTouch.read(A2);
  int touchValuePump4 = ADCTouch.read(A3);
  
  touchValuePump1 -= refPump1;
  touchValuePump2 -= refPump2;
  touchValuePump3 -= refPump3;
  touchValuePump4 -= refPump4;

  Serial.print("Touch value pump 1: ");
  Serial.println(touchValuePump1);
  Serial.print("Touch value pump 2: ");
  Serial.println(touchValuePump2);
  Serial.print("Touch value pump 3: ");
  Serial.println(touchValuePump3);
  Serial.print("Touch value pump 4: ");
  Serial.println(touchValuePump4);
  
  if(touchValuePump1 > 18){
    touchedPump1 = true;
  }else{
    touchedPump1 = false;
  }

  if(touchValuePump2 > 18){
    touchedPump2 = true;
  }else{
    touchedPump2 = false;
  }

  if(touchValuePump3 > 18){
    touchedPump3 = true;
  }else{
    touchedPump3 = false;
  }

  if(touchValuePump4 > 18){
    touchedPump4 = true;
  }else{
    touchedPump4 = false;
  }
 
  if(buttonAutoRandomState == LOW){
    if(autoOn == false){
      autoOn = true;
      Serial.println("Auto Mode Activated"); 
      randomizePump();
    }else{
      autoOn = false;
      Serial.println("Auto Mode Deactivated");
      auto_mode_off_light();
    }
  }

  if(autoOn == true){
    bool touched;
    if(randomPump == 0){
      touched = touchedPump1;
    }else if(randomPump == 1){
      touched = touchedPump2;
    }else if(randomPump == 2){
      touched = touchedPump3;
    }else{
      touched = touchedPump4;
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
    digitalWrite(pump3, HIGH);
    digitalWrite(pump4, HIGH);
    light_led(0);
    light_led(1);
    light_led(2);
    light_led(3);
    
  } else {
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, LOW);
    digitalWrite(pump3, LOW);
    digitalWrite(pump4, LOW);
    off_led(0);
    off_led(1);
    off_led(2);
    off_led(3);
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
  delay(pump_time);
  digitalWrite(pump, LOW);
  off_led(randomPump);
}

void randomizePump(){
  randomPump = random(4);
  Serial.print("Pump number: ");
  Serial.println(randomPump + 1);
  if (randomPump == 0){
    activatePump(pump1);
  }else if(randomPump == 1){
    activatePump(pump2);
  }else if(randomPump == 2){
    activatePump(pump3);
  }else{
    activatePump(pump4);
  }
  delay_light();
}

void light_led(int pump){
  if(pump == 0){
    for(int i = pump_1_start; i < pump_1_end; i++) {   
      leds1[i] = CRGB::Crimson;
    }
    FastLED.show();
  }else if(pump == 1){
    for(int i = pump_2_start; i < pump_2_end; i++) {   
      leds1[i] = CRGB::DeepSkyBlue;
    }
    FastLED.show();
  }else if(pump == 2){
    for(int i = pump_3_start; i < pump_3_end; i++) {   
      leds2[i] = CRGB::Chocolate;
    }
    FastLED.show();
  }else{
    for(int i = pump_4_start; i < pump_4_end; i++) {   
      leds2[i] = CRGB::DeepPink;
    }
    FastLED.show();
  }
}

void off_led(int pump){
  if(pump == 0){
    for(int i = pump_1_start; i < pump_1_end; i++) {   
      leds1[i] = CRGB::Black;
    }
    FastLED.show();
  }else if(pump == 1){
    for(int i = pump_2_start; i < pump_2_end; i++) {   
      leds1[i] = CRGB::Black;
    }
    FastLED.show();
  }else if(pump == 2){
    for(int i = pump_3_start; i < pump_3_end; i++) {   
      leds2[i] = CRGB::Black;
    }
    FastLED.show();
  }else{
    for(int i = pump_4_start; i < pump_4_end; i++) {   
      leds2[i] = CRGB::Black;
    }
    FastLED.show();
  }
}

void delay_light(){
  int iteration = delay_time/1000;
  for(int i = 0; i < iteration; i++){
    for(int i = pump_1_end; i < pump_2_start; i++) {   
      leds1[i] = CRGB::ForestGreen;
    }
    for(int i = pump_3_end; i < pump_4_start; i++) {   
      leds2[i] = CRGB::ForestGreen;
    }
    FastLED.show();
    delay(500);
    for(int i = pump_1_end; i < pump_2_start; i++) {   
      leds1[i] = CRGB::Black;
    }
    for(int i = pump_3_end; i < pump_4_start; i++) {   
      leds2[i] = CRGB::Black;
    }
    FastLED.show();
    delay(500);
  }
}

void auto_mode_off_light(){
  for(int i = 0; i < 3; i++){
    for(int i = 0; i < 100; i++) {   
      leds1[i] = CRGB::DarkOrange;
      leds2[i] = CRGB::DarkOrange;
    }
    FastLED.show();
    delay(500);
    for(int i = 0; i < 100; i++) {   
      leds1[i] = CRGB::Black;
      leds2[i] = CRGB::Black;
    }
    FastLED.show();
    delay(500);
  }
}

void setup_light(){
  for(int i = 0; i < 100; i++) {   
    leds1[i] = CRGB::Red;
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
  delay(500);
  for(int i = 0; i < 100; i++) {   
    leds1[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);
  for(int i = 0; i < 100; i++) {   
    leds1[i] = CRGB::Red;
    leds2[i] = CRGB::Red;
  }
  FastLED.show();
  delay(500);
  for(int i = 0; i < 100; i++) {   
    leds1[i] = CRGB::Black;
    leds2[i] = CRGB::Black;
  }
  FastLED.show();
}
  
