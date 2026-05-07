#include "DHT.h"

// --- CONFIGURATION BRANCHEMENT ---
#define DHTPIN1 2     // 1er capteur bleu (DHT11) sur Digital D2
#define DHTPIN2 3     // 2eme capteur bleu (DHT11) sur Digital D3
#define DHTTYPE DHT11

// REMPLACE CES REFERENCES PAR CELLES DE TES MACHINES DANS L'APPLI QT
const String REF_MACHINE_1 = "mac5"; 
const String REF_MACHINE_2 = "mac60";

const float SEUIL = 27.0; // Seuil bas pour tester facilement


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
    if (t1 > SEUIL && !alerte1) {
      Serial.println("ALERTE:" + REF_MACHINE_1);
      alerte1 = true;
    } else if (t1 < SEUIL - 2.0) {
      alerte1 = false; 
    }
  }

  // --- SURVEILLANCE MACHINE 2 ---
  float t2 = dht2.readTemperature();
  if (!isnan(t2)) {
    if (t2 > SEUIL && !alerte2) {
      Serial.println("ALERTE:" + REF_MACHINE_2);
      alerte2 = true;
    } else if (t2 < SEUIL - 2.0) {
      alerte2 = false;
    }
  }


  digitalWrite(LED_BUILTIN, (alerte1 || alerte2) ? HIGH : LOW);

  delay(2000); 
}
