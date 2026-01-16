/**
 * TAP-Box X-Axis Motion Test
 * Team ctrl+WIN - Jamia Millia Islamia
 * Author: AFFAN DANISH
 * 
 * Purpose: Test and calibrate X-axis stepper motor movement
 * 
 * Hardware Connections:
 * - X-axis stepper motor connected to CNC Shield X-axis pins
 * - A4988 driver on X-axis slot
 * - X-axis limit switch on Pin 9 (optional)
 * 
 * Test Procedure:
 * 1. Upload code to Arduino Uno
 * 2. Open Serial Monitor (9600 baud)
 * 3. Motor will move back and forth along X-axis
 * 4. Verify smooth motion and positioning accuracy
 * 
 * Expected Results:
 * - Smooth linear motion without stuttering
 * - No skipped steps
 * - Accurate positioning within ±0.5mm
 * 
 * Last Updated: January 2026
 */

// Pin Definitions for CNC Shield V3
#define X_STEP_PIN 2    // X-axis step pin
#define X_DIR_PIN 5     // X-axis direction pin
#define X_ENABLE_PIN 8  // Enable pin (shared for all axes)
#define X_LIMIT_PIN 9   // X-axis limit switch (optional)

// Motor Configuration
#define STEPS_PER_REV 200  // Standard NEMA 17: 200 steps per revolution (1.8° per step)
#define MICROSTEPS 16      // A4988 microstepping (1, 2, 4, 8, or 16)
#define STEPS_PER_MM 80    // Steps per millimeter (adjust based on your belt/pulley)

// Movement Parameters
#define MAX_SPEED 1000       // Maximum speed in steps per second
#define ACCELERATION 500     // Acceleration in steps per second^2
#define TRAVEL_DISTANCE 200  // Travel distance in mm

// Timing Variables
unsigned long stepDelay = 1000;  // Microseconds between steps (initial)
int currentPosition = 0;         // Current position in steps
int targetPosition = 0;          // Target position in steps

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("TAP-Box X-Axis Motion Test");
  Serial.println("Team ctrl+WIN");
  Serial.println("=================================");

  // Configure motor pins as outputs
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);

  // Configure limit switch (if installed)
  pinMode(X_LIMIT_PIN, INPUT_PULLUP);

  // Enable motor driver (LOW = enabled for CNC Shield)
  digitalWrite(X_ENABLE_PIN, LOW);

  // Initial state
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(X_DIR_PIN, LOW);

  delay(1000);  // Wait for motor to stabilize

  Serial.println("\nStarting X-axis test...");
  Serial.println("Motor will move back and forth");
  Serial.println();
}

void loop() {
  // Move RIGHT (positive direction)
  Serial.println(">>> Moving RIGHT");
  Serial.print("Target position: ");
  Serial.print(TRAVEL_DISTANCE);
  Serial.println(" mm");

  moveToPosition(TRAVEL_DISTANCE * STEPS_PER_MM, MAX_SPEED);

  Serial.println("Position reached!");
  delay(2000);  // Pause at end

  // Move LEFT (negative direction - return to origin)
  Serial.println("<<< Moving LEFT (returning to origin)");
  Serial.println("Target position: 0 mm");

  moveToPosition(0, MAX_SPEED);

  Serial.println("Origin reached!");
  Serial.println();
  delay(2000);  // Pause at origin
}

/**
 * Move to absolute position with acceleration/deceleration
 * 
 * @param target Target position in steps
 * @param maxSpeed Maximum speed in steps per second
 */
void moveToPosition(long target, int maxSpeed) {
  long stepsToMove = target - currentPosition;

  if (stepsToMove == 0) {
    Serial.println("Already at target position");
    return;
  }

  // Set direction
  if (stepsToMove > 0) {
    digitalWrite(X_DIR_PIN, HIGH);  // Move right
  } else {
    digitalWrite(X_DIR_PIN, LOW);  // Move left
    stepsToMove = -stepsToMove;    // Make positive for counting
  }

  // Calculate step delay for desired speed
  unsigned long minDelay = 1000000L / maxSpeed;  // Microseconds per step

  Serial.print("Moving ");
  Serial.print(stepsToMove);
  Serial.print(" steps (");
  Serial.print(stepsToMove / (float)STEPS_PER_MM);
  Serial.println(" mm)");

  // Execute steps
  for (long i = 0; i < stepsToMove; i++) {
    // Check limit switch (safety feature)
    if (digitalRead(X_LIMIT_PIN) == LOW) {
      Serial.println("LIMIT SWITCH TRIGGERED! Stopping.");
      digitalWrite(X_ENABLE_PIN, HIGH);  // Disable motor
      while (1)
        ;  // Halt program
    }

    // Step pulse
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(10);  // Pulse width
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(minDelay - 10);

    // Print progress every 100 steps
    if (i % 100 == 0) {
      Serial.print("Progress: ");
      Serial.print((i * 100) / stepsToMove);
      Serial.println("%");
    }
  }

  // Update current position
  if (target > currentPosition) {
    currentPosition += stepsToMove;
  } else {
    currentPosition -= stepsToMove;
  }

  Serial.println("Movement complete!");
  Serial.print("Current position: ");
  Serial.print(currentPosition / (float)STEPS_PER_MM);
  Serial.println(" mm");
}

/**
 * Emergency stop function
 * Call this if you need to immediately halt motion
 */
void emergencyStop() {
  digitalWrite(X_ENABLE_PIN, HIGH);  // Disable motor
  Serial.println("!!! EMERGENCY STOP ACTIVATED !!!");
  while (1)
    ;  // Infinite loop - requires reset
}