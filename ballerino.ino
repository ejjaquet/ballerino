// CONSTANTS
int led = LED_BUILTIN;
int BUTTON_PIN = 12;
int POT_METER = A0;

// Servo
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 13; 
// Create a servo object 
Servo Servo1; 

// Variables
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int motorPower = 0;

void setup() {

  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // We need to attach the servo to the used pin number 
  Servo1.attach(servoPin);
  Servo1.write(0);
}

void loop() {

  // read the pot meter, convert to DC motor values
  int motorPower = analogRead(POT_METER) / 4;
  
  // read the pushbutton input pin:
  buttonState = digitalRead(BUTTON_PIN);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      ejectBall(motorPower);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;  
}

void ejectBall(int motorSpeed) {
    buttonPushCounter++;

    // start engine
    delay(1000);
    // Make servo go to 0 degrees 
    Servo1.write(90); 
    delay(5000); 
    // Make servo go to 180 degrees 
    Servo1.write(0); 
      
    Serial.println("on");
    Serial.print("number of button pushes: ");
    Serial.println(buttonPushCounter);
    Serial.print("Pot meter value: ");
    Serial.println(motorSpeed);
  
}
