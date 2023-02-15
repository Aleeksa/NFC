#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int buzzerPin = 6;
int greenLedPin = 7;
int redLedPin = 8;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.begin(16, 2);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  lcd.print("Ready to scan...");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card ID:");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      lcd.print(mfrc522.uid.uidByte[i], HEX);
    }
    lcd.setCursor(0, 1);
    if (mfrc522.uid.uidByte[0] == 0xAA && mfrc522.uid.uidByte[1] == 0xBB && mfrc522.uid.uidByte[2] == 0xCC && mfrc522.uid.uidByte[3] == 0xDD) {
      lcd.print("Access granted");
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      tone(buzzerPin, 1000, 500);
      delay(1000);
      noTone(buzzerPin);
      digitalWrite(greenLedPin, LOW);
    } else {
      lcd.print("Access denied");
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
      tone(buzzerPin, 1000, 500);
      delay(500);
      tone(buzzerPin, 1000, 500);
      delay(3500);
      noTone(buzzerPin);
      digitalWrite(redLedPin, LOW);
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready to scan...");
  }
}
