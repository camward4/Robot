#define NOTE_D4             294

int ledPin = 9;                                               // LED pin number
int buzzerPin = 8;                                            // Buzzer pin number
unsigned long ledDuration = 500;                              // Length of time for the buzzer and LED to be on (milliseconds)



void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);                                    // Initialize digital pin ledPin as an output.
}

void loop() {
  waypoint();
  delay(2000);

}

void buzzer(){
  tone(buzzerPin, NOTE_D4, ledDuration);                      // Creates a tone of duration ledDuration
}

void waypoint(){
  digitalWrite(ledPin, HIGH);                                 // Turn the LED on (HIGH is the voltage level)
  buzzer();                                                   // Calls the buzzer function
  delay(ledDuration);                                         // Delay to keep LED on for a duration of ledDuration
  digitalWrite(ledPin, LOW);                                  // turn the LED off by making the voltage LOW
}

