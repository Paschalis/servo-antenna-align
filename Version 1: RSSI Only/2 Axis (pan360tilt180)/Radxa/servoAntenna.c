#include <stdio.h>
#include <gpiod.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define PAN_DIR_LINE 25   // GPIO line for BLDC direction (CW/CCW)
#define PAN_PWM_LINE 26   // GPIO line for BLDC PWM control
#define TILT_LINE 24      // GPIO line for tilt servo
#define ENABLE_LINE 23    // GPIO line to enable motor
#define STEP_DELAY 50000  // Microseconds between steps
#define MIN_TILT 0        // Minimum tilt angle
#define MAX_TILT 180      // Maximum tilt angle
#define TILT_STEP 10      // Tilt angle step size
#define MIN_SIGNAL -100   // Minimum signal strength

// Function to set servo angle
void setTiltAngle(struct gpiod_line *line, int angle) {
    int pulseWidth = (angle * 10) + 600; // Convert angle to pulse width in microseconds
    gpiod_line_set_value(line, 1); // Start PWM pulse
    usleep(pulseWidth); // Hold high for pulse duration
    gpiod_line_set_value(line, 0); // End PWM pulse
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
void setPanMotor(struct gpiod_line *dirLine, struct gpiod_line *pwmLine, struct gpiod_line *enableLine, int direction, int speed) {
    gpiod_line_set_value(enableLine, 1); // Enable motor
    gpiod_line_set_value(dirLine, direction); // Set direction (1 = CW, 0 = CCW)
    gpiod_line_set_value(pwmLine, speed); // Set PWM duty cycle (0-255)
}

// Find the best signal using pan and tilt
void findBestSignal(struct gpiod_line *panDirLine, struct gpiod_line *panPwmLine, struct gpiod_line *tiltLine, struct gpiod_line *enableLine) {
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
        setTiltAngle(tiltLine, tiltAngle);
        usleep(500000); // Wait for servo to stabilize

        // Sweep through pan angles
        for (panAngle = 0; panAngle < 360; panAngle += 10) {
            setPanMotor(panDirLine, panPwmLine, enableLine, panDirection, 128); // 50% speed
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
    setTiltAngle(tiltLine, bestTiltAngle);
    setPanMotor(panDirLine, panPwmLine, enableLine, bestPanAngle >= 180, 128); // CW or CCW
    usleep(STEP_DELAY * abs(bestPanAngle));
    gpiod_line_set_value(enableLine, 0); // Disable motor
}

int main() {
    struct gpiod_chip *chip = gpiod_chip_open(GPIO_CHIP);
    struct gpiod_line *panDirLine = gpiod_chip_get_line(chip, PAN_DIR_LINE);
    struct gpiod_line *panPwmLine = gpiod_chip_get_line(chip, PAN_PWM_LINE);
    struct gpiod_line *tiltLine = gpiod_chip_get_line(chip, TILT_LINE);
    struct gpiod_line *enableLine = gpiod_chip_get_line(chip, ENABLE_LINE);

    if (gpiod_line_request_output(panDirLine, "pan_dir", 0) < 0 ||
        gpiod_line_request_output(panPwmLine, "pan_pwm", 0) < 0 ||
        gpiod_line_request_output(tiltLine, "tilt_servo", 0) < 0 ||
        gpiod_line_request_output(enableLine, "motor_enable", 0) < 0) {
        printf("Failed to request GPIO lines\n");
        return 1;
    }

    findBestSignal(panDirLine, panPwmLine, tiltLine, enableLine);

    gpiod_chip_close(chip);
    return 0;
}
