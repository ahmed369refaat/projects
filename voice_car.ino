#include <SoftwareSerial.h>
SoftwareSerial voice(A1, A2); // TX, RX

#define enA 4
#define in1 6
#define in2 7
#define enB 3
#define in3 8
#define in4 9

const int trigger = 12; // Front ultrasonic sensor
const int echo = 13;   // Front ultrasonic sensor
long distance;
int move_forward = 0;

#include <NewPing.h>
#include <Servo.h>

Servo servo_motor;

NewPing sonar(trigger, echo);

int servoPin = 10;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  servo_motor.attach(servoPin);
  servo_motor.write(90);

  Serial.begin(9600);
  voice.begin(9600);
}

void check_distance()
{
  long dis;
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  dis = pulseIn(echo, HIGH);
  distance = dis / 29 / 2;
  Serial.print("distance:");
  Serial.println(distance);
}

void moveStop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void moveForward() {
  analogWrite(enA, 255);
  analogWrite(enB, 225);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward() {
  analogWrite(enA, 255);
  analogWrite(enB, 225);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(300);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void executeVoiceCommand() {
  while (voice.available()) {
    String command = voice.readString();

    if (command == "forward") {
      servo_motor.write(90);
      delay(500);
      check_distance();
      if (distance > 20) {
        moveForward();
        delay(100);
      } else {
        moveStop();
        delay(500);
      }
    } else if (command == "backward") {
      moveBackward();
      delay(100);
    } else if (command == "left") {
      servo_motor.write(180);
      delay(500);
      check_distance();
      if (distance > 20) {
        turnLeft();
        delay(100);
      } else {
        moveStop();
        delay(500);
      }
    } else if (command == "right") {
      servo_motor.write(20);
      delay(500);
      check_distance();
      if (distance > 20) {
        turnRight();
        delay(100);
      } else {
        moveStop();
        delay(500);
      }
    } else if (command == "stop") {
      moveStop();
      delay(100);
    } else if (command == "object") {
      int distance = readPing();

      if (distance <= 20) {
        moveStop();
        delay(300);
        moveBackward();
        delay(400);
        moveStop();
        delay(300);
        int distanceRight = lookRight();
        delay(300);
        int distanceLeft = lookLeft();
        delay(300);

        if (distanceRight >= distanceLeft) {
          turnRight();
          moveStop();
        } else {
          turnLeft();
          moveStop();
        }
      } else {
        moveForward();
      }
    }
  }
}

void loop() {
  executeVoiceCommand();

  // Check distance separately outside the voice command loop
  check_distance();
  if (distance <= 20) {
    // Obstacle detected, stop the robot
    moveStop();
    delay(500);
  }
}

int lookRight() {
  servo_motor.write(45);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(90);
  return distance;
}

int lookLeft() {
  servo_motor.write(135);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(90);
  return distance;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}
