#include <Servo.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <SPI.h>

#define SS_PIN D4 // RFID Slave Select Pin
#define RST_PIN D3 // RFID Reset Pin
#define SERVO_RFID_PIN D8 // Servo Motor Control Pin for RFID

Servo servoRFID;
// Servo servoIR;
int ledPin = D2;

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the UID of authorized RFID cards
// #define RFID_CARD_1_UID {0xB3, 0xB4, 0xA5, 0x1A}
// Add more card UIDs as needed

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  servoRFID.attach(SERVO_RFID_PIN);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  WiFi.begin("POCO", "9691073541");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
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
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(ledPin, LOW);
      closeRFIDDoor();
    }

    // Halt the RFID module
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

}

void openRFIDDoor() {
  servoRFID.write(120); // Move the RFID servo to the open position (90 degrees)
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  servoRFID.write(0); // Close the door
}

void closeRFIDDoor() {
  servoRFID.write(0); // Move the RFID servo to the closed position (0 degrees)
}

bool compareCardUID(byte* cardUID) {
  // Add authorized UIDs here
  byte authorizedCard1[] = {0xB3, 0xB4, 0xA5, 0x1A};
  byte authorizedCard2[] = {0x2C, 0x30, 0x47, 0x49};
  byte authorizedCard3[] = {0xF3, 0x73, 0x1D, 0x11};
  // Add more authorized UIDs as needed

  for (int i = 0; i < 4; i++) {
    if (
      cardUID[i] != authorizedCard1[i] &&
      cardUID[i] != authorizedCard2[i] &&
      cardUID[i] != authorizedCard3[i]
      // Add more conditions for additional UIDs
    ) {
      return false; // UID doesn't match
    }
  }

  return true; // UID matches
}

