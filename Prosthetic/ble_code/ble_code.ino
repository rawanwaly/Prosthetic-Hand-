#include <ArduinoBLE.h>  // Bluetooth Library
#include <Servo.h>

BLEService customService("180C");
BLEByteCharacteristic switchCharacteristic("2A58", BLERead | BLEWrite);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;


void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1)
      ;
  }

  // Attach Servo Motors
  servo1.attach(6);
  servo2.attach(5);
  servo3.attach(12);
  servo4.attach(10);
  servo5.attach(9);


  BLE.setLocalName("Nano 33 BLE Sense");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(switchCharacteristic);
  switchCharacteristic.writeValue(0);
  BLE.addService(customService);
  BLE.advertise();
  Serial.println("Bluetooth device is now active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      switchCharacteristic_recieve();
      delay(1000);
    }
  }
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}

void switchCharacteristic_recieve() {
  int number = switchCharacteristic.value();
  if (switchCharacteristic.written()) {
    Serial.print("\nNumber: ");
    Serial.println(number);
    switch (switchCharacteristic.value()) {  // any value other than 0
      case 1:
        Serial.println("open");
        servo1.write(130);
        servo2.write(30);
        servo3.write(40);
        servo4.write(100);
        servo5.write(0);
        break;
      case 2:
        Serial.println("close");
        servo1.write(0);
        servo2.write(180);
        servo3.write(180);
        servo4.write(180);
        servo5.write(180);
        break;
      case 3:
        Serial.println("point");
        servo1.write(180);
        servo2.write(180);
        servo3.write(180);
        servo4.write(180);
        servo5.write(180);

        break;
      case 4:
        Serial.println("peace");
        servo1.write(180);
        servo2.write(180);
        servo3.write(180);
        servo4.write(180);
        servo5.write(0);
        break;
        case 05:
        Serial.println("ok");
        servo1.write(0);
        servo2.write(0);
        servo3.write(40);
        servo4.write(100);
        servo5.write(0);
        break;
      default:
        Serial.println("No gesture");
        break;
    }
  }
}