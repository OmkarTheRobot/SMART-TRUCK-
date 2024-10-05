/* Author : Omkar Dilip Savaratkar*/

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address
#include <SPI.h>
#include <MFRC522.h>
int motor1 = 6;
int motor2 = 7;
int buzzer = 5;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
#include <SoftwareSerial.h> 
SoftwareSerial blue(2, 3);
#include <Wire.h>

void setup() 
{
  lcd.begin();
  lcd.backlight(); 
  /*Serial.begin(115200);
  blue.begin(9600); 
  SPI.begin();  // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  */
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Welcome screen
  lcd.setCursor(3, 0);  // Centered 'Welcome to'
  lcd.print("Welcome to");
  lcd.setCursor(2, 1);  // Centered 'Smart Truck'
  lcd.print("Smart Truck");
  delay(2000);  // Hold the welcome message for 2 seconds

  lcd.clear();  // Clear the screen after the welcome message
  lcd.setCursor(2, 0);  // Centered 'Project by:'
  lcd.print("Project by:");
  delay(2000);  // Hold the names for 2 seconds

  // Display full names one by one
  lcd.clear();
  lcd.setCursor(0, 0);  // Fully display 'Suraj R. Bedkyale'
  lcd.print("Suraj R.B");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);  // Fully display 'Ajay J. Banasavade'
  lcd.print("Ajay J.B");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);  // Fully display 'Dhiraj P. Davar'
  lcd.print("Dhiraj P.D");
  delay(2000);
  
  lcd.clear();  // Clear the screen after displaying the names

  // Initial prompt to scan card
  lcd.setCursor(0, 0);
  lcd.print("Please Scan");
  lcd.setCursor(0, 1);
  lcd.print("LICENSE CARD");
  delay(1000);
}

void loop()
{
  Serial.begin(115200);
  blue.begin(9600); 
  SPI.begin();  // Initiate SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();


  digitalWrite(buzzer, LOW);

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) 
  {
  
    return; // If no new card, continue looping
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return; // If no card read, continue looping
  }
  
  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  // Access validation for different cards
  if (content.substring(1) == "23 43 4A FE")  // Card 1: Ajay
  {
    accessGranted("AJAY");
  }
  else if (content.substring(1) == "D3 9F 41 FE")  // Card 2: Suraj
  {
    accessGranted("SURAJ");
  }
  else if (content.substring(1) == "1A BF 95 1A")  // Card 3: Dhiraj
  {
    accessGranted("DHIRAJ");
  }
  else if (content.substring(1) == "24 5C B9 9P")  // Card 4: Prathamesh
  {
    accessGranted("PRATHAMESH");
  }
  else 
  {
    accessDenied();
  }
}

void accessGranted(String name) {
  Serial.println("Authorized access");
  lcd.setCursor(0, 0);
  lcd.print("Authorized access");
  lcd.setCursor(4, 1);  // Center the name
  lcd.print(name);
  digitalWrite(buzzer, HIGH);
  lcd.clear();
  delay(3000); 
  digitalWrite(buzzer, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Success verify");
  lcd.setCursor(0, 1);
  lcd.print("Driving Licence");
  delay(2000); // Delay before displaying name
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("License Holder:");
  lcd.setCursor(0, 1);
  lcd.print(name);
  delay(1000);
  
  // Start the motors
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, HIGH);
  delay(5000);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  delay(100);
  lcd.clear();
  delay(100);
  lcd.setCursor(1, 0);
  lcd.print("Please Scan");
  lcd.setCursor(0, 1);
  lcd.print("LICENSE CARD");
  
}

void accessDenied() {
  Serial.println("Access denied");
  lcd.setCursor(2, 0);
  lcd.print("Access denied");
  lcd.setCursor(3, 1);  // Center 'Unknown Card'
  lcd.print("Unknown Card");
  delay(3000);
  lcd.clear();
  delay(1000);
  lcd.setCursor(1, 0);
  lcd.print("Please Scan");
  lcd.setCursor(0, 1);
  lcd.print("LICENSE CARD");
}
