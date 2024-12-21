#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAN_DIR_PIN 25   // GPIO pin for BLDC direction (CW/CCW)
#define PAN_PWM_PIN 26   // GPIO pin for BLDC PWM control
#define TILT_PIN 24      // GPIO pin for tilt servo
#define ENABLE_PIN 23    // GPIO pin to enable motor
#define STEP_DELAY 50000 // Microseconds between steps
#define MIN_TILT 0       // Minimum tilt angle
#define MAX_TILT 180     // Maximum tilt angle
#define TILT_STEP 10     // Tilt angle step size
#define MIN_SIGNAL -100  // Minimum signal strength

// Function to set servo angle
void setTiltAngle(int pin, int angle) {
    int pulseWidth = (angle * 10) + 600; // Convert angle to pulse width in microseconds
    digitalWrite(pin, HIGH); // Start PWM pulse
    usleep(pulseWidth);      // Hold high for pulse duration
    digitalWrite(pin, LOW);  // End PWM pulse
    usleep(20000 - pulseWidth); // Wait for the rest of the 20ms cycle
}

// Function to get signal strength
int getSignalStrength() {
    FILE *fp;
    char signalStr[10];
    int signalStrength = MIN_SIGNAL;

    fp = popen("iwconfig wlan0 | grep 'Signal level' | awk '{print $4}' | cut -d'=' -f2", "r");
    if (fp != NULL && fgets(signalStr, sizeof(signalStr) - 1, fp) != NULL) {
        signalStrength = atoi(signalStr);
        pclose(fp);
    }
    return signalStrength;
}

// Function to rotate BLDC motor
void setPanMotor(int dirPin, int pwmPin, int enablePin, int direction, int speed) {
    digitalWrite(enablePin, HIGH); // Enable motor
    digitalWrite(dirPin, direction); // Set direction (1 = CW, 0 = CCW)
    pwmWrite(pwmPin, speed); // Set PWM duty cycle (0-1024 for WiringPi PWM)
}

// Find the best signal using pan and tilt
void findBestSignal(int panDirPin, int panPwmPin, int tiltPin, int enablePin) {
    int bestSignal = MIN_SIGNAL;
    int currentSignal;
    int bestPanAngle = 0;
    int bestTiltAngle = 0;
    int panDirection = 1; // Start with CW
    int panAngle = 0;
    int tiltAngle = MIN_TILT;

    printf("Starting signal scan...\n");

    // Sweep through tilt angles
    for (tiltAngle = MIN_TILT; tiltAngle <= MAX_TILT; tiltAngle += TILT_STEP) {
        setTiltAngle(tiltPin, tiltAngle);
        usleep(500000); // Wait for servo to stabilize

        // Sweep through pan angles
        for (panAngle = 0; panAngle < 360; panAngle += 10) {
            setPanMotor(panDirPin, panPwmPin, enablePin, panDirection, 512); // 50% speed
            usleep(STEP_DELAY);

            currentSignal = getSignalStrength();
            printf("Tilt: %d°, Pan: %d°, Signal Strength: %d dBm\n", tiltAngle, panAngle, currentSignal);

            if (currentSignal > bestSignal) {
                bestSignal = currentSignal;
                bestPanAngle = panAngle;
                bestTiltAngle = tiltAngle;
            }

            if (panAngle >= 360) {
                panDirection = !panDirection; // Reverse direction for next sweep
                panAngle = 0;
            }
        }
    }

    printf("Best Tilt Angle: %d°, Best Pan Angle: %d°, Best Signal: %d dBm\n", bestTiltAngle, bestPanAngle, bestSignal);

    // Move to the best angles
    setTiltAngle(tiltPin, bestTiltAngle);
    setPanMotor(panDirPin, panPwmPin, enablePin, bestPanAngle >= 180, 512); // CW or CCW
    usleep(STEP_DELAY * abs(bestPanAngle));
    digitalWrite(enablePin, LOW); // Disable motor
}

int main() {
    // Initialize WiringPi
    if (wiringPiSetupGpio() == -1) {
        printf("Failed to initialize WiringPi\n");
        return 1;
    }

    // Set pin modes
    pinMode(PAN_DIR_PIN, OUTPUT);
    pinMode(PAN_PWM_PIN, PWM_OUTPUT);
    pinMode(TILT_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    // Start signal scan
    findBestSignal(PAN_DIR_PIN, PAN_PWM_PIN, TILT_PIN, ENABLE_PIN);

    return 0;
}
