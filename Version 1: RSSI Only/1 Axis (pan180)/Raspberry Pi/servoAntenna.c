#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>

#define SERVO_PIN 18      // GPIO pin connected to the servo
#define MIN_ANGLE 0       // Minimum servo angle
#define MAX_ANGLE 180     // Maximum servo angle
#define STEP_ANGLE 5      // Step size for angle adjustments

void setServoAngle(int angle) {
    int pulseWidth = (angle * 10) + 600; // Convert angle to pulse width
    pwmWrite(SERVO_PIN, pulseWidth);
}

int getSignalStrength() {
    FILE *fp;
    char path[1035];
    char signalStr[10];
    int signalStrength = -100; // Initialize signal strength to a very low value

    // Execute the command to fetch RSSI using iwconfig for wlan0
    fp = popen("iwconfig wlan0 | grep 'Signal level' | awk '{print $4}' | cut -d'=' -f2", "r");
    if (fp == NULL) {
        printf("Failed to get RSSI\n");
        return signalStrength;
    }

    // Read the output of the command into signalStr
    if (fgets(signalStr, sizeof(signalStr) - 1, fp) != NULL) {
        signalStrength = atoi(signalStr);  // Convert string to int
    }

    // Close the file pointer and return the signal strength
    pclose(fp);
    return signalStrength;
}


int main() {
    if (wiringPiSetupGpio() == -1) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }

    pinMode(SERVO_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);

    int angle = MIN_ANGLE;
    int bestAngle = MIN_ANGLE;
    int bestSignal = -100;

    printf("Starting antenna alignment...\n");

    for (angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += STEP_ANGLE) {
        setServoAngle(angle);
        delay(500);

        int currentSignal = getSignalStrength();
        printf("Angle: %d°, Signal Strength: %d dBm\n", angle, currentSignal);

        if (currentSignal > bestSignal) {
            bestSignal = currentSignal;
            bestAngle = angle;
        }
    }

    printf("Optimal Angle: %d° with Signal Strength: %d dBm\n", bestAngle, bestSignal);
    setServoAngle(bestAngle);

    return 0;
}