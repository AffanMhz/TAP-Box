/**
 * TAP-Box G-Code Executor with GRBL-style Commands
 * Author: AFFAN DANISH
 * Team: ctrl+WIN - Jamia Millia Islamia
 *
 * Purpose:
 *  - Execute a small subset of G-code (G00, G01, G28) and machine codes (M03, M05, M17, M18)
 *  - Designed for TAP-Box braille plotter prototype
 *
 * Notes / Changes:
 *  - Coordinated X/Y motion uses a DDA-like stepping algorithm for better proportional stepping.
 *  - Simple timeout protection for homing to avoid infinite loops.
 *  - Clear pin / config constants, added safety checks.
 *
 * Last Updated: January 2026
 */

#include <Arduino.h>

// ----------------------- Pin Definitions -----------------------
const uint8_t X_STEP_PIN  = 2;
const uint8_t X_DIR_PIN   = 5;
const uint8_t Y_STEP_PIN  = 3;
const uint8_t Y_DIR_PIN   = 6;
const uint8_t Z_STEP_PIN  = 4;
const uint8_t Z_DIR_PIN   = 7;
const uint8_t ENABLE_PIN  = 8;
const uint8_t X_LIMIT_PIN = 9;
const uint8_t Y_LIMIT_PIN = 10;

// ----------------------- Configuration -------------------------
const float  STEPS_PER_MM    = 80.0f;    // mechanical calibration (steps per mm)
const uint32_t MAX_SPEED     = 1000u;    // steps per second (max)
const uint32_t HOMING_SPEED  = 300u;     // steps per second for homing
const float  Z_TRIGGER_DEPTH = 5.0f;     // mm to push Z for triggering a pin
const float  Z_STEPS_PER_MM  = 100.0f;   // Z steps per mm (calibrate)
const uint32_t Z_SPEED       = 500u;     // steps/sec for Z movement
const uint32_t STEP_PULSE_US = 6u;       // pulse width in microseconds

// Safety/timeouts
const uint32_t HOMING_TIMEOUT_MS = 20000u; // 20 seconds per axis max

// ----------------------- State Variables -----------------------
volatile float currentX = 0.0f;
volatile float currentY = 0.0f;
bool motorsEnabled = false;
String commandBuffer = "";

// ----------------------- Setup -----------------------
void setup() {
  Serial.begin(9600);

  // Banner
  Serial.println(F("================================="));
  Serial.println(F("TAP-Box G-Code Executor"));
  Serial.println(F("Author: AFFAN DANISH"));
  Serial.println(F("Team ctrl+WIN - Jamia Millia Islamia"));
  Serial.println(F("================================="));
  Serial.println(F("Ready to receive G-code commands"));
  Serial.println(F("Type 'HELP' for command list"));
  Serial.println();

  // Configure pins
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(X_LIMIT_PIN, INPUT_PULLUP);
  pinMode(Y_LIMIT_PIN, INPUT_PULLUP);

  // Initial safe state: motors disabled
  disableMotors();

  Serial.println(F("ok")); // GRBL-style ready
}

// ----------------------- Main Loop -----------------------
void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    // Accept both newline and carriage return as terminators
    if (c == '\n' || c == '\r') {
      if (commandBuffer.length() > 0) {
        processCommand(commandBuffer);
        commandBuffer = "";
      }
    } else {
      // Filter out control characters except space
      if (c >= 32) commandBuffer += c;
    }
  }
}

// ----------------------- Command Processing -----------------------
void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd.length() == 0) return;

  Serial.print(F("Received: "));
  Serial.println(cmd);

  // Basic direct commands
  if (cmd == "HELP") {
    printHelp();
    return;
  }
  if (cmd == "?") {
    printStatus();
    return;
  }

  // Ignore comments after ';' or '('
  int commentPos = cmd.indexOf(';');
  if (commentPos >= 0) cmd = cmd.substring(0, commentPos).trim();

  if (cmd.startsWith("G")) {
    handleGCommand(cmd);
  } else if (cmd.startsWith("M")) {
    handleMCommand(cmd);
  } else {
    Serial.println(F("error: Unknown command"));
  }
}

