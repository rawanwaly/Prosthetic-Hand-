#include "model.h"

ProstheticHand::ProstheticHand(int servoPin1, int servoPin2, int emgPin) {
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  this->emgPin = emgPin;
  emgThreshold = 500;
  servoPosOpen = 0;
  servoPosClose = 180;
}

void ProstheticHand::begin() {
  // Load TensorFlow Lite model
  tflite = TfLiteESP32::loadModelFromFlash("/model.tflite");
  if (tflite == nullptr) {
    Serial.println("Failed to load model!");
    while (1);
  }

  // Initialize TensorFlow Lite interpreter
  TfLiteESP32::initializeInterpreter(tflite);

  // Allocate memory for input tensor
  TfLiteTensor* inputTensor = TfLiteESP32::getInputTensor(tflite, 0);
  TfLiteESP32::allocateTensor(inputTensor);

  // Enable IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void ProstheticHand::update() {
  // Read EMG sensor value
  int emgValue = analogRead(emgPin);

  // Normalize EMG value
  float normalizedEmg = normalizeEMG(emgValue);

  // Run inference
  TfLiteTensor* inputTensor = TfLiteESP32::getInputTensor(tflite, 0);
  TfLiteTensor* outputTensor = TfLiteESP32::getOutputTensor(tflite, 0);
  float* inputData = TfLiteESP32::inputDataPtr(inputTensor);
  inputData[0] = normalizedEmg;
  TfLiteESP32::invoke();

  // Get prediction result
  float prediction = outputTensor->data.f[0];

  // Move prosthetic hand based on prediction
  if (prediction > 0.5) {
    openHand();
  } else {
    closeHand();
  }

  delay(100);
}

void ProstheticHand::openHand() {
  servo1.write(servoPosOpen);
  servo2.write(servoPosClose);
}

void ProstheticHand::closeHand() {
  servo1.write(servoPosClose);
  servo2.write(servoPosOpen);
}

float ProstheticHand::normalizeEMG(int emgValue) {
  return map(emgValue, 0, 1023, 0, 1);
}

