/**
  ******************************************************************************
  * @file    main.c
  * @author  Mertcan Ekiz
  * @version V1.0.0
  * @date    21-Nov-2019
  * @brief   Lamp Simulator
  *
  *****************************************************************************/
#include "main.h"
#include <stm32f0xx.h>

void LEDToggle(uint16_t delay);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#define RESET_DELAY 3000 
#define FIRST_STATE 0
typedef enum {
    STATE_RED_A = FIRST_STATE,
    STATE_RED_B,
    STATE_LOW_A,
    STATE_LOW_B,
    STATE_HIGH_A,
    STATE_HIGH_B,
    STATE_OFF_A,
    STATE_OFF_B
} STATE;

uint8_t state = STATE_OFF_B;
uint16_t counter = 0;
uint16_t timer = 0;
static __IO uint32_t TimingDelay;

int main(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);


    while (1)
    {
        switch (state)
        {
            case STATE_RED_A:
            case STATE_RED_B:
                STM_EVAL_LEDOff(LED3);
                STM_EVAL_LEDOn(LED4);
                timer++;
                break;
            case STATE_LOW_A:
            case STATE_LOW_B:
                LEDToggle(200);
                timer++;
                break;
            case STATE_HIGH_A:
            case STATE_HIGH_B:
                LEDToggle(100);
                timer++; // This isn't technically needed.
                break;
            case STATE_OFF_A:
            case STATE_OFF_B:
                STM_EVAL_LEDOff(LED3);
                STM_EVAL_LEDOff(LED4);
                break;
            default:
                state = FIRST_STATE;
                break;
        }
        if (STM_EVAL_PBGetState(BUTTON_USER) == (state % 2)) {
        // ^^^ This conditional is equivalent to:
        //
        //    if ((STM_EVAL_PBGetState(BUTTON_USER) == 1 && (state % 2 == 1)) ||
        //       (STM_EVAL_PBGetState(BUTTON_USER) == 0 && (state % 2 == 0))) {
        //
        // Which means that an odd numbered state will go to the next state on a button press,
        // whereas an even numbered state will go to the next state after a button release.
            if (timer > RESET_DELAY) {
                state = STATE_OFF_A;
                timer = 0;
                continue;
            }
            if (state == STATE_RED_B) {
                STM_EVAL_LEDOff(LED4);
            }
            state++;
            timer = 0;
        }
        Delay(1);
    }
}

void LEDToggle(uint16_t delay)
{
    if (++counter > delay) {
        STM_EVAL_LEDToggle(LED3);
        counter = 0;
    }
}

void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;
    while (TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
