/**
 * TAP-Box Y-Axis Motion Test
 * Team ctrl+WIN - Jamia Millia Islamia
 * Author : AFFAN DANISH
 *
 * Purpose: Test and calibrate Y-axis stepper motor movement
 * 
 * Hardware Connections:
 * - Y-axis stepper motor connected to CNC Shield Y-axis pins
 * - A4988 driver on Y-axis slot
 * - Y-axis limit switch on Pin 10 (optional)
 * 
 * Test Procedure:
 * 1. Upload code to Arduino Uno
 * 2. Open Serial Monitor (9600 baud)
 * 3. Motor will move back and forth along Y-axis
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
#define Y_STEP_PIN     3    // Y-axis step pin
#define Y_DIR_PIN      6    // Y-axis direction pin
#define Y_ENABLE_PIN   8    // Enable pin (shared for all axes)
#define Y_LIMIT_PIN    10   // Y-axis limit switch (optional)

// Motor Configuration
#define STEPS_PER_REV  200  // Standard NEMA 17: 200 steps per revolution (1.8° per step)
#define MICROSTEPS     16   // A4988 microstepping (1, 2, 4, 8, or 16)
#define STEPS_PER_MM   80   // Steps per millimeter (adjust based on your belt/pulley)

// Movement Parameters
#define MAX_SPEED      1000  // Maximum speed in steps per second
#define ACCELERATION   500   // Acceleration in steps per second^2
#define TRAVEL_DISTANCE 200  // Travel distance in mm

// Timing Variables
unsigned long stepDelay = 1000; // Microseconds between steps (initial)
int currentPosition = 0;        // Current position in steps
int targetPosition = 0;         // Target position in steps

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("TAP-Box Y-Axis Motion Test");
  Serial.println("Team ctrl+WIN");
  Serial.println("=================================");
  
  // Configure motor pins as outputs
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  
  // Configure limit switch (if installed)
  pinMode(Y_LIMIT_PIN, INPUT_PULLUP);
  
  // Enable motor driver (LOW = enabled for CNC Shield)
  digitalWrite(Y_ENABLE_PIN, LOW);
  
  // Initial state
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Y_DIR_PIN, LOW);
  
  delay(1000); // Wait for motor to stabilize
  
  Serial.println("\nStarting Y-axis test...");
  Serial.println("Motor will move back and forth");
  Serial.println();
}

void loop() {
  // Move FORWARD (positive direction)
  Serial.println(">>> Moving FORWARD");
  Serial.print("Target position: ");
  Serial.print(TRAVEL_DISTANCE);
  Serial.println(" mm");
  
  moveToPosition(TRAVEL_DISTANCE * STEPS_PER_MM, MAX_SPEED);
  
  Serial.println("Position reached!");
  delay(2000); // Pause at end
  
  // Move BACKWARD (negative direction - return to origin)
  Serial.println("<<< Moving BACKWARD (returning to origin)");
  Serial.println("Target position: 0 mm");
  
  moveToPosition(0, MAX_SPEED);
  
  Serial.println("Origin reached!");
  Serial.println();
  delay(2000); // Pause at origin
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
    digitalWrite(Y_DIR_PIN, HIGH); // Move forward
  } else {
    digitalWrite(Y_DIR_PIN, LOW);  // Move backward
    stepsToMove = -stepsToMove;    // Make positive for counting
  }
  
  // Calculate step delay for desired speed
  unsigned long minDelay = 1000000L / maxSpeed; // Microseconds per step
  
  Serial.print("Moving ");
  Serial.print(stepsToMove);
  Serial.print(" steps (");
  Serial.print(stepsToMove / (float)STEPS_PER_MM);
  Serial.println(" mm)");
  
  // Execute steps
  for (long i = 0; i < stepsToMove; i++) {
    // Check limit switch (safety feature)
    if (digitalRead(Y_LIMIT_PIN) == LOW) {
      Serial.println("LIMIT SWITCH TRIGGERED! Stopping.");
      digitalWrite(Y_ENABLE_PIN, HIGH); // Disable motor
      while(1); // Halt program
    }
    
    // Step pulse
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(10); // Pulse width
    digitalWrite(Y_STEP_PIN, LOW);
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
  digitalWrite(Y_ENABLE_PIN, HIGH); // Disable motor
  Serial.println("!!! EMERGENCY STOP ACTIVATED !!!");
  while(1); // Infinite loop - requires reset
}