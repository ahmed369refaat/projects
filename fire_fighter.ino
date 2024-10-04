#include <AFMotor.h>
int Flame = A2;

const int sensorMin = 0;
const int sensorMax = 1023;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor Fan(4);

char commands;
int sensorReading;
int range, oldrange =0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorReading = analogRead (Flame);
  range = map(sensorReading, sensorMax, sensorMin, 0, 4);
  if (range==0 && range != oldrange) {
    Fan.setSpeed(254);
    Fan.run (FORWARD);
    Serial.println("close fire");
    oldrange = range;
  }
  else if (range> 0 && range <3 && range != oldrange) {
    Fan.setSpeed(0);
    Fan.run (RELEASE);
    Serial.println ("Disntant fire");
    oldrange = range;
  }
  else if (range==3 && range != oldrange) {
    Fan.setSpeed(0);
    Fan.run (RELEASE);
    Serial.println ("No fire");
    oldrange = range;
  }
  if (Serial.available() > 0) {
    commands = Serial.read();
    switch(commands) {
      case 'F':
      forward();
      break;
      case 'B':
      back();
      break;
      case 'R':
      right();
      break;
      case 'L':
      left();
      break;
  default:
   break;
    }
  }
}


void forward() {
  motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
}

void back() {
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
}

void left() {
  motor1.setSpeed(255);
  motor1.run(FORWARD);
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
}

void right() {
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.setSpeed(255);
  motor2.run(FORWARD);
}
