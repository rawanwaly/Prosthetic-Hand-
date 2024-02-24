
#include "model.h"

const int servoPin1 = 9;
const int servoPin2 = 10;
const int emgPin = A0;

ProstheticHand hand(servoPin1, servoPin2, emgPin);

void setup() {
  Serial.begin(115200);
  hand.begin();
}

void loop() {
  hand.update();
}
