#include "DHT.h"

// --- CONFIGURATION BRANCHEMENT ---
#define DHTPIN1 2     // 1er capteur bleu (DHT11) sur Digital D2
#define DHTPIN2 3     // 2eme capteur bleu (DHT11) sur Digital D3
#define DHTTYPE DHT11

// --- CONFIGURATION MACHINES ---
// REMPLACE CES REFERENCES PAR CELLES DE TES MACHINES DANS L'APPLI QT
const String REF_MACHINE_1 = "mac5"; 
const String REF_MACHINE_2 = "mac60";

const float SEUIL = 27.0; // Seuil bas pour tester facilement

// --- INITIALISATION ---
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

bool alerte1 = false;
bool alerte2 = false;

void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Systeme de surveillance pret !");
}

void loop() {
  // --- SURVEILLANCE MACHINE 1 ---
  float t1 = dht1.readTemperature();
  if (!isnan(t1)) {
    Serial.print("Temp1: "); Serial.println(t1); // DEBUG: Affiche la vraie temperature
    if (t1 > SEUIL && !alerte1) {
      Serial.println("ALERTE:" + REF_MACHINE_1);
      alerte1 = true;
    } else if (t1 < SEUIL - 2.0) {
      alerte1 = false; 
    }
  } else {
    Serial.println("Erreur: DHT1 debranche ou defaillant !"); // DEBUG: Alerte erreur de cablage
  }

  // --- SURVEILLANCE MACHINE 2 ---
  float t2 = dht2.readTemperature();
  if (!isnan(t2)) {
    Serial.print("Temp2: "); Serial.println(t2); // DEBUG 
    if (t2 > SEUIL && !alerte2) {
      Serial.println("ALERTE:" + REF_MACHINE_2);
      alerte2 = true;
    } else if (t2 < SEUIL - 2.0) {
      alerte2 = false;
    }
  } else {
    Serial.println("Erreur: DHT2 debranche ou defaillant !"); // DEBUG
  }

  // Allume la LED de l'Arduino si l'une des machines surchauffe
  digitalWrite(LED_BUILTIN, (alerte1 || alerte2) ? HIGH : LOW);

  delay(2000); // Mise a jour toutes les 2 secondes
}
