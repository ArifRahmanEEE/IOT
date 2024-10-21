// Import library for flash memory operations
#include <EEPROM.h>

// Define the EEPROM size for accessing 1 byte
#define EEPROM_SIZE 1

// Pin definitions remain unchanged:
const int inputButtonPin = 4;    // Pushbutton pin
const int outputLEDPin = 16;     // LED pin

// Variables for tracking states:
int stateOfLED = HIGH;       // Current LED state (ON/OFF)
int inputButtonState;        // Current state of the button
int lastInputState = LOW;    // Previous button state

// Timing variables for debouncing:
unsigned long debounceStartTime = 0;  
unsigned long debounceTime = 50;      // Debounce delay

void setup() {
  Serial.begin(115200);

  // Initialize EEPROM memory size
  EEPROM.begin(EEPROM_SIZE);

  pinMode(inputButtonPin, INPUT);
  pinMode(outputLEDPin, OUTPUT);

  // Load and set the last saved LED state from EEPROM
  stateOfLED = EEPROM.read(0);
  digitalWrite(outputLEDPin, stateOfLED);
}

void loop() {
  // Read button's current state
  int currentButtonReading = digitalRead(inputButtonPin);

  // Detect button state changes:
  if (currentButtonReading != lastInputState) {
    debounceStartTime = millis();
  }

  // Check after debounce time:
  if ((millis() - debounceStartTime) > debounceTime) {
    if (currentButtonReading != inputButtonState) {
      inputButtonState = currentButtonReading;

      // Toggle LED state if button is pressed
      if (inputButtonState == HIGH) {
        stateOfLED = !stateOfLED;
      }
    }
  }

  // Save the current state for next loop iteration
  lastInputState = currentButtonReading;

  // If the LED state changes, update and save to EEPROM
  if (digitalRead(outputLEDPin) != stateOfLED) {
    Serial.println("LED state updated");
    digitalWrite(outputLEDPin, stateOfLED);
    EEPROM.write(0, stateOfLED);
    EEPROM.commit();
    Serial.println("Saved to EEPROM");
  }
}
