#define MODE_RECEIVER 0
#define MODE_SENDER 1

#define PIN_SENDER 9
#define PIN_RECEIVER 2

#define PIN_TUMBLER 13

const int dataPin = 4;  // blue wire to 74HC595 pin 14
const int latchPin = 7; // green to 74HC595 pin 12
const int clockPin = 8; // yellow to 74HC595 pin 11
int timeUnitMs = 60;


String inputString = "";
String resultMessage = "";
String receivedMessage = "";
int mode = 0;
bool transmited = false;

String morseAlphabet[] = {"-----", ".----", "..---", "...--", "....-", //0-4
						".....", "-....", "--...", "---..", "----.",//5-9
						".-", "-...", "-.-.", "-..", ".", // A-E
						"..-.", "--.", "....", "..", ".---",//F-J
						"-.-", ".-..", "--", "-.", "---", //K-O
						".--.", "--.-", ".-.", "...","-",//P-T
						"..-", "...-", ".--", "-..-", "-.--",//U-Y
						"--.."};//Z
byte digits[] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110,
                0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110,
                0b11101110, 0b00111110, 0b10011100, 0b01111010, 0b10011110,
                0b10001110, 0b10111110, 0b00101110, 0b0110000, 0b01111000,
                0b01101110, 0b00011100, 0b00101010, 0b1110110, 0b11111100,
                0b11001110, 0b11100110, 0b10001100, 0b10110110, 0b00011110,
                0b01111100, 0b00111000, 0b01111110, 0b00001110, 0b01110110,
                0b10010010
                };


void translate_morse(String str) {
  str.toUpperCase();
	for (int i = 0; i < str.length(); i++) {
      
      if (str[i] == ' ') Serial.print("   ");
      setDisplay(0b00000000);
      if ((str[i] - '0') >= 0 && (str[i] - '0') <= 9){
        Serial.print(morseAlphabet[(str[i] - '0')]);
      	setDisplay(digits[(str[i] - '0')]);
      }
      if ((str[i] - 'A') >= 0 && (str[i] - 'A') <= 35){
        Serial.print(morseAlphabet[(str[i] - 'A') + 10]);
        setDisplay(digits[(str[i] - 'A') + 10]);
      }
      delay(500);
      Serial.print(" ");
		
	}
   Serial.println();
}

 
void setup() {
    
  // initialize I/O pins
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(PIN_SENDER, OUTPUT);
  pinMode(PIN_RECEIVER, INPUT);

  if (digitalRead(PIN_TUMBLER) == 1) {
    Serial.println("Controller Mode: SENDER");
    mode = MODE_SENDER;
  } else {
    mode = MODE_RECEIVER;
    Serial.println("Controller Mode: RECEIVER");
    
  }
}

void loop() {
  // if (digitalRead(PIN_TUMBLER) != mode) {
  //   if (digitalRead(PIN_TUMBLER) == 1) {
  //   Serial.println("Controller Mode: SENDER");
  //   mode = MODE_SENDER;
  // } else {
  //   mode = MODE_RECEIVER;
  //   Serial.println("Controller Mode: RECEIVER");
  // }
  // }
 
  if (MODE_RECEIVER != mode) {
    mode = MODE_RECEIVER;
    Serial.println("Controller Mode: RECEIVER");
  }
  // Serial.println("RESSSTINGDHSG");
  
  if (mode == MODE_RECEIVER) {
    receiveMorse();
  }
  else{
   if(Serial.available()){
     inputString = Serial.readString();
     if (inputString.length() > 0){
    	transmitMorse(inputString);
    }
    }
  }
}

void setDisplay(byte code) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, code);
  digitalWrite(latchPin, HIGH);
}

void transmitMorse(String inputString) {
  String prepared = inputString;
  Serial.print("Sending: ");
  prepared.toUpperCase();
  Serial.println(prepared);
  for (int i = 0; i < prepared.length(); i++) {
    char currentLetter = prepared.charAt(i);
    int letterIndex = 0;
    if (currentLetter == ' ') {
      delay(7 * timeUnitMs);
      Serial.println();
      continue;
    } else {
      
      if ((prepared[i] - '0') >= 0 && (prepared[i] - '0') <= 9){
        letterIndex = prepared[i] - '0';
      }
      if ((prepared[i] - 'A') >= 0 && (prepared[i] - 'A') <= 35){
        letterIndex = (prepared[i] - 'A') + 10;
      }
      }
      transmitWord(morseAlphabet[letterIndex]);
      Serial.print("Transmitting: ");
      Serial.println(morseAlphabet[letterIndex]);
      delay(3 * timeUnitMs);
    }
  
  delay(8 * timeUnitMs);
}

void transmitWord(String dotDashSequence) {
  for (int i = 0; i < dotDashSequence.length(); i++) {
    if (dotDashSequence[i] == '.') {
      transmitSignal(1);
    } else if (dotDashSequence[i] == '-') {
      transmitSignal(3);
    } else if (dotDashSequence[i] == ' ') {
      delay(3 * timeUnitMs);
    } else {
      delay(5 * timeUnitMs);
    }
  }
}

void transmitSignal(int duration) {
  digitalWrite(PIN_SENDER, LOW);
  delay(duration * timeUnitMs);
  digitalWrite(PIN_SENDER, HIGH);
  delay(1 * timeUnitMs);
}

void receiveMorse() {
  // delay(1);
  int idleCounter = 0;
  while (digitalRead(PIN_RECEIVER) == HIGH) {
    delay(timeUnitMs);
    idleCounter++;
    if (idleCounter > 10) {
      transmited = true;
      break;
    }
  }
  if (transmited == true) {
    // printMessage(resultMessage);
    resultMessage = "";
    transmited = false;
  }

  if (idleCounter == 7) {
    receivedMessage += "%";
  } else if (idleCounter == 3) {
    receivedMessage += " ";
  } else if (idleCounter > 7) {
    // Serial.println(idleCounter);
    // resultMessage += decodeMorse(receivedMessage) + " ";
    receivedMessage = "";
  }

  int signalDuration = 0;
  while (digitalRead(PIN_RECEIVER) == LOW) {
    Serial.println("Receiver, LOW");
    delay(timeUnitMs);
    signalDuration++;
  }

  if (signalDuration == 1) {
    receivedMessage += '.';
  } else if (signalDuration == 3) {
    receivedMessage += '-';
  }

  Serial.println(receivedMessage);
  delay(timeUnitMs);
}


