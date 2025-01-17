#include <EZButton.h>

#define BTN_1_PIN 6
#define BTN_1 0

void ReadButtons(bool *states, int num) {
  //Read all button states however you want
  states[BTN_1] = !digitalRead(BTN_1_PIN);
}

EZButton _ezb(1, ReadButtons, 1000, 200, 15);

void setup() {
  //initialize pins
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  //subscribe to needed events
  _ezb.Subscribe(BTN_1, Btn1Pressed, PRESSED);
  _ezb.Subscribe(BTN_1, Btn1Released, RELEASED);
  _ezb.Subscribe(BTN_1, Btn1Hold, HOLD);
  //Or you can use lambda functions
  // _ezb.Subscribe(BTN_1, [](){Serial.println("pressed");}, PRESSED);
  
  //You can also get the button index in the event
  _ezb.Subscribe(BTN_1, Btn1HoldReleased, HOLD_RELEASED);
  //Or you can use lambda functions
  // _ezb.Subscribe(BTN_1, [](int index){Serial.println(index);}, HOLD);
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

void Btn1HoldReleased(int index) {
  for (int i = 0; i < 6; i++) {
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    delay(50);
  }
  digitalWrite(LED_BUILTIN, LOW);
}