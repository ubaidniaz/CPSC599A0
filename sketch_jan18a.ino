// Ubaid Niaz 30179000 A0 Hello World
// Reaction time test game Arduino soure code
const int BUTTON_PIN = 7;   // Connect the Button to pin 7
const int LED_PIN1 = 3;     // Connect the reaction time LED to pin 3
const int LED_PIN2 = 5;     // Connect the better time LED to pin 5

unsigned long startTime;
unsigned long reactionTime;
unsigned long totalReactionTime = 0;           // Total reaction time for all rounds
unsigned long bestReactionTime = 4294967295;   // Initialize with the maximum possible value
int roundCount = 0;
bool isLedOn = false;
bool gameActive = false; // to indicate if the game is active

void setup() {
  Serial.begin(9600);                           // Initialize serial communication
  pinMode(BUTTON_PIN, INPUT);                   // Set button pin to input mode
  pinMode(LED_PIN1, OUTPUT);                    // Set LED1 pin to output mode
  pinMode(LED_PIN2, OUTPUT);                    // Set LED2 pin to output mode
  
  digitalWrite(LED_PIN2, LOW);                  // Ensure LED2 is off initially
}

// Blinking function
void blinkLed(int pin, int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    if (i < times - 1) { // Avoid unnecessary delay after the last blink
      delay(duration);
    }
  }
}

void loop() {
  if (!gameActive) {
    // Check if the button is pressed to start the game
    if (digitalRead(BUTTON_PIN) == HIGH) {
      gameActive = true;
      roundCount = 0;
      totalReactionTime = 0;                    // Reset the total reaction time for the new game
      bestReactionTime = 4294967295;            // Reset the best reaction time for the new game
      Serial.println("Game started!");
    }
  } else {
    if (roundCount < 5) {
      if (!isLedOn) {
        long waitTime = random(1500, 4000);         // Random wait time between 1.5-4 seconds
        delay(waitTime);
        digitalWrite(LED_PIN1, HIGH);               // Turn on LED1
        startTime = millis();                       // Record the time LED1 turns on
        isLedOn = true;
      } else {
        if (digitalRead(BUTTON_PIN) == HIGH) {      // Check if button is pressed
          reactionTime = millis() - startTime;      // Calculate reaction time
          Serial.print("Reaction Time (Round ");
          Serial.print(roundCount + 1);
          Serial.print("): ");
          Serial.print(reactionTime);
          Serial.println(" ms");

          if (reactionTime < bestReactionTime) {    // Check if this reaction time is the best so far
            bestReactionTime = reactionTime;        // Update the best reaction time
            blinkLed(LED_PIN2, 3, 250);             // Blink LED_PIN2 for 3 times with 250ms interval
          }

          totalReactionTime += reactionTime;        // Add current reaction time to total

          digitalWrite(LED_PIN1, LOW);              // Turn off LED1
          isLedOn = false;
          roundCount++;
          delay(3000);                              // Wait 3 seconds before next round
        }
      }
    } else {
      // Game ends, calculate and display average and best reaction time
      Serial.print("Best Reaction Time: ");
      Serial.print(bestReactionTime);
      Serial.println(" ms");

      // Calculate and display average reaction time
      if (roundCount > 0) {
        unsigned long averageReactionTime = totalReactionTime / roundCount;
        Serial.print("Average Reaction Time: ");
        Serial.print(averageReactionTime);
        Serial.println(" ms");
      }
      
      gameActive = false; // Reset the game
      Serial.println("Game ended! Press the button to start a new game.");
    }
  }
}
