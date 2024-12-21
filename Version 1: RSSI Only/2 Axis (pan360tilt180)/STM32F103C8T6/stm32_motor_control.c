#include "stm32f10x.h"

// Define pin connections
#define PWM_PIN GPIO_Pin_0     // PA0 for PWM
#define DIR_PIN GPIO_Pin_1     // PA1 for Direction
#define ENABLE_PIN GPIO_Pin_2  // PA2 for Enable

void setup() {
    // Enable GPIO and TIM2 clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // Configure GPIOA pins for PWM, Direction, and Enable
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = PWM_PIN | DIR_PIN | ENABLE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure Timer2 for PWM
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 255;             // 8-bit resolution
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       // 1 MHz frequency
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // Configure PWM mode on Channel 1
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // Start with 0% duty cycle
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // Start Timer2
    TIM_Cmd(TIM2, ENABLE);
}

void setMotorSpeed(uint8_t direction, uint8_t speed) {
    if (direction) {
        GPIO_SetBits(GPIOA, DIR_PIN);  // CW direction
    } else {
        GPIO_ResetBits(GPIOA, DIR_PIN);  // CCW direction
    }

    GPIO_SetBits(GPIOA, ENABLE_PIN);    // Enable motor
    TIM_SetCompare1(TIM2, speed);       // Set PWM duty cycle
}

int main() {
    setup();
    while (1) {
        // Example: Rotate CW at 50% speed for demonstration
        setMotorSpeed(1, 128);
        for (int i = 0; i < 5000000; i++);  // Delay

        // Example: Rotate CCW at 50% speed for demonstration
        setMotorSpeed(0, 128);
        for (int i = 0; i < 5000000; i++);  // Delay
    }
}
