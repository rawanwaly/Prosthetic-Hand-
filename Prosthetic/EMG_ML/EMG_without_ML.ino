#include <Servo.h>

// Define Servo Objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

//define Emg sensor
int EMGPin = A0;
int EMGVal = 0;

void setup() {
  // Set up serial port
  Serial.begin(115200);

  // Attach Servo Motors
  servo1.attach(6);
  servo2.attach(5);
  servo3.attach(12);
  servo4.attach(10);
  servo5.attach(9);
}

void loop() {
  //Reading EmgSensor
  EMGVal = analogRead(EMGPin);
  Serial.println(EMGVal);
  delay(500);
  if (EMGVal > 150) {
    Serial.println("close");
    servo1.write(0);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);
    servo5.write(180);
    delay(1000);
  } else if (EMGVal < 35) {
    Serial.println("rest");
    servo1.write(130);
    servo2.write(30);
    servo3.write(40);
    servo4.write(100);
    servo5.write(0);
    delay(1000);
  } else if (60 < EMGVal < 120) {
    Serial.println("point");
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);
    servo5.write(180);

    delay(1000);
  }
}
