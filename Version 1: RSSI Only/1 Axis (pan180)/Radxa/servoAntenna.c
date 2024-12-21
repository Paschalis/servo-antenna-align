#include <stdio.h>
#include <gpiod.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define SERVO_LINE 24       // GPIO line connected to the servo
#define MIN_ANGLE 0         // Minimum servo angle
#define MAX_ANGLE 180       // Maximum servo angle
#define STEP_ANGLE 5        // Step size for angle adjustments

void setServoAngle(struct gpiod_line *line, int angle) {
    int pulseWidth = (angle * 10) + 600; 
    gpiod_line_set_value(line, 1); 
    usleep(pulseWidth * 1000);     
    gpiod_line_set_value(line, 0);
    usleep((20000 - pulseWidth) * 1000); 
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
    struct gpiod_chip *chip = gpiod_chip_open(GPIO_CHIP);
    struct gpiod_line *line = gpiod_chip_get_line(chip, SERVO_LINE);

    if (gpiod_line_request_output(line, "servo", 0) < 0) {
        printf("Failed to request GPIO line\n");
        return 1;
    }

    int angle = MIN_ANGLE;
    int bestAngle = MIN_ANGLE;
    int bestSignal = -100;

    printf("Starting antenna alignment...\n");

    for (angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += STEP_ANGLE) {
        setServoAngle(line, angle);
        usleep(500000); 

        int currentSignal = getSignalStrength();
        printf("Angle: %d°, Signal Strength: %d dBm\n", angle, currentSignal);

        if (currentSignal > bestSignal) {
            bestSignal = currentSignal;
            bestAngle = angle;
        }
    }

    printf("Optimal Angle: %d° with Signal Strength: %d dBm\n", bestAngle, bestSignal);

    return 0;
}
