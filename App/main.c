

#include "stm32g030xx.h"
#include "tim.h"

int main(void)
{
	
	SCB->VTOR = 0x08002000UL;
	
	// LED on PA4
    RCC->IOPENR  |=  RCC_IOPENR_GPIOAEN;
	GPIOA->ODR   &=~ GPIO_ODR_OD4;
	GPIOA->MODER &=~ GPIO_MODER_MODE4_Msk;
	GPIOA->MODER |=  GPIO_MODER_MODE4_0;
	
	
	Timer_Init();
	
    while (1)
	{
		
	}
}



void TIM16_IRQHandler(void){
	TIM16->SR  &=~ TIM_SR_UIF;
	GPIOA->ODR ^=  GPIO_ODR_OD4;
}

