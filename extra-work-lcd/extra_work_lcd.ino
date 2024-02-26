#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int sensor1 = A1;  // IR sensor 1
const int sensor2 = A0;  // IR sensor 2
const int sensor3 = A2;  // IR sensor 3
const int sensor4 = A3;  // IR sensor 4

const int trigPin = 9;
const int echoPin = A4;
const int greenLedPin = 7;
const int yellowLedPin = 1;
const int redLedPin = 8;
const int buzzerPin = 13;

const int led1 = 6;
const int led2 = A5;
const int led3 = 10;


void setup() {
  lcd.begin(16, 2);  // initialize the lcd
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  // Sensor code
  int status1 = digitalRead(sensor1);
  int status2 = digitalRead(sensor2);
  int status3 = digitalRead(sensor3);
  int status4 = digitalRead(sensor4);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.print((status2 == LOW) ? "full" : "Empty");

  lcd.setCursor(8, 0);
  lcd.print("2:");
  lcd.print((status1 == LOW) ? "full" : "Empty");

  lcd.setCursor(0, 1);
  lcd.print("3:");
  lcd.print((status3 == LOW) ? "full" : "Empty");

  lcd.setCursor(8, 1);
  lcd.print("4:");
  lcd.print((status4 == LOW) ? "full" : "Empty");

  delay(500);

  if (status1 == LOW) {
    digitalWrite(led2, LOW); // Turn on the LED
  } else {
    digitalWrite(led2, HIGH); // Turn off the LED
  }

  if (status2 == LOW) {
    digitalWrite(led1, LOW); // Turn on the LED
  } else {
    digitalWrite(led1, HIGH); // Turn off the LED
  }

  if (status3 == LOW) {
    digitalWrite(led3, LOW); // Turn on the LED
  } else {
    digitalWrite(led3, HIGH); // Turn off the LED
  }

  // Ultrasonic sensor code
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // LED and Buzzer control based on distance
  if (distance > 10) {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  } else if (distance >= 4) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  } else if (distance > 1) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(500);  // Adjust the delay as needed
    digitalWrite(buzzerPin, LOW);
  } else {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    // digitalWrite(redLedPin, HIGH);
    // digitalWrite(buzzerPin, HIGH);
    delay(1000);  // Adjust the delay as needed
    digitalWrite(buzzerPin, LOW);
  }

  delay(1000);
}
