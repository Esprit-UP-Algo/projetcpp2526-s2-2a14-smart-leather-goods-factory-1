// ============================================================
//  BARRIERE AUTOMATIQUE - Version validation (sans integration)
//  Auteur    : A completer
//  Date      : 2026
//  Note      : Ce code sera remplace lors de l'integration
//              avec le module de saisie du collegue.
//              La reference est saisie via le Serial Monitor.
// ============================================================

#include <Servo.h>

// ─── Brochage ────────────────────────────────────────────────
const int PIN_SERVO = 9;

// ─── Angles de la barriere ───────────────────────────────────
const int ANGLE_FERME  = 0;    // Barriere fermee (horizontale)
const int ANGLE_OUVERT = 90;   // Barriere ouverte (verticale)

// ─── Timing ──────────────────────────────────────────────────
const int DUREE_OUVERT  = 3000; // Duree ouverture en ms
const int VITESSE_MS    = 15;   // Delai entre chaque degre (mouvement fluide)

// ─── References valides ──────────────────────────────────────
//     Modifie ces valeurs selon ton projet
String refsValides[] = {"REF001", "REF002", "REF003"};
const int NB_REFS = 3;

// ─── Variables internes ──────────────────────────────────────
Servo    barriere;
String   inputBuffer  = "";
bool     barriereOuverte = false;
long     tempsOuverture  = 0;

// ============================================================
//  FONCTIONS
// ============================================================

// Mouvement fluide degre par degre
void allerA(int angleActuel, int angleCible) {
  if (angleActuel < angleCible) {
    for (int a = angleActuel; a <= angleCible; a++) {
      barriere.write(a);
      delay(VITESSE_MS);
    }
  } else {
    for (int a = angleActuel; a >= angleCible; a--) {
      barriere.write(a);
      delay(VITESSE_MS);
    }
  }
}

// Verifie si la reference saisie est dans la liste
bool refValide(String ref) {
  ref.trim();
  ref.toUpperCase();
  for (int i = 0; i < NB_REFS; i++) {
    if (ref == refsValides[i]) return true;
  }
  return false;
}

// Ouvre la barriere (mouvement fluide)
void ouvrirBarriere() {
  Serial.println("  >> Ouverture en cours...");
  allerA(ANGLE_FERME, ANGLE_OUVERT);
  barriereOuverte  = true;
  tempsOuverture   = millis();
  Serial.println("  >> Barriere OUVERTE");
  Serial.print  ("  >> Fermeture automatique dans ");
  Serial.print  (DUREE_OUVERT / 1000);
  Serial.println(" secondes.");
}

// Ferme la barriere (mouvement fluide)
void fermerBarriere() {
  Serial.println("  >> Fermeture en cours...");
  allerA(ANGLE_OUVERT, ANGLE_FERME);
  barriereOuverte = false;
  Serial.println("  >> Barriere FERMEE");
  Serial.println("----------------------------------------");
  Serial.println("Entrez une reference :");
}

// Affiche la liste des references valides
void afficherRefs() {
  Serial.println("  References acceptees :");
  for (int i = 0; i < NB_REFS; i++) {
    Serial.print("    - ");
    Serial.println(refsValides[i]);
  }
}

// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(9600);

  barriere.attach(PIN_SERVO);
  barriere.write(ANGLE_FERME);
  delay(500);

  Serial.println("========================================");
  Serial.println("   SYSTEME BARRIERE - Mode validation  ");
  Serial.println("========================================");
  afficherRefs();
  Serial.println("----------------------------------------");
  Serial.println("Entrez une reference et appuyez Entree :");
}

// ============================================================
//  LOOP
// ============================================================
void loop() {

  // -- Fermeture automatique apres DUREE_OUVERT ---------------
  if (barriereOuverte && (millis() - tempsOuverture >= DUREE_OUVERT)) {
    fermerBarriere();
  }

  // -- Lecture Serial -----------------------------------------
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {

        // Affiche la reference recue
        Serial.println("----------------------------------------");
        Serial.print("Reference recue : [");
        Serial.print(inputBuffer);
        Serial.println("]");

        // Validation
        if (refValide(inputBuffer)) {
          Serial.println("[OK] Reference VALIDE");
          if (!barriereOuverte) {
            ouvrirBarriere();
          } else {
            Serial.println("  >> Barriere deja ouverte, timer reinitialise.");
            tempsOuverture = millis(); // Repart le compteur
          }
        } else {
          Serial.println("[REFUS] Reference INCONNUE - Acces refuse.");
          Serial.println("----------------------------------------");
          Serial.println("Entrez une reference :");
        }

        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}
