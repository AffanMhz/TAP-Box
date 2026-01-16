/**
 * TAP-Box Z-Axis Pin Triggering Test
 * Team ctrl+WIN - Jamia Millia Islamia
 * Author : AFFAN DANISH
 * Purpose: Test Z-axis actuator for Braille pin triggering mechanism
 * 
 * Hardware Connections:
 * - Z-axis linear actuator connected to CNC Shield Z-axis pins
 * - A4988 driver on Z-axis slot (or dedicated driver)
 * - Trigger sensor (optional) for feedback
 * 
 * Test Procedure:
 * 1. Upload code to Arduino Uno
 * 2. Open Serial Monitor (9600 baud)
 * 3. Z-axis will perform up/down triggering motions
 * 4. Verify pin latching mechanism engagement
 * 
 * Expected Results:
 * - Clean triggering motion
 * - Consistent engagement with Braille pins
 * - Reliable return to neutral position
 * 
 * Last Updated: January 2026
 */

// Pin Definitions for CNC Shield V3
#define Z_STEP_PIN     4    // Z-axis step pin
#define Z_DIR_PIN      7    // Z-axis direction pin
#define Z_ENABLE_PIN   8    // Enable pin (shared for all axes)
#define TRIGGER_SENSOR 11   // Optional: feedback sensor

// Motor Configuration
#define STEPS_PER_REV  200  // Standard NEMA 17: 200 steps per revolution
#define MICROSTEPS     16   // A4988 microstepping
#define STEPS_PER_MM   100  // Steps per millimeter (linear actuator)

// Triggering Parameters
#define TRIGGER_DEPTH  5    // How deep to push (in mm)
#define TRIGGER_SPEED  500  // Speed in steps per second
#define PAUSE_TIME     500  // Pause at bottom (milliseconds)

// Current position tracking
int currentPosition = 0;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("TAP-Box Z-Axis Trigger Test");
  Serial.println("Team ctrl+WIN");
  Serial.println("=================================");
  
  // Configure motor pins as outputs
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  
  // Configure sensor (if installed)
  pinMode(TRIGGER_SENSOR, INPUT_PULLUP);
  
  // Enable motor driver (LOW = enabled for CNC Shield)
  digitalWrite(Z_ENABLE_PIN, LOW);
  
  // Initial state (retracted)
  digitalWrite(Z_STEP_PIN, LOW);
  digitalWrite(Z_DIR_PIN, LOW);
  
  delay(1000); // Wait for motor to stabilize
  
  Serial.println("\nStarting Z-axis trigger test...");
  Serial.println("Actuator will extend and retract");
  Serial.println();
}

void loop() {
  // Trigger sequence
  Serial.println("=== Trigger Sequence Start ===");
  
  // Step 1: Extend (push down to trigger pin)
  Serial.println(">>> Extending actuator...");
  triggerDown();
  
  // Step 2: Hold at bottom
  Serial.println("--- Holding position ---");
  delay(PAUSE_TIME);
  
  // Step 3: Retract (return to neutral)
  Serial.println("<<< Retracting actuator...");
  triggerUp();
  
  Serial.println("=== Trigger Sequence Complete ===");
  Serial.println();
  
  delay(2000); // Wait before next trigger
}

/**
 * Extend Z-axis actuator to trigger Braille pin
 * Pushes down to engage the latching mechanism
 */
void triggerDown() {
  digitalWrite(Z_DIR_PIN, HIGH); // Direction: down/extend
  
  int stepsToMove = TRIGGER_DEPTH * STEPS_PER_MM;
  unsigned long stepDelay = 1000000L / TRIGGER_SPEED;
  
  Serial.print("Moving down ");
  Serial.print(TRIGGER_DEPTH);
  Serial.println(" mm");
  
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(stepDelay - 10);
    
    // Check sensor feedback (optional)
    if (digitalRead(TRIGGER_SENSOR) == LOW) {
      Serial.println("Trigger sensor activated!");
    }
  }
  
  currentPosition += stepsToMove;
  Serial.println("Extended position reached");
}

/**
 * Retract Z-axis actuator to neutral position
 * Returns to ready state for next trigger
 */
void triggerUp() {
  digitalWrite(Z_DIR_PIN, LOW); // Direction: up/retract
  
  int stepsToMove = TRIGGER_DEPTH * STEPS_PER_MM;
  unsigned long stepDelay = 1000000L / TRIGGER_SPEED;
  
  Serial.print("Moving up ");
  Serial.print(TRIGGER_DEPTH);
  Serial.println(" mm");
  
  for (int i = 0; i < stepsToMove; i++) {
    digitalWrite(Z_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Z_STEP_PIN, LOW);
    delayMicroseconds(stepDelay - 10);
  }
  
  currentPosition -= stepsToMove;
  Serial.println("Neutral position reached");
}

/**
 * Test pattern: Multiple rapid triggers
 * Useful for testing latch durability
 */
void rapidTriggerTest(int count) {
  Serial.println(">>> Starting Rapid Trigger Test <<<");
  Serial.print("Triggers: ");
  Serial.println(count);
  
  for (int i = 0; i < count; i++) {
    Serial.print("Trigger #");
    Serial.println(i + 1);
    
    triggerDown();
    delay(100);
    triggerUp();
    delay(100);
  }
  
  Serial.println(">>> Rapid Trigger Test Complete <<<");
}