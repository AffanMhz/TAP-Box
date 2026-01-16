/**
 * TAP-Box Combined X-Y Motion Test
 * Team ctrl+WIN - Jamia Millia Islamia
 * 
 * Purpose: Test coordinated X-Y plotter movement
 * 
 * Hardware Connections:
 * - X-axis stepper on CNC Shield X-axis pins
 * - Y-axis stepper on CNC Shield Y-axis pins
 * - Both A4988 drivers installed
 * 
 * Test Procedure:
 * 1. Upload code to Arduino Uno
 * 2. Open Serial Monitor (9600 baud)
 * 3. Plotter will trace various patterns
 * 4. Verify coordinated multi-axis movement
 * 
 * Expected Results:
 * - Smooth diagonal and curved movements
 * - Accurate positioning at all test points
 * - No axis interference or binding
 * 
 * Test Patterns:
 * - Square pattern (corners)
 * - Diagonal movements
 * - Circle pattern
 * - Braille cell grid pattern
 * 
 * Last Updated: January 2026
 */

// Pin Definitions for CNC Shield V3
#define X_STEP_PIN     2
#define X_DIR_PIN      5
#define Y_STEP_PIN     3
#define Y_DIR_PIN      6
#define ENABLE_PIN     8

// Motor Configuration
#define STEPS_PER_MM   80   // Adjust based on your mechanics
#define MAX_SPEED      1000 // Steps per second
#define GRID_SIZE      18   // 18x18 Braille cell grid
#define CELL_PITCH     2.5  // 2.5mm between Braille cells

// Position tracking
float currentX = 0;
float currentY = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("TAP-Box XY Combined Motion Test");
  Serial.println("Team ctrl+WIN");
  Serial.println("=================================");
  
  // Configure all pins
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Enable drivers
  digitalWrite(ENABLE_PIN, LOW);
  
  // Initial state
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Y_DIR_PIN, LOW);
  
  delay(1000);
  
  Serial.println("\nStarting coordinated XY motion tests");
  Serial.println("Multiple test patterns will be executed\n");
}

void loop() {
  // Test Pattern 1: Square corners
  Serial.println("=== TEST 1: Square Pattern ===");
  testSquarePattern(100, 100); // 100mm x 100mm square
  delay(3000);
  
  // Test Pattern 2: Diagonal movements
  Serial.println("\n=== TEST 2: Diagonal Movements ===");
  testDiagonals(100);
  delay(3000);
  
  // Test Pattern 3: Braille grid pattern
  Serial.println("\n=== TEST 3: Braille Grid Pattern ===");
  testBrailleGrid();
  delay(3000);
  
  // Return to origin
  Serial.println("\n=== Returning to Origin ===");
  moveToXY(0, 0, MAX_SPEED);
  delay(5000);
}

/**
 * Move to absolute X,Y position using coordinated motion
 * 
 * @param targetX Target X position in mm
 * @param targetY Target Y position in mm
 * @param speed Movement speed in steps per second
 */
