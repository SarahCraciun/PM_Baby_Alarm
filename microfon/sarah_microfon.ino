#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int micPin = 35;
const int threshold = 100;

int read_mic_value() {
  int sum = 0;
  for (int i = 0; i < 300; i++) {
    sum += analogRead(micPin);
  }
  sum /= 300;
  return sum;
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_MIC"); // Numele dispozitivului Bluetooth
  pinMode(micPin, INPUT);

  // Așteaptă până când un client se conectează
  Serial.println("Waiting for client connection...");
  while (!SerialBT.hasClient()) {
    delay(1000);
    Serial.println("Still waiting for client connection...");
  }
  Serial.println("Client connected!");
}

void loop() {
  if (SerialBT.hasClient()) {
    int micValue = read_mic_value();
    Serial.print("Mic Value: ");
    Serial.println(micValue);

    if (micValue > threshold) {
      SerialBT.print("X");
    }

    delay(1000); // 1 secundă
  } else {
    // Oprește trimiterea de date dacă clientul se deconectează
    Serial.println("Client disconnected, waiting for new connection...");
    while (!SerialBT.hasClient()) {
      delay(1000);
      Serial.println("Still waiting for client connection...");
    }
    Serial.println("Client reconnected!");
  }
}