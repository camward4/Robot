#include <Wire.h>                                             // Calls for I2C bus library
#include <Servo.h>                                            // Calls the servo library


#define MD25ADDRESS         0x58                              // Address of the MD25
#define SPEED1              0x00                              // Byte to send speed to both motors for forward and backwards motion if operated in MODE 2 or 3 and Motor 1 Speed if in MODE 0 or 1
#define SPEED2              0x01                              // Byte to send speed for turn speed if operated in MODE 2 or 3 and Motor 2 Speed if in MODE 0 or 1
#define ENCODERONE          0x02                              // Byte to read motor encoder 1
#define ENCODERTWO          0x06                              // Byte to read motor encoder 2
#define ACCELERATION        0xE                               // Byte to define motor acceleration
#define CMD                 0x10                              // Byte to reset encoder values
#define MODE_SELECTOR       0xF                               // Byte to change between control MODES
#define NOTE_D4             294                               // Frequency of the note for buzzer to play

int DualSpeedValue = 0;                                       // Combined motor speed variable
int Mode = 2;                                                 // MODE in which the MD25 will operate selector value 
float Wheel_1_Distance_CM = 0;                                // Wheel 1 travel distance variable
float Wheel_2_Distance_CM = 0;                                // Wheel 2 travel distance variable
int degree = 38;                                              // Degrees between each M&M slot
int posCounter = 0;                                           // Counter to determine slot position
int ledPin = 9;                                               // LED pin number
int buzzerPin = 8;                                            // Buzzer pin number
unsigned long noteDuration = 500                              // Length of time for the buzzer to be on (milliseconds)

Servo myservo;                                                // Create servo object to control servo

void setup(){
  Wire.begin();                                               // Begin I2C bus
  Serial.begin(9600);                                         // Begin serial
  delay(100);                                                 // Wait for everything to power up

  Wire.beginTransmission(MD25ADDRESS);                        // Set MD25 operation MODE
  Wire.write(MODE_SELECTOR);
  Wire.write(Mode);                                           
  Wire.endTransmission();

  encodeReset();                                              // Calls a function that resets the encoder values to 0
  myservo.attach(2);                                          // Attaches the servo on pin 2 to the servo object
  pinMode(ledPin, OUTPUT);                                    // Initialize digital pin ledPin as an output.
}

void loop(){
  

  resetServo();
}

void dispense(int posCounter, int degree){
  if (posCounter < 5) {
    posCounter+=;
    myservo.write(posCounter*degree);                         // Tells servo to go to position of next slot
    delay(15);                                                // Delay to allow servo to move to position (Needs to be changed for optimal time)
  }  
}

void waypoint(){
  ledFlash();                                                 // Calls a function to light up the LED
  buzzer();                                                   // Calls a function which plays a sound
}

void ledFlash(){
  digitalWrite(ledPin, HIGH);                                 // turn the LED on (HIGH is the voltage level)
  delay(1000);                                                // wait for 1 second (Change for optimal time)
  digitalWrite(ledPin, LOW);                                  // turn the LED off by making the voltage LOW
}

void buzzer(){
  tone(buzzerPin, NOTE_D4, noteDuration);
}

