#include <EZButton.h>

#define BTN_1_PIN 6
#define BTN_1 0

void ReadButtons(bool *states, int num) {
  //Read all button states however you want
  states[BTN_1] = !digitalRead(BTN_1_PIN);
}

EZButton _ezb(1, ReadButtons, 1000, 200);

void setup() {
  //initialize pins
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  //subscribe to needed events
  _ezb.Subscribe(BTN_1, Btn1Pressed, PRESSED);
  _ezb.Subscribe(BTN_1, Btn1Released, RELEASED);
  _ezb.Subscribe(BTN_1, Btn1Hold, HOLD);
  _ezb.Subscribe(BTN_1, Btn1HoldReleased, HOLD_RELEASED);
}

void loop() {
  //EZButton loop
  _ezb.Loop();
}

void Btn1Pressed() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void Btn1Released() {
  digitalWrite(LED_BUILTIN, LOW);
}

bool state = true;
void Btn1Hold() {
  state = !state;
  digitalWrite(LED_BUILTIN, state);
}

void Btn1HoldReleased() {
  for (int i = 0; i < 6; i++) {
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    delay(50);
  }
  digitalWrite(LED_BUILTIN, LOW);
}