// ----------------------- G-Commands -----------------------
void handleGCommand(String cmd) {
  int gCode = (int)parseValue(cmd, 'G');

  switch (gCode) {
    case 0: // G00 - Rapid positioning (no Z trigger)
    case 1: // G01 - Linear movement (optional Z)
      {
        float x = parseValue(cmd, 'X');
        float y = parseValue(cmd, 'Y');
        float z = parseValue(cmd, 'Z');

        bool hasX = (cmd.indexOf('X') != -1);
        bool hasY = (cmd.indexOf('Y') != -1);
        bool hasZ = (cmd.indexOf('Z') != -1);

        float targetX = hasX ? x : currentX;
        float targetY = hasY ? y : currentY;

        uint32_t speed = (gCode == 0) ? MAX_SPEED : MAX_SPEED; // keep same for now

        moveToXY(targetX, targetY, speed);

        // If Z specified and equals 1 (or >0), trigger
        if (hasZ && z >= 1.0f) {
          triggerPin();
        }

        Serial.println(F("ok"));
      }
      break;

    case 28: // G28 - Home all axes
      homeAllAxes();
      Serial.println(F("ok"));
      break;

    default:
      Serial.print(F("error: Unsupported G-code G"));
      Serial.println(gCode);
  }
}

// ----------------------- M-Commands -----------------------
void handleMCommand(String cmd) {
  int mCode = (int)parseValue(cmd, 'M');

  switch (mCode) {
    case 3: // M03 - Trigger pin
      triggerPin();
      Serial.println(F("ok"));
      break;

    case 5: // M05 - Retract trigger
      retractTrigger();
      Serial.println(F("ok"));
      break;

    case 17: // M17 - Enable motors
      enableMotors();
      Serial.println(F("ok"));
      break;

    case 18: // M18 - Disable motors
      disableMotors();
      Serial.println(F("ok"));
      break;

    default:
      Serial.print(F("error: Unsupported M-code M"));
      Serial.println(mCode);
  }
}

// ----------------------- Parsing -----------------------
/**
 * parseValue:
 *  - Finds first occurrence of letter in cmd and parses the following number.
 *  - Returns NAN if not found.
 */
float parseValue(const String &cmd, char letter) {
  int index = cmd.indexOf(letter);
  if (index == -1) return NAN;

  String value = "";
  for (int i = index + 1; i < cmd.length(); ++i) {
    char c = cmd.charAt(i);
    if ( (c >= '0' && c <= '9') || c == '.' || c == '-' ) {
      value += c;
    } else {
      break;
    }
  }
  if (value.length() == 0) return NAN;
  return value.toFloat();
}

// ----------------------- Motion Primitives -----------------------
/**
 * moveToXY:
 *  - Moves from (currentX,currentY) to (targetX,targetY)
 *  - speed: steps per second
 *  - Uses DDA-like stepping so X and Y remain coordinated
 */
void moveToXY(float targetX, float targetY, uint32_t speed) {
  if (!motorsEnabled) enableMotors();

  float deltaX_mm = targetX - currentX;
  float deltaY_mm = targetY - currentY;

  long stepsX = lround(fabs(deltaX_mm * STEPS_PER_MM));
  long stepsY = lround(fabs(deltaY_mm * STEPS_PER_MM));

  // No motion required
  if (stepsX == 0 && stepsY == 0) {
    // update position anyway
    currentX = targetX;
    currentY = targetY;
    return;
  }

  digitalWrite(X_DIR_PIN, (deltaX_mm >= 0.0f) ? HIGH : LOW);
  digitalWrite(Y_DIR_PIN, (deltaY_mm >= 0.0f) ? HIGH : LOW);

  long n = max(stepsX, stepsY);
  // stepDelay in microseconds
  uint32_t stepDelayUs = (speed > 0) ? (1000000UL / speed) : 1000UL;

  // DDA counters
  long accX = 0;
  long accY = 0;

  // Avoid division->use integer accumulation scaled to n
  for (long i = 0; i < n; ++i) {
    accX += stepsX;
    if (accX >= n) {
      pulsePin(X_STEP_PIN);
      accX -= n;
    }
    accY += stepsY;
    if (accY >= n) {
      pulsePin(Y_STEP_PIN);
      accY -= n;
    }
    // coarse pacing
    delayMicroseconds(stepDelayUs);
  }

  // final step correction if rounding left one step
  // (rare) — ensure we reach exact counts
  // (for reliability we set state variables based on requested target)
  currentX = targetX;
  currentY = targetY;
}

/**
 * pulsePin:
 *  - Sends a single step pulse with configured pulse width
 */
inline void pulsePin(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(STEP_PULSE_US);
  digitalWrite(pin, LOW);
}

