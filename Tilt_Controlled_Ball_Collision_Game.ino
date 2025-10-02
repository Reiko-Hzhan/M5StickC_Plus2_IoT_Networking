#include "M5StickCPlus2.h"
#include <stdlib.h>  // For random number generation

// Player ball property settings
float ballX = 120;        // Initial X coordinate of the ball (center of the screen)
float ballY = 67;         // Initial Y coordinate of the ball (center of the screen)
float ballSpeedX = 0;     // Ball speed in X direction
float ballSpeedY = 0;     // Ball speed in Y direction
const int ballRadius = 8; // Ball radius
const int ballColor = RED; // Ball color

// Random-moving enemy ball properties
float enemyX;             // Enemy ball X coordinate
float enemyY;             // Enemy ball Y coordinate
float enemySpeedX;        // Enemy ball speed in X direction
float enemySpeedY;        // Enemy ball speed in Y direction
const int enemyRadius = 6; // Enemy ball radius
const int enemyColor = BLUE; // Enemy ball color

// Student ID setting
const char* studentID = "11111111";

// Control parameters
const float sensitivity = 1.2;  // Sensitivity
const float bounceForce = 5.0;  // Collision bounce force
const int enemyMinSpeed = 1;    // Minimum speed of enemy ball
const int enemyMaxSpeed = 3;    // Maximum speed of enemy ball

void setup() {
  // Initialize M5StickCPlus2 device
  M5.begin();
  
  // Configure screen (M5StickCPlus2 screen resolution is 240x135)
  M5.Lcd.setRotation(1);  // Set screen orientation
  M5.Lcd.fillScreen(BLACK);  // Black background
  M5.Lcd.setTextColor(WHITE); // Text color is white
  
  // Initialize enemy ball position and speed
  initEnemy();
}

// Initialize enemy ball
void initEnemy() {
  // Random position (ensure it's within the screen)
  enemyX = random(enemyRadius, M5.Lcd.width() - enemyRadius);
  enemyY = random(enemyRadius, M5.Lcd.height() - enemyRadius);
  
  // Random speed (including direction)
  enemySpeedX = random(enemyMinSpeed, enemyMaxSpeed + 1);
  enemySpeedY = random(enemyMinSpeed, enemyMaxSpeed + 1);
  
  // 50% chance to reverse direction
  if (random(2) == 0) enemySpeedX = -enemySpeedX;
  if (random(2) == 0) enemySpeedY = -enemySpeedY;
}

// Update enemy ball position
void updateEnemy() {
  // Move the enemy ball
  enemyX += enemySpeedX;
  enemyY += enemySpeedY;
  
  // Bounce when hitting the boundary
  if (enemyX <= enemyRadius || enemyX >= M5.Lcd.width() - enemyRadius) {
    enemySpeedX = -enemySpeedX;
    // Ensure it doesn't get stuck on the boundary
    enemyX = constrain(enemyX, enemyRadius, M5.Lcd.width() - enemyRadius);
  }
  
  if (enemyY <= enemyRadius || enemyY >= M5.Lcd.height() - enemyRadius) {
    enemySpeedY = -enemySpeedY;
    // Ensure it doesn't get stuck on the boundary
    enemyY = constrain(enemyY, enemyRadius, M5.Lcd.height() - enemyRadius);
  }
}

// Check for collision
bool checkCollision() {
  // Calculate distance between the two balls
  float dx = ballX - enemyX;
  float dy = ballY - enemyY;
  float distance = sqrt(dx*dx + dy*dy);
  
  // Collision occurs if distance is less than the sum of radii
  return distance < (ballRadius + enemyRadius);
}

// Handle collision bounce
void handleCollision() {
  // Calculate collision direction
  float dx = ballX - enemyX;
  float dy = ballY - enemyY;
  float distance = sqrt(dx*dx + dy*dy);
  
  // Normalize direction vector
  float dirX = dx / distance;
  float dirY = dy / distance;
  
  // Apply bounce force
  ballSpeedX = dirX * bounceForce;
  ballSpeedY = dirY * bounceForce;
}

void loop() {
  M5.update();  // Update device status
  
  // Variables to store acceleration data
  float ax, ay, az;
  
  // Get acceleration data
  M5.Imu.getAccelData(&ax, &ay, &az);
  
  // Update player ball speed based on acceleration
  ballSpeedX += ay * sensitivity * 0.2;  // Use a smaller coefficient for smoother control
  ballSpeedY += ax * sensitivity * 0.2;
  
  // Apply speed to position
  ballX += ballSpeedX;
  ballY += ballSpeedY;
  
  // Speed decay (simulate friction)
  ballSpeedX *= 0.9;
  ballSpeedY *= 0.9;
  
  // Restrict player ball within the screen
  if (ballX < ballRadius) {
    ballX = ballRadius;
    ballSpeedX = -ballSpeedX * 0.5;  // Bounce when hitting the boundary
  } else if (ballX > M5.Lcd.width() - ballRadius) {
    ballX = M5.Lcd.width() - ballRadius;
    ballSpeedX = -ballSpeedX * 0.5;
  }
  
  if (ballY < ballRadius) {
    ballY = ballRadius;
    ballSpeedY = -ballSpeedY * 0.5;
  } else if (ballY > M5.Lcd.height() - ballRadius) {
    ballY = M5.Lcd.height() - ballRadius;
    ballSpeedY = -ballSpeedY * 0.5;
  }
  
  // Update enemy ball position
  updateEnemy();
  
  // Check for collision
  if (checkCollision()) {
    handleCollision();  // Handle collision bounce
  }
  
  // Clear the screen
  M5.Lcd.fillScreen(BLACK);
  
  // Draw player ball
  M5.Lcd.fillCircle(ballX, ballY, ballRadius, ballColor);
  
  // Draw enemy ball
  M5.Lcd.fillCircle(enemyX, enemyY, enemyRadius, enemyColor);
  
  // Display student ID
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 8);
  M5.Lcd.print(studentID);
  
  delay(10);  // Control refresh rate
}