void moveToXY(float targetX, float targetY, int speed) {
  // Calculate distances to move
  float deltaX = targetX - currentX;
  float deltaY = targetY - currentY;
  
  // Calculate steps needed for each axis
  long stepsX = abs(deltaX * STEPS_PER_MM);
  long stepsY = abs(deltaY * STEPS_PER_MM);
  
  // Set directions
  digitalWrite(X_DIR_PIN, (deltaX >= 0) ? HIGH : LOW);
  digitalWrite(Y_DIR_PIN, (deltaY >= 0) ? HIGH : LOW);
  
  // Calculate total distance for speed control
  float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
  unsigned long stepDelay = 1000000L / speed;
  
  Serial.print("Moving to (");
  Serial.print(targetX);
  Serial.print(", ");
  Serial.print(targetY);
  Serial.print(") - Distance: ");
  Serial.print(distance);
  Serial.println(" mm");
  
  // Use Bresenham-like algorithm for coordinated motion
  long maxSteps = max(stepsX, stepsY);
  
  for (long i = 0; i <= maxSteps; i++) {
    // Step X if needed
    if (i * stepsX / maxSteps < stepsX) {
      if (i > 0 && (i * stepsX / maxSteps) != ((i-1) * stepsX / maxSteps)) {
        digitalWrite(X_STEP_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(X_STEP_PIN, LOW);
      }
    }
    
    // Step Y if needed
    if (i * stepsY / maxSteps < stepsY) {
      if (i > 0 && (i * stepsY / maxSteps) != ((i-1) * stepsY / maxSteps)) {
        digitalWrite(Y_STEP_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(Y_STEP_PIN, LOW);
      }
    }
    
    delayMicroseconds(stepDelay);
    
    // Progress indicator
    if (i % (maxSteps / 10) == 0 && maxSteps > 10) {
      Serial.print(".");
    }
  }
  
  Serial.println(" Done!");
  
  // Update current position
  currentX = targetX;
  currentY = targetY;
  
  Serial.print("Position: (");
  Serial.print(currentX);
  Serial.print(", ");
  Serial.print(currentY);
  Serial.println(")");
}

/**
 * Test Pattern 1: Square corners
 * Tests movement to four corners of a square
 */
void testSquarePattern(float width, float height) {
  Serial.println("Tracing square pattern...");
  
  // Start at origin
  moveToXY(0, 0, MAX_SPEED);
  delay(500);
  
  // Bottom right
  moveToXY(width, 0, MAX_SPEED);
  delay(500);
  
  // Top right
  moveToXY(width, height, MAX_SPEED);
  delay(500);
  
  // Top left
  moveToXY(0, height, MAX_SPEED);
  delay(500);
  
  // Back to origin
  moveToXY(0, 0, MAX_SPEED);
  
  Serial.println("Square pattern complete!");
}

/**
 * Test Pattern 2: Diagonal movements
 * Tests coordinated multi-axis motion
 */
void testDiagonals(float distance) {
  Serial.println("Testing diagonal movements...");
  
  // Diagonal to top-right
  moveToXY(distance, distance, MAX_SPEED);
  delay(500);
  
  // Diagonal to bottom-left (back to origin)
  moveToXY(0, 0, MAX_SPEED);
  delay(500);
  
  // Diagonal to top-left
  moveToXY(0, distance, MAX_SPEED);
  delay(500);
  
  // Diagonal to bottom-right
  moveToXY(distance, 0, MAX_SPEED);
  delay(500);
  
  // Back to origin
  moveToXY(0, 0, MAX_SPEED);
  
  Serial.println("Diagonal test complete!");
}

/**
 * Test Pattern 3: Braille cell grid
 * Visits positions corresponding to Braille cells
 */
void testBrailleGrid() {
  Serial.println("Testing Braille grid positions...");
  Serial.print("Grid size: ");
  Serial.print(GRID_SIZE);
  Serial.print("x");
  Serial.println(GRID_SIZE);
  
  // Visit corners of grid
  float maxX = (GRID_SIZE - 1) * CELL_PITCH;
  float maxY = (GRID_SIZE - 1) * CELL_PITCH;
  
  Serial.println("Visiting grid corners...");
  
  // Bottom-left (0,0) - already there
  delay(500);
  
  // Bottom-right
  moveToXY(maxX, 0, MAX_SPEED);
  delay(500);
  
  // Top-right
  moveToXY(maxX, maxY, MAX_SPEED);
  delay(500);
  
  // Top-left
  moveToXY(0, maxY, MAX_SPEED);
  delay(500);
  
  // Center position
  moveToXY(maxX / 2, maxY / 2, MAX_SPEED);
  delay(500);
  
  // Sample some grid positions
  Serial.println("Sampling grid positions...");
  for (int i = 0; i < 5; i++) {
    int x = random(0, GRID_SIZE);
    int y = random(0, GRID_SIZE);
    
    Serial.print("Cell (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(")");
    
    moveToXY(x * CELL_PITCH, y * CELL_PITCH, MAX_SPEED);
    delay(300);
  }
  
  // Return to origin
  moveToXY(0, 0, MAX_SPEED);
  
  Serial.println("Braille grid test complete!");
}

/**
 * Draw a simple character 'A' in Braille
 * Braille 'A' is dot 1 (top-left of 6-dot cell)
 */
void drawBrailleA() {
  Serial.println("Drawing Braille 'A'...");
  
  // Move to position for dot 1
  float dotX = 0;
  float dotY = 0;
  
  moveToXY(dotX, dotY, MAX_SPEED);
  Serial.println("Position for dot 1 reached");
  
  // Here you would trigger the Z-axis to set the pin
  // triggerPin(); // Call Z-axis function
  
  Serial.println("Braille 'A' complete!");
}