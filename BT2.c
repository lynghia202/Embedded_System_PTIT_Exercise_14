#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

static volatile uint32_t ms_ticks = 0;

void Disable_Periph(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);

}

void SysTick_Handler(void) {
		ms_ticks++;
		static uint32_t cnt = 0;
		cnt++;
		if (cnt >= 1000) {
		cnt = 0;
		GPIOC->ODR ^= GPIO_Pin_13;
			}
}

void LED_Init(void) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_InitTypeDef gpio;
		gpio.GPIO_Pin = GPIO_Pin_13;
		gpio.GPIO_Speed = GPIO_Speed_2MHz;
		gpio.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &gpio);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

int main(void) {
		SystemInit();
		SystemCoreClockUpdate();
		LED_Init();
		SysTick_Config(SystemCoreClock / 1000);
		Disable_Periph();

while (1) {
		__WFI(); 
			}
}
