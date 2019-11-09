// Microswith and potmeter
int BUTTON_PIN = 12;
int TOGGLE = 1;

// Motors
#define EnA 10
#define EnB 5
#define In1 9
#define In2 8
#define In3 7
#define In4 6

// Servo
#include <Servo.h> 

// Declare the Servo pin 
int servoPin = 13; 

// Create a servo object 
Servo Servo1; 

// display
#include "SevenSegmentTM1637.h"

const byte PIN_CLK = 2;   // define CLK pin (any digital pin)
const byte PIN_DIO = 3;   // define DIO pin (any digital pin)

SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);

// mp3 player
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(4, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Variables
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int motorPower = 0;
int playSound = 1;

void setup() {

  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT);

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  // All motor control pins are outputs
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  // start display
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.print("0");      // display INIT on the display

  // We need to attach the servo to the used pin number 
  Servo1.attach(servoPin);
  Servo1.write(0);

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
}

void loop() {

  // read the pot meter, convert to DC motor values
  int motorPower = analogRead(A0) / 4;

  // read the sound toggle swicth
  playSound - digitalRead(TOGGLE);
  
  // read the pushbutton input pin:
  buttonState = digitalRead(BUTTON_PIN);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
     if(playSound == 1) {
        myDFPlayer.play(2);
     }
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

    // start engines

    // turn on motor A
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    // set speed
    analogWrite(EnA, motorSpeed);
    // turn on motor B
    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);
    // set speed
    analogWrite(EnB, motorSpeed);
    
    // wait engines to get to speed
    delay(800);
        
    // Open the gate
    Servo1.write(90); 
    
    // wait for ball te pass
    delay(35000); 

    // stop engines
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);  
    digitalWrite(In3, LOW);
    digitalWrite(In4, LOW);
      
    // close the gate 
    Servo1.write(0); 

    // notify Joep of ball ejection
     if(playSound == 1) {
        myDFPlayer.play(3);
     }

    // write to display
    display.print(buttonPushCounter);  
}
