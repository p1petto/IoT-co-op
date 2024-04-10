#include <SoftwareSerial.h>

#define SPEED_1 5
#define DIR_1 4
#define SPEED_2 6
#define DIR_2 7

#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define CIRCLE 'C'
#define CROSS 'X'
#define TRIANGLE 'T'
#define SQUARE 'S'
#define START 'A'
#define PAUSE 'P'

SoftwareSerial MySerial(10, 11);


unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 500;
const int MIN_DISTANCE_LEFT = 20;
const int MIN_DISTANCE_FRONT = 11;

float distanceLeft = 0;
float distanceFront = 0;

float countTurnLeft = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Goodnight PC");
  MySerial.begin(9600);
  MySerial.println("Hello phone");
  for(int i = 4; i <= 7; i++) {
    pinMode(i, OUTPUT);
  }  
}

void move_back(int car_speed) {
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);
  analogWrite(SPEED_2, car_speed);
}

void move_forward(int car_speed) {
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, car_speed);
  analogWrite(SPEED_2, car_speed);
}

void rotate_right(int car_speed) {
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, car_speed);
  analogWrite(SPEED_2, car_speed);
}

void rotate_left(int car_speed) {
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);
  analogWrite(SPEED_2, car_speed);
}

void stop() {
  analogWrite(SPEED_1, 0);
  analogWrite(SPEED_2, 0);
}

void turn_left(int car_speed, float steepness) {
  if (steepness > 1) {
    steepness = 1;
  }
  if (steepness < 0) {
    steepness = 0;
  }
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);
  analogWrite(SPEED_2, car_speed * steepness);
}

void turn_right(int car_speed, float steepness) {
  if (steepness > 1) {
    steepness = 1;
  }
  if (steepness < 0) {
    steepness = 0;
  }
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed * steepness);
  analogWrite(SPEED_2, car_speed);
}

float readDistance(int trig_pin, int echo_pin) {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  unsigned long duration = pulseIn(echo_pin, HIGH);
  float distance_cm = duration * 0.0343 / 2.0;
  return distance_cm;
}

void executeCommand(char command) {
  switch (command) {
    case FORWARD:
    {
      Serial.println("F");
      move_forward(250);
      break;
    }
      
    case BACKWARD:
    {
      Serial.println("B");
      move_back(250);
      // Perform action for moving backward
      break;
    }
      
    case LEFT:
    {
      rotate_left(250);
      Serial.println("L");
      // Perform action for turning left
      break;
    }
      
    case RIGHT:
    {
      rotate_right(250);
      Serial.println("R");
      // Perform action for turning right
      break;
    }
      
    case CIRCLE:
      // Perform action for circle
      break;
    case CROSS:
      // Perform action for immediate stop or crossing
      break;
    case TRIANGLE:
      // Perform action for toggling a state (e.g., LED on/off)
      break;
    case SQUARE:
      // Perform action for retrieving and sending status information
      break;
    case START:
      // Perform action for starting a process or operation
      break;
    case PAUSE:
    {
      stop();
      Serial.println("P");
      // Perform action for pausing a process or operation
      break;
    }
      
    default:
      Serial.println(command);
      break;
  }
}
void loop() {
  if (MySerial.available() >= 2) {
    char command = MySerial.read();
    char nil = MySerial.read();
    executeCommand(command);
    //Serial.println("in: "+ command);
  }
  // unsigned long currentMillis = millis();

  // if (currentMillis - lastPublishTime >= publishInterval) {

  //   Serial.print("Distance left: ");
  //   distanceLeft = readDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
  //   Serial.print(distanceLeft);
  //   Serial.println(" cm");

  //   Serial.print("Distance front: ");
  //   distanceFront = readDistance(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
  //   Serial.print(distanceFront);
  //   Serial.println(" cm");

  //   lastPublishTime = currentMillis;
  // }
}