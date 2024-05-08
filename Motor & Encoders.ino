#include "Arduino.h"
#include "DCMDriverL298.h"

// Pin Definitions
#define LEFT_CHANNEL_A_PIN 38
#define LEFT_CHANNEL_B_PIN 40
#define RIGHT_CHANNEL_A_PIN 39
#define RIGHT_CHANNEL_B_PIN 41
#define DCMOTORDRIVERL298_PIN_ENA  2
#define DCMOTORDRIVERL298_PIN_ENB  3
#define DCMOTORDRIVERL298_PIN_INT1 4
#define DCMOTORDRIVERL298_PIN_INT2 5
#define DCMOTORDRIVERL298_PIN_INT3 6
#define DCMOTORDRIVERL298_PIN_INT4 7

// Object initialization
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA, DCMOTORDRIVERL298_PIN_INT1, DCMOTORDRIVERL298_PIN_INT2, DCMOTORDRIVERL298_PIN_ENB, DCMOTORDRIVERL298_PIN_INT3, DCMOTORDRIVERL298_PIN_INT4);

volatile long leftEncoderCount = 0;
volatile long rightEncoderCount = 0;

void setup() {
    // Setup Serial (optional, for debugging)
    Serial.begin(9600);
    while (!Serial); // Wait for serial port to connect. Needed for native USB
    Serial.println("Press 's' to start routine.");

    // Attach interrupts for left and right encoders
    attachInterrupt(digitalPinToInterrupt(LEFT_CHANNEL_A_PIN), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_CHANNEL_A_PIN), rightEncoderISR, CHANGE);
}

void loop() {
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 's' || c == 'S') {
            startRoutine();
        }
    }
}

void startRoutine() {
    // Reset encoder counts
    leftEncoderCount = 0;
    rightEncoderCount = 0;

    // Routine: Forward
    Serial.println("Moving Forward");
    dcMotorDriverL298.setMotorA(255, 1); // Set Motor A speed and direction
    dcMotorDriverL298.setMotorB(255, 1); // Set Motor B speed and direction
    delay(3000); // Move forward for 3 seconds

    // Display rotation counts
    Serial.print("Left Motor Rotations: ");
    Serial.println(leftEncoderCount);
    Serial.print("Right Motor Rotations: ");
    Serial.println(rightEncoderCount);

    // Routine: Stop
    Serial.println("Stopping");
    dcMotorDriverL298.stopMotors(); // Stop both motors
    delay(1000); // Stop for 1 second

    // Routine: Backward
    Serial.println("Moving Backward");
    dcMotorDriverL298.setMotorA(255, 0); // Set Motor A speed and reverse direction
    dcMotorDriverL298.setMotorB(255, 0); // Set Motor B speed and reverse direction
    delay(3000); // Move backward for 3 seconds

    // Display rotation counts
    Serial.print("Left Motor Rotations: ");
    Serial.println(leftEncoderCount);
    Serial.print("Right Motor Rotations: ");
    Serial.println(rightEncoderCount);

    // Routine: Stop
    Serial.println("Stopping");
    dcMotorDriverL298.stopMotors(); // Stop both motors
    delay(1000); // Stop for 1 second

    // Routine: Turn Right
    Serial.println("Turning Right");
    dcMotorDriverL298.setMotorA(255, 1); // Motor A forward
    dcMotorDriverL298.setMotorB(200, 0); // Motor B backward for a sharp turn
    delay(2000); // Turn right for 2 seconds

    // Display rotation counts
    Serial.print("Left Motor Rotations: ");
    Serial.println(leftEncoderCount);
    Serial.print("Right Motor Rotations: ");
    Serial.println(rightEncoderCount);

    // Routine: Stop and End of Routine
    Serial.println("Stopping");
    dcMotorDriverL298.stopMotors(); // Stop both motors
    delay(1000); // Stop for 1 second

    Serial.println("Routine Complete");
}

void leftEncoderISR() {
    leftEncoderCount++;
}

void rightEncoderISR() {
    rightEncoderCount++;
}
