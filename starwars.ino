#include <ADCTouch.h>

const int buttonPin = 12;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int buttonConstantPin = 2;
const int buttonAutoRandomPin = 10;
const int pump1 = 9;
const int pump2 = 5;
const int pump3 = 8;

// variables will change:
int randomPump = 0;
int buttonState = 0;         // variable for reading the pushbutton status
int buttonConstantState = 0;         // variable for reading the pushbutton status
int buttonAutoRandomState = 0;
bool autoOn = false;
bool touched = false;
int ref = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonConstantPin, INPUT_PULLUP);
  pinMode(buttonAutoRandomPin, INPUT_PULLUP);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  ref = ADCTouch.read(A0, 500);    //create reference values to
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  buttonConstantState = digitalRead(buttonConstantPin);
  buttonAutoRandomState = digitalRead(buttonAutoRandomPin);
  int touchValue = ADCTouch.read(A0);   //no second parameter
  touchValue -= ref;       //remove offset
  Serial.println(touchValue);
  if(touchValue > 20){
    touched = true;
  }else{
    touched = false;
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
    if(touched == true){
      randomizePump();
      Serial.println("Auto Mode Continued"); 
    }
  }

  if (buttonConstantState == LOW  ) {
    Serial.println("Constant Mode Activated");
    digitalWrite(pump1, HIGH);
    digitalWrite(pump2, HIGH);
    digitalWrite(pump3, HIGH);
  } else {
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, LOW);
    digitalWrite(pump3, LOW);
  }
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW  ) {
    Serial.println("Single Random Mode Activated");
    randomizePump();
  }
}

void activatePump(int pump){
  digitalWrite(pump, HIGH);
  delay(3000);
  digitalWrite(pump, LOW);
}

void randomizePump(){
  randomPump = random(3);
  Serial.println(randomPump);
  if (randomPump == 0){
    activatePump(pump1);
  }else if (randomPump == 1){
    activatePump(pump2);
  }else{
    activatePump(pump3);
  }
  delay(3000);
}

