#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

// Utilisation du nom de variable "rfid" comme votre binôme
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresse I2C 0x27, écran 16 colonnes, 2 lignes

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  En attente   ");
  lcd.setCursor(0, 1);
  lcd.print("   de badge... ");

  // Signal envoyé à Qt (comme dans le code de votre binôme)
  Serial.println("RFID_READY");
}

void loop() {
  // ---------------------------------------------------------
  // 1. LECTURE DE LA CARTE RFID (Code exact de votre binôme)
  // ---------------------------------------------------------
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";

    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) {
        uid += "0";
      }
      uid += String(rfid.uid.uidByte[i], HEX);
    }

    uid.toUpperCase();

    // Envoi de l'UID à Qt via le port série
    Serial.println(uid);
    
    // Affichage d'attente sur l'écran pendant que Qt cherche
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recherche...");

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(800); 
  }

  // ---------------------------------------------------------
  // 2. RÉCEPTION DES DONNÉES DEPUIS QT (Votre scénario)
  // ---------------------------------------------------------
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim(); // Enlever les espaces ou retours à la ligne

    if (data == "NOT_FOUND") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Badge Inconnu!");
      delay(3000);
    } else if (data == "DB_ERROR") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Erreur Serveur!");
      delay(3000);
    } else if (data != "RFID_READY") { 
      // Format attendu: "Nom;Poste" (ex: "Aziz Benali;Informatique")
      int separatorIndex = data.indexOf(';');
      if (separatorIndex != -1) {
        String nom = data.substring(0, separatorIndex);
        String poste = data.substring(separatorIndex + 1);

        // Affichage Phase 1: Identité
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(nom);
        lcd.setCursor(0, 1);
        lcd.print(poste);
        
        delay(3000); // Attendre 3 secondes

        // Affichage Phase 2: Direction
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Dirige vers");
        lcd.setCursor(0, 1);
        lcd.print("Parking " + poste);
        
        delay(4000); // Attendre 4 secondes
      }
    }
    
    // Remise à l'état initial
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  En attente   ");
    lcd.setCursor(0, 1);
    lcd.print("   de badge... ");
  }
}
