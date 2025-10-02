/**
 * @file button.ino
 * @brief M5StickCPlus2 Button Count
 * @date 2025-09-15
 */

#include "M5StickCPlus2.h"

// Display student ID
const char* studentID = "111111111";

// Button count
int countA = 0;

void updateDisplay();

void setup() {

    auto cfg = M5.config();  // Create hardware configuration object (using default configuration)
    StickCP2.begin(cfg);  // Initialize M5StickCPlus2 board (StickCP2 is the board abbreviation)

    StickCP2.Display.setRotation(1);  // Set screen rotation angle (1 means 90-degree rotation, adapting to the board's default usage orientation)
    StickCP2.Display.setTextColor(GREEN);  // Set text color to green
    StickCP2.Display.setTextDatum(top_left);  // Set text alignment
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);  // Set font
    StickCP2.Display.setTextSize(1);  // Set text size to 1 (no scaling)
    
    updateDisplay();
}

void loop() {

    StickCP2.update();
    
    // Detect BtnA press event and count
    if (StickCP2.BtnA.wasPressed()) {
        StickCP2.Speaker.tone(8000, 20);
        countA++;  // Increment A button count by 1
        updateDisplay();  // Update display
    }
    
    // Detect BtnB press event and clear data
    if (StickCP2.BtnB.wasPressed()) {
        StickCP2.Speaker.tone(6000, 20);  // B button uses a different tone
        countA = 0;
        updateDisplay();  // Update display
    }

}

// Update screen display content
void updateDisplay() {

    // Clear screen
    StickCP2.Display.clear();
    
    // Display student ID
    StickCP2.Display.drawString("ID: " + String(studentID), 10, 10);

    // Display A button count on a new line
    StickCP2.Display.drawString("A BUTTON: " + String(countA), 10, 90);
    
}
