#include <Stepper.h>
#include <PCF8574.h>

PCF8574 stepperExpander(0x20);              //Create the PCF8574 object at the hex address the PCF8574 chip is at 
const int stepsPerRevolution = 2048;        // Steps per revolution for your stepper motor

Stepper myStepperX(stepperExpander,stepsPerRevolution, 0, 2, 1, 3);       //Declaring stepper motor inputs 0 = P0, 1 = P1,...
Stepper myStepperY(stepperExpander,stepsPerRevolution, 4, 6, 5, 7);

float xcoordinates[10] = {0, 37.5, 50, 37.5, 0, -37.5, -50, -37.5, 0,  0};  //X coordinates for circle
float ycoordinates[10] = {50,37.5, 0, -37.5, -50, -37.5, 0, 37.5, 50, 0};   //Y coordinates for circle
int ArrayLength = sizeof(xcoordinates) / sizeof(int);                       //Getting length of coordinates array

double xdiff = 0, ydiff = 0, stepX = 0, stepY = 0, smaller = 0;

int speed = 16;                                                             // setting RPM for motor (for 28BYJ-48 is 16 rpm max for 2048 spr)
void setup() {
  myStepperX.setSpeed(speed);
  myStepperY.setSpeed(speed);
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(12,INPUT);                                                        //endstop
  pinMode(11,INPUT);                                                        //endstop
  stepperExpander.begin();                                                  //start of PCF8574
}

void loop() {
  for (int i = 0; i < ArrayLength; i++)
  {
    if (i==0){
      xdiff = xcoordinates[i];
      ydiff = ycoordinates[i];
      }
    else{
      xdiff = xcoordinates[i] - xcoordinates[i-1];
      ydiff = ycoordinates[i] - ycoordinates[i-1];
      }
    if (abs(xdiff) < abs(ydiff) and xdiff != 0){
      stepY = (ydiff / abs(xdiff)); 
      if (xdiff <0){
        stepX = -1;
      }
      else{
        stepX = 1;
      }
    }
    else if (abs(ydiff) < abs(xdiff) and ydiff != 0){
      stepX = (xdiff / abs(ydiff));
      if (ydiff <0){
        stepY = -1;
      }
      else{
        stepY = 1;
      }
    }

    else if (xdiff == 0){
      stepX = 0;
      stepY = ydiff;
      smaller = 1;
    }

    else if (ydiff == 0){
      stepX = xdiff;
      stepY = 0;
      smaller = 1;
    }
    else{
      smaller = xdiff;
      if (xdiff<0)
      {
        stepX = -1;
      } 
      else{
        stepX = 1;
      }
      if (ydiff<0)
      {
        stepY = -1;
      } 
      else{
        stepY = 1;
      }
    }
    if (abs(ydiff) < abs(xdiff)){
      if (ydiff == 0){
        smaller = 1;
      }
      else{
        smaller = abs(ydiff);
      }
    }
    else
      if (xdiff == 0){
        smaller = 1;
      }
      else{
        smaller = abs(xdiff);
      }
  for (int n = 0; n < smaller; n++){
    digitalWrite(10,LOW);                                                                           //Switch laser off
    myStepperX.step(stepX);
    myStepperY.step(stepY);
    digitalWrite(10,HIGH);                                                                          //Switch laser on
    delay(1);
    }
  }

  delay(1000);
}

