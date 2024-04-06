#define SPEED_1      5 
#define DIR_1        2 
#define SPEED_2      6 
#define DIR_2        7 

#define TRIG_PIN_LEFT  11  // transmitter
#define ECHO_PIN_LEFT  10  // receiver

#define TRIG_PIN_FRONT  9  // transmitter
#define ECHO_PIN_FRONT  3  // receiver

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 500;
const int MIN_DISTANCE_LEFT = 20;
const int MIN_DISTANCE_FRONT = 11;

float distanceLeft = 0;
float distanceFront = 0;

float countTurnLeft = 0;
 
void setup() {
  Serial.begin(9600);
  for (int i = 5; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
  pinMode(2, OUTPUT);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);
} 

void move_forward(int car_speed) {
  digitalWrite(DIR_1, HIGH);  
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);  
  analogWrite(SPEED_2, car_speed);
}

void move_back(int car_speed) {
  digitalWrite(DIR_1, LOW);  
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, car_speed);  
  analogWrite(SPEED_2, car_speed);
}

void rotate_left(int car_speed) {
  digitalWrite(DIR_1, HIGH);  
  digitalWrite(DIR_2, HIGH);
  analogWrite(SPEED_1, car_speed);  
  analogWrite(SPEED_2, car_speed);
}

void rotate_right(int car_speed) {
  digitalWrite(DIR_1, LOW);  
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);  
  analogWrite(SPEED_2, car_speed);
}

void stop(){
  analogWrite(SPEED_1, 0);  
  analogWrite(SPEED_2, 0);
}

void turn_left(int car_speed, float steepness) {
  if(steepness > 1) {
    steepness = 1;
  }
    if(steepness < 0) {
    steepness = 0;
  }
  digitalWrite(DIR_1, HIGH);  
  digitalWrite(DIR_2, LOW);
  analogWrite(SPEED_1, car_speed);  
  analogWrite(SPEED_2, car_speed * steepness);
}

void turn_right(int car_speed, float steepness) {
  if(steepness > 1) {
    steepness = 1;
  }
    if(steepness < 0) {
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
 
void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastPublishTime >= publishInterval) {
    
    Serial.print("Distance left: ");
    distanceLeft = readDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
    Serial.print(distanceLeft);
    Serial.println(" cm");

    Serial.print("Distance front: ");
    distanceFront = readDistance(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
    Serial.print(distanceFront);
    Serial.println(" cm");
    
    lastPublishTime = currentMillis;
  }

  
  if(distanceFront > MIN_DISTANCE_FRONT && distanceLeft < MIN_DISTANCE_LEFT){
    move_forward(150);
    //rotate_right(150);
    Serial.println("вперед");
    distanceFront = readDistance(TRIG_PIN_FRONT, ECHO_PIN_FRONT);
    distanceLeft = readDistance(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
    //if (distanceLeft < MIN_DISTANCE_LEFT) move_back(150);
  }
  
  else if (distanceLeft > MIN_DISTANCE_LEFT){
    turn_left(250, 0.3);
    Serial.println("налево");
  }
  else if (distanceFront < MIN_DISTANCE_FRONT && distanceLeft < MIN_DISTANCE_LEFT){
    //move_back(250);
    rotate_right(250);
    Serial.println("направо");
  }
  // else {
  //   rotate_left(250);
  // }
  
}