void resetServo(){
  posCounter = 0;
  myservo.write(posCounter);                                  // Reset servo to starting position
  delay(15);                                                  // Delay to allow M&M to drop (Needs to be changed to optimal time)
}
/* void loop(){

  DualSpeedValue = 150;                                       // Sets a combined motor speed value for upcoming forward motion function
  Wheel_1_Distance_CM = 30;                                   // Sets wheel 1 travel distance value - cm, for upcoming forward motion function
  Wheel_2_Distance_CM = 30;                                   // Sets wheel 1 travel distance value - cm, for upcoming forward motion function
  forward();                                                  // Cals a function that moves the platform forward
  stopMotor();                                                // Cals a function that stops the platform 
  encodeReset();                                              // Cals a function that resets the encoder values to 0 

  delay(500);
  
  DualSpeedValue = 145;                                       // Sets a combined motor speed value for upcoming right turn function
  Wheel_1_Distance_CM = 16;                                // Sets wheel 2 travel distance value - cm, for upcoming right turn motion function
  Wheel_2_Distance_CM = -16;                               // Sets wheel 2 travel distance value - cm, for upcoming right turn motion function
  Right_Turn();                                               // Cals a function that turn the platform right by 90 degrees
  stopMotor();                                                // Cals a function that stops the platform 
  encodeReset();                                              // Cals a function that resets the encoder values to 0 

  delay(500);
}

void forward(){                                               // This function moves the platform forward by a previously set distance
  
  encoder1();                                                 // Calls a function that reads value of encoder 1
  encoder2();                                                 // Calls a function that reads value of encoder 2

    if (encoder1() <= Wheel_1_Distance_CM && encoder2() <= Wheel_2_Distance_CM){     // If statement to check the status of the traveled distance
    
    Wire.beginTransmission(MD25ADDRESS);                      // Sets the acceleration to register 1 (6.375s)
    Wire.write(ACCELERATION);
    Wire.write(1);
    Wire.endTransmission();

    Wire.beginTransmission(MD25ADDRESS);                      // Sets a combined motor speed value
    Wire.write(SPEED1);
    Wire.write(DualSpeedValue);
    Wire.endTransmission();
    
    forward();
  }
}


void Right_Turn(){                                           // This function turns the platform right by a previously set angle
  encoder1();                                                // Calls a function that reads value of encoder 1
  encoder2();                                                // Calls a function that reads value of encoder 2

    if (encoder1() <= Wheel_1_Distance_CM && encoder2() >= Wheel_2_Distance_CM){     // If statement to check the status of the traveled distance
    
    Wire.beginTransmission(MD25ADDRESS);                     // Sets the acceleration to register 1 (6.375s)
    Wire.write(ACCELERATION);
    Wire.write(1);
    Wire.endTransmission();

    Wire.beginTransmission(MD25ADDRESS);                     // Sets a combined motor speed value
    Wire.write(SPEED2);
    Wire.write(DualSpeedValue);
    Wire.endTransmission();
    
    Right_Turn();
  }
}


void encodeReset(){                                         // This function resets the encoder values to 0
  Wire.beginTransmission(MD25ADDRESS);
  Wire.write(CMD);
  Wire.write(0x20);                                         
  Wire.endTransmission(); 
  delay(50);
}



float encoder1(){                                           // Function to read and display value of encoder 1 as a long
  Wire.beginTransmission(MD25ADDRESS);                      // Send byte to get a reading from encoder 1
  Wire.write(ENCODERONE);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to arrive
  long poss1 = Wire.read();                                 // First byte for encoder 1, HH.
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Second byte for encoder 1, HL
  poss1 <<= 8;
  poss1 += Wire.read();                                     // Third byte for encoder 1, LH
  poss1 <<= 8;
  poss1  +=Wire.read();                                     // Fourth byte for encoder 1, LLalue
  delay(5);                                                 // Wait for everything to make sure everything is sent
  return(poss1*0.09);                                       // Convert encoder value to cm
}

float encoder2(){                                            // Function to read and display velue of encoder 2 as a long
  Wire.beginTransmission(MD25ADDRESS);           
  Wire.write(ENCODERTWO);
  Wire.endTransmission();

  Wire.requestFrom(MD25ADDRESS, 4);                         // Request 4 bytes from MD25
  while(Wire.available() < 4);                              // Wait for 4 bytes to become available
  long poss2 = Wire.read();                                 // First byte for encoder 2, HH
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Second byte for encoder 2, HL             
  poss2 <<= 8;
  poss2 += Wire.read();                                     // Third byte for encoder 2, LH             
  poss2 <<= 8;
  poss2  +=Wire.read();                                     // Fourth byte for encoder 2, LLalue
  delay(5);                                                 // Wait to make sure everything is sent
  return(poss2*0.09);                                       // Convert encoder value to cm
}


void stopMotor(){                                           // Function to stop motors
  
  Wire.beginTransmission(MD25ADDRESS);                      // Sets the acceleration to register 10 (0.65s)
  Wire.write(ACCELERATION);
  Wire.write(10);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motors motor 1 if operated in MODE 0 or 1 and Stops both motors if operated in MODE 2 or 3
  Wire.write(SPEED1);
  Wire.write(128);
  Wire.endTransmission();

  Wire.beginTransmission(MD25ADDRESS);                      // Stops motors motor 2 when operated in MODE 0 or 1 and Stops both motors while in turning sequence if operated in MODE 2 or 3
  Wire.write(SPEED2);
  Wire.write(128);
  Wire.endTransmission();
  delay(50);

  encoder1();                                                // Calls a function that reads value of encoder 1
  encoder2();                                                // Calls a function that reads value of encoder 2

  Serial.print("Encoder 1 Distance CM - ");                  // Displays las recorded traveled distance
  Serial.print(encoder1());
  Serial.print("   ");
  Serial.print("Encoder 2 Distance CM - ");
  Serial.print(encoder2());
  Serial.println(" ");
}  */


