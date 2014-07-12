#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_tim.h>

//Define onboard LED's
#define GreenLED GPIO_Pin_8
#define BlueLED GPIO_Pin_9
#define LEDGPIO GPIOC
#define LEDToggleValue ((uint16_t) 3000)

//Initialization structs
GPIO_InitTypeDef LEDs;
TIM_TimeBaseInitTypeDef TTB;

int main(void)
{
	//Initialize system and ensure core clock is up to date
	SystemInit();
	SystemCoreClockUpdate();

	//Enable GPIO Clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//Initialize LEDs
	LEDs.GPIO_Pin = GreenLED | BlueLED;
	LEDs.GPIO_Mode = GPIO_Mode_OUT;
	LEDs.GPIO_OType = GPIO_OType_PP;
	LEDs.GPIO_PuPd = GPIO_PuPd_NOPULL;
	LEDs.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(LEDGPIO, &LEDs);

	//Initialize and enable timer to have a maximum period for 16bits
	//running at a frequency of 48MHz/(65535*(1000 - 1)) = 0.732Hz
	TTB.TIM_ClockDivision = TIM_CKD_DIV1;
	TTB.TIM_CounterMode = TIM_CounterMode_Up;
	TTB.TIM_Period = 0xFFFF;
	TTB.TIM_Prescaler = 1000;
	TTB.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TTB);
	TIM_Cmd(TIM1, ENABLE);

	uint16_t CurrentTimerVal = 0;

	while(1)
	{
		//Store current timer value in variable
		CurrentTimerVal = TIM_GetCounter(TIM1);

		//See if current timer value is more than LED toggle value
		if(CurrentTimerVal>LEDToggleValue){
			GPIO_SetBits(LEDGPIO, GreenLED);
			GPIO_ResetBits(LEDGPIO, BlueLED);
		}
		else{
			GPIO_ResetBits(LEDGPIO, GreenLED);
			GPIO_SetBits(LEDGPIO, BlueLED);
		}
	}
}
