#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

// Define IR sensor and LED pins for each pair
const int irSensorPins[] = {A0, A1, A2, A3};
const int ledPins[] = {2, 3, 4, 5};

#define SS_PIN 10 // RFID Slave Select Pin
#define RST_PIN 9  // RFID Reset Pin

#define IR_SENSOR_PIN 8 // IR Sensor Signal Pin
#define SERVO_RFID_PIN 6 // Servo Motor Control Pin for RFID
#define SERVO_IR_PIN 7   // Servo Motor Control Pin for IR

Servo servoRFID;
Servo servoIR;
int ledPin = 12;
int buzzerPin = 11;

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the UID of authorized RFID cards
#define RFID_CARD_1_UID {0xB3, 0xB4, 0xA5, 0x1A}
// Add more card UIDs as needed

void setup() {
  Serial.begin(9600);

  // Set up IR sensor pins as input
  pinMode(irSensorPins[0], INPUT);
  pinMode(irSensorPins[1], INPUT);
  pinMode(irSensorPins[2], INPUT);
  pinMode(irSensorPins[3], INPUT);

  // Set up LED pins as output
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();
  servoRFID.attach(SERVO_RFID_PIN);
  servoIR.attach(SERVO_IR_PIN);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Read IR sensor value for sensor 1
  int irValue1 = digitalRead(irSensorPins[0]);

  // If an object is detected, turn on the LED for sensor 1
  if (irValue1 == LOW) {
    digitalWrite(ledPins[0], HIGH);
    Serial.println("Object detected on sensor 1");
  } else {
    digitalWrite(ledPins[0], LOW);
  }

  // Read IR sensor value for sensor 2
  int irValue2 = digitalRead(irSensorPins[1]);

  // If an object is detected, turn on the LED for sensor 2
  if (irValue2 == LOW) {
    digitalWrite(ledPins[1], HIGH);
    Serial.println("Object detected on sensor 2");
  } else {
    digitalWrite(ledPins[1], LOW);
  }

  // Read IR sensor value for sensor 3
  int irValue3 = digitalRead(irSensorPins[2]);

  // If an object is detected, turn on the LED for sensor 3
  if (irValue3 == LOW) {
    digitalWrite(ledPins[2], HIGH);
    Serial.println("Object detected on sensor 3");
  } else {
    digitalWrite(ledPins[2], LOW);
  }

  // Read IR sensor value for sensor 4
  int irValue4 = digitalRead(irSensorPins[3]);

  // If an object is detected, turn on the LED for sensor 4
  if (irValue4 == LOW) {
    digitalWrite(ledPins[3], HIGH);
    Serial.println("Object detected on sensor 4");
  } else {
    digitalWrite(ledPins[3], LOW);
  }

  // Look for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Compare the card UID with authorized UIDs
    if (compareCardUID(mfrc522.uid.uidByte)) {
      Serial.println("Authorized card detected!");

      // Perform actions when an authorized RFID card is detected
      openRFIDDoor();
      delay(5000); // Wait for 5 seconds
      closeRFIDDoor();
    } else {
      Serial.println("Unauthorized card detected!");
      closeRFIDDoor();
    }

    // Halt the RFID module
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  // IR sensor detection
  if (digitalRead(IR_SENSOR_PIN) == LOW) {
    Serial.println("Object detected by IR sensor");

    // Perform actions when an object is detected by the IR sensor
    openIRDoor();
    delay(3000); // Wait for 5 seconds
    closeIRDoor();
  }
  if (digitalRead(IR_SENSOR_PIN) == HIGH) {
    Serial.println("Object NOT detected by IR sensor");

    // Perform actions when an object is detected by the IR sensor
    closeIRDoor();
  }

  // Add a delay to avoid rapid toggling of LED states
  delay(100);
}

void openRFIDDoor() {
  servoRFID.write(90); // Move the RFID servo to the open position (90 degrees)
  tone(buzzerPin, 1000, 500);
  digitalWrite(ledPin, LOW);
  delay(5000); // Keep the door open for 5 seconds
  digitalWrite(ledPin, HIGH);
  servoRFID.write(0); // Close the door
}

void closeRFIDDoor() {
  servoRFID.write(0); // Move the RFID servo to the closed position (0 degrees)
  tone(buzzerPin, 200, 2000);
}

void openIRDoor() {
  servoIR.write(90); // Move the IR servo to the open position (90 degrees)
  digitalWrite(ledPin, HIGH);
  delay(3000); // Keep the door open for 5 seconds
  digitalWrite(ledPin, LOW);
}

void closeIRDoor() {
  servoIR.write(0); // Move the IR servo to the closed position (0 degrees)
}

bool compareCardUID(byte* cardUID) {
  // Compare the provided card UID with authorized UIDs
  byte authorizedCard1[] = RFID_CARD_1_UID;
  // Add more card UIDs as needed

  for (int i = 0; i < mfrc522.uid.size; i++) {
    if (cardUID[i] != authorizedCard1[i]) {
      return false; // UID doesn't match
    }
  }

  return true; // UID matches
}
