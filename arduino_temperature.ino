/*
 * Smart Leather Factory - Machine Monitoring
 * 2 Temperature Sensors Alert System
 */

const int sensor1Pin = A0; // Sensor for Machine 1
const int sensor2Pin = A1; // Sensor for Machine 2

// CONFIGURATION - CHANGE REFS TO MATCH YOUR DATABASE
const String MACHINE_1_REF = "MAC-001"; 
const String MACHINE_2_REF = "MAC-002";

const float THRESHOLD = 40.0; // Alert threshold in Celsius

bool alertSent1 = false;
bool alertSent2 = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Read A0
  int val1 = analogRead(sensor1Pin);
  float temp1 = (val1 * 5.0 * 100.0) / 1024.0; // LM35 formula: 10mV per degree

  // Read A1
  int val2 = analogRead(sensor2Pin);
  float temp2 = (val2 * 5.0 * 100.0) / 1024.0;

  // Monitor Machine 1
  if (temp1 > THRESHOLD) {
    if (!alertSent1) {
      Serial.print("ALERTE:");
      Serial.println(MACHINE_1_REF);
      alertSent1 = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  } else if (temp1 < THRESHOLD - 5.0) {
    alertSent1 = false; // Reset alert when cooled down
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Monitor Machine 2
  if (temp2 > THRESHOLD) {
    if (!alertSent2) {
      Serial.print("ALERTE:");
      Serial.println(MACHINE_2_REF);
      alertSent2 = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  } else if (temp2 < THRESHOLD - 5.0) {
    alertSent2 = false;
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Periodic status (optional for debugging)
  /*
  Serial.print("T1:"); Serial.print(temp1);
  Serial.print(" | T2:"); Serial.println(temp2);
  */

  delay(2000); // Check every 2 seconds
}
