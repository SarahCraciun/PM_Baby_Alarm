#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int redLedPin = 2;
const int blueLedPin = 4;
const int buzzerPin = 18;
uint8_t serverMAC[6] = {0xa0, 0xa3, 0xb3, 0x96, 0x5c, 0xee}; 

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_ALERT", true); // Numele dispozitivului Bluetooth
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Oprește toate ieșirile la început
  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(buzzerPin, HIGH);

  // Conectează-te la serverul Bluetooth ESP32_MIC
  while (!SerialBT.connect(serverMAC)) {
    Serial.println("Connecting to ESP32_MIC...");
    delay(1000);
  }

  Serial.println("Connected to ESP32_MIC");
}

void loop() {
  if (SerialBT.connected()) {
    if (SerialBT.available()) {
      char receivedChar = SerialBT.read();
      if (receivedChar == 'X') {
        // Aprinde LED-urile și suna buzzer-ul
        digitalWrite(redLedPin, HIGH);
        digitalWrite(blueLedPin, LOW);
        tone(buzzerPin, 349); // Sunet la 1000 Hz
        delay(500); // 0.5 secunde

        digitalWrite(redLedPin, LOW);
        digitalWrite(blueLedPin, HIGH);
        tone(buzzerPin, 262); // Sunet la 1500 Hz
        delay(500); // 0.5 secunde

        // Oprește buzzer-ul si ledurile între cicluri
        digitalWrite(blueLedPin, LOW);
        noTone(buzzerPin);
        digitalWrite(buzzerPin, HIGH);
      }
    }
  } else {
    Serial.println("Connection lost. Reconnecting...");
    while (!SerialBT.connect(serverMAC)) {
      Serial.println("Reconnecting to ESP32_MIC...");
      delay(1000);
    }
    Serial.println("Reconnected to ESP32_MIC");
  }
}