// ----------------------- Z-axis trigger -----------------------
void triggerPin() {
  if (!motorsEnabled) enableMotors();

  digitalWrite(Z_DIR_PIN, HIGH); // extend
  long steps = lround(Z_TRIGGER_DEPTH * Z_STEPS_PER_MM);
  uint32_t stepDelay = (Z_SPEED > 0) ? (1000000UL / Z_SPEED) : 2000UL;

  for (long i = 0; i < steps; ++i) {
    pulsePin(Z_STEP_PIN);
    delayMicroseconds(stepDelay - STEP_PULSE_US);
  }
}

void retractTrigger() {
  if (!motorsEnabled) enableMotors();

  digitalWrite(Z_DIR_PIN, LOW); // retract
  long steps = lround(Z_TRIGGER_DEPTH * Z_STEPS_PER_MM);
  uint32_t stepDelay = (Z_SPEED > 0) ? (1000000UL / Z_SPEED) : 2000UL;

  for (long i = 0; i < steps; ++i) {
    pulsePin(Z_STEP_PIN);
    delayMicroseconds(stepDelay - STEP_PULSE_US);
  }
}

// ----------------------- Homing -----------------------
void homeAllAxes() {
  Serial.println(F("Homing..."));

  if (!motorsEnabled) enableMotors();

  // Home X
  Serial.print(F("Homing X..."));
  digitalWrite(X_DIR_PIN, LOW); // move toward limit switch (assuming pullup)
  unsigned long start = millis();
  while (digitalRead(X_LIMIT_PIN) == HIGH) { // until pressed (LOW)
    pulsePin(X_STEP_PIN);
    delayMicroseconds(2000);
    // timeout safety
    if ((millis() - start) > HOMING_TIMEOUT_MS) {
      Serial.println(F(" error: X homing timeout"));
      break;
    }
  }
  currentX = 0.0f;
  Serial.println(F(" done"));

  // Home Y
  Serial.print(F("Homing Y..."));
  digitalWrite(Y_DIR_PIN, LOW); // move toward limit
  start = millis();
  while (digitalRead(Y_LIMIT_PIN) == HIGH) {
    pulsePin(Y_STEP_PIN);
    delayMicroseconds(2000);
    if ((millis() - start) > HOMING_TIMEOUT_MS) {
      Serial.println(F(" error: Y homing timeout"));
      break;
    }
  }
  currentY = 0.0f;
  Serial.println(F(" done"));

  Serial.println(F("Homing complete"));
}

// ----------------------- Motor enable/disable -----------------------
void enableMotors() {
  digitalWrite(ENABLE_PIN, LOW); // active low enable (common)
  motorsEnabled = true;
  delay(2);
}

void disableMotors() {
  digitalWrite(ENABLE_PIN, HIGH);
  motorsEnabled = false;
  delay(2);
}

// ----------------------- Utilities / IO -----------------------
void printHelp() {
  Serial.println();
  Serial.println(F("=== TAP-Box G-Code Commands ==="));
  Serial.println(F("Motion Commands:"));
  Serial.println(F("  G00 Xnn Ynn    - Rapid move to position"));
  Serial.println(F("  G01 Xnn Ynn Zn - Linear move (Z1 = trigger)"));
  Serial.println(F("  G28            - Home all axes"));
  Serial.println(F(""));
  Serial.println(F("Machine Commands:"));
  Serial.println(F("  M03            - Trigger pin"));
  Serial.println(F("  M05            - Retract trigger"));
  Serial.println(F("  M17            - Enable motors"));
  Serial.println(F("  M18            - Disable motors"));
  Serial.println(F(""));
  Serial.println(F("Utility:"));
  Serial.println(F("  ?              - Print status"));
  Serial.println(F("  HELP           - Show this help"));
  Serial.println(F(""));
  Serial.println(F("Examples:"));
  Serial.println(F("  G00 X10 Y10    - Move to (10,10)"));
  Serial.println(F("  G01 X20 Y20 Z1 - Move to (20,20) and trigger"));
  Serial.println(F("  M03            - Trigger at current position"));
  Serial.println(F("================================"));
  Serial.println();
}

void printStatus() {
  Serial.print(F("<"));
  Serial.print(motorsEnabled ? F("Run") : F("Idle"));
  Serial.print(F("|MPos:"));
  Serial.print(currentX, 3);
  Serial.print(F(","));
  Serial.print(currentY, 3);
  Serial.print(F(",0.000"));
  Serial.println(F(">"));
}
