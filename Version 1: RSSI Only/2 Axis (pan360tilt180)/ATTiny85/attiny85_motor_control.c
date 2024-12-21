#include <avr/io.h>
#include <util/delay.h>

// Pin definitions for ATtiny85
#define PWM_PIN PB1    // Pin for PWM signal
#define DIR_PIN PB2    // Pin for direction control
#define EN_PIN PB0     // Pin for motor enable control

void setup() {
    // Set PWM, direction, and enable pins as output
    DDRB |= (1 << PWM_PIN) | (1 << DIR_PIN) | (1 << EN_PIN);

    // Configure Timer1 for PWM
    TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (1 << CS10);  // Fast PWM, no prescaler
    GTCCR = (1 << PWM1B);  // Enable PWM for Timer1
}

void setMotorSpeed(uint8_t direction, uint8_t speed) {
    // Set motor direction
    if (direction) {
        PORTB |= (1 << DIR_PIN);  // CW direction
    } else {
        PORTB &= ~(1 << DIR_PIN); // CCW direction
    }

    // Set motor speed (PWM duty cycle)
    OCR1A = speed;
}

void enableMotor() {
    PORTB |= (1 << EN_PIN);  // Enable motor
}

void disableMotor() {
    PORTB &= ~(1 << EN_PIN); // Disable motor
}

int main() {
    setup();

    // Enable motor
    enableMotor();

    while (1) {
        // Rotate CW at 50% speed
        setMotorSpeed(1, 128);  // CW direction, 50% speed
        _delay_ms(2000);

        // Rotate CCW at 50% speed
        setMotorSpeed(0, 128);  // CCW direction, 50% speed
        _delay_ms(2000);
    }
}
