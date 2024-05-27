
int columns[] = { 9, 10, 11, 12 };  //low
int layers[] = { 3, 2 };            //hidh
// Одна колонка один слой
void onec_onel() {
  digitalWrite(layers[0], HIGH);
  digitalWrite(layers[1], LOW);

  // 1
  for (int i = 0; i < 2; i++) {
    digitalWrite(layers[0], !digitalRead(layers[0]));
    digitalWrite(layers[1], !digitalRead(layers[1]));
    for (int j = 0; j < 4; j++) {
      digitalWrite(columns[j], LOW);
      delay(500);
      digitalWrite(columns[j], HIGH);
    }
  }
}

int states[8][6] = {};

int states_queue[8] = {};

int states_indexes[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int combo[8];

void lowerp(){
    for (int i = 0; i < 4; i++) {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(layers[i], OUTPUT);
    digitalWrite(layers[i], LOW);
  }
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  lowerp();

  simple_states_gen();
  combination();
}

void loop() {
  lowerp();
  // for (int i = 0; i < 8; i++) {
  //   set_state(i);
  //   delay(200);
  // }
}

void set_state(int st) {
  if (st == -10){
    lowerp();
    return;
  }
  digitalWrite(layers[0], states[st][0]);
  digitalWrite(layers[1], states[st][1]);
  digitalWrite(columns[0], states[st][2]);
  digitalWrite(columns[1], states[st][3]);
  digitalWrite(columns[2], states[st][4]);
  digitalWrite(columns[3], states[st][5]);
}

void simple_states_gen() {
  int counter = 0;
  // 1
  int row[6] = { 1, 0, 1, 1, 1, 1 };
  for (int i = 0; i < 2; i++) {
    row[0] = !row[0];
    row[1] = !row[1];

    for (int j = 0; j < 4; j++) {
      // 2+ потому что первые два элемента это слои
      row[2 + j] = LOW;
      // memcpy(states[i * 4 + j], row, sizeof(row));
      for (int ma = 0; ma < 6; ++ma) {
        states[counter][ma] = row[ma];
      }
      row[2 + j] = HIGH;
      ++counter;
    }
  }
  // for (int i = 0; i < 8; ++i) {
  //   for (int j = 0; j < 6; ++j) {
  //     Serial.print(states[i][j]);
  //     Serial.print(" ");
  //   }
  //   Serial.println("------");
  // }
}

void combination() {
  int n = 8;
  for (int i = 1; i < (1 << n); i++) {
    int state_indexes[8] = {-10,-10,-10,-10,-10,-10,-10,-10};
    int counter = 0;
    // for (int timeer = 0; timeer < 100; timeer++){
      for (int j = 0; j < n; j++) {
        if ((1 << j) & i) {
          // set_state(j);
          // Serial.print(j);
          // Serial.print(" ");
          // delay(1);
          // lowerp();
          state_indexes[counter] = j;
          counter++;
        }
      }
    
    for (int timeer = 0; timeer < 100; timeer++){
      for (int st=0;st < 8; st++){
        // Serial.print(state_indexes[st]);
        // Serial.print(" ");
        set_state(state_indexes[st]);
        delay(1);
        lowerp();
      }
    } 
    // delay(500);
    Serial.println(" ");
  }
}
