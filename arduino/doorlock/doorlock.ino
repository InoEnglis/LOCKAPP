#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 // define green LED pin
#define LED_R 4 // define red LED
#define RELAY 3 // relay pin
#define BUZZER 2 // buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

bool doorLocked = true;

void setup() {
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin(); // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, LOW);
  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() {
  // Check for serial input
  if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.print("Received command: ");
    Serial.println(command);
    if (command == 'U') {
      lockDoor(); // Swapped from unlockDoor() to lockDoor()
    } else if (command == 'L') {
      unlockDoor(); // Swapped from lockDoor() to unlockDoor()
    }
  }

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "33 FC AE C9") // change here the UID of the card/cards that you want to give access
  {
    lockDoor(); // Swapped from unlockDoor() to lockDoor()
  } else {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}

void unlockDoor() {
  if (!doorLocked) { // Condition swapped
    Serial.println("Door unlocked"); // Message changed
    Serial.println();
    digitalWrite(RELAY, LOW); // Swapped from HIGH to LOW
    digitalWrite(LED_G, LOW); // Turn off green LED
    tone(BUZZER, 800); // Lower pitch for unlock
    delay(DENIED_DELAY); // Duration of buzzer sound
    noTone(BUZZER); // Stop buzzer sound
    digitalWrite(LED_R, HIGH); // Turn on red LED after delay
    delay(ACCESS_DELAY);
    digitalWrite(LED_R, LOW); // Turn off red LED after delay
    doorLocked = true;
  }
}

void lockDoor() {
  if (doorLocked) { // Condition swapped
    Serial.println("Authorized access"); // Message changed
    Serial.println();
    digitalWrite(RELAY, HIGH); // Swapped from LOW to HIGH
    digitalWrite(LED_G, HIGH); // Turn on green LED
    tone(BUZZER, 2000); // Higher pitch for lock
    delay(ACCESS_DELAY);
    digitalWrite(LED_G, LOW); // Turn off green LED after delay
    noTone(BUZZER); // Stop buzzer sound
    doorLocked = false;
  }
}
