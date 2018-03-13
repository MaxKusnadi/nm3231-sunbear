#include <ADCTouch.h>

const int buttonPin = 3;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
const int buttonConstantPin = 2;
const int buttonAutoRandomPin = 4;
const int pump1 = 12;
const int pump2 = 11;
const int pump3 = 10;
const int led1 = 5;
const int led2 = 6;
const int led3 = 7;

// variables will change:
int randomPump = 0;
int buttonState = 0;         // variable for reading the pushbutton status
int buttonConstantState = 0;         // variable for reading the pushbutton status
int buttonAutoRandomState = 0;

bool autoOn = false;
bool touchedPump1 = false;
bool touchedPump2 = false;
bool touchedPump3 = false;

int refPump1 = 0;
int refPump2 = 0;
int refPump3 = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonConstantPin, INPUT_PULLUP);
  pinMode(buttonAutoRandomPin, INPUT_PULLUP);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  
  refPump1 = ADCTouch.read(A0, 500);    //create reference values to
  refPump2 = ADCTouch.read(A1, 500);    //create reference values to
  refPump3 = ADCTouch.read(A2, 500);    //create reference values to
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  buttonConstantState = digitalRead(buttonConstantPin);
  buttonAutoRandomState = digitalRead(buttonAutoRandomPin);
  
  int touchValuePump1 = ADCTouch.read(A0);
  int touchValuePump2 = ADCTouch.read(A1);
  int touchValuePump3 = ADCTouch.read(A2);
  touchValuePump1 -= refPump1;
  touchValuePump2 -= refPump2;
  touchValuePump3 -= refPump3;

  Serial.print("Touch value pump 1: ");
  Serial.println(touchValuePump1);
  Serial.print("Touch value pump 2: ");
  Serial.println(touchValuePump2);
  Serial.print("Touch value pump 3: ");
  Serial.println(touchValuePump3);
  
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

  if(touchValuePump3 > 20){
    touchedPump3 = true;
  }else{
    touchedPump3 = false;
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
    }else if(randomPump == 1){
      touched = touchedPump2;
    }else{
      touched = touchedPump3;
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
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    
  } else {
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, LOW);
    digitalWrite(pump3, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
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
  Serial.print("Pump number: ");
  Serial.println(randomPump + 1);
  if (randomPump == 0){
    digitalWrite(led1, HIGH);
    activatePump(pump1);
  }else if (randomPump == 1){
    digitalWrite(led2, HIGH);
    activatePump(pump2);
  }else{
    digitalWrite(led3, HIGH);
    activatePump(pump3);
  }
  delay(3000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  
}

