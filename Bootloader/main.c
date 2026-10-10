

#include "stm32g030xx.h"
#include "app_image.h"
#include "flash.h"

#define  APP_ADDR 0x08002000UL

void Jump_To_App(void)
{
	uint32_t app_sp = *(volatile uint32_t *)(APP_ADDR);
    uint32_t entry  = *(volatile uint32_t *)(APP_ADDR + 4);
    
	GPIOA->ODR   &=~ GPIO_ODR_OD6;
	GPIOA->MODER &=~ GPIO_MODER_MODE6_Msk;
	
	
	//compute crc instead of this check
    if ((app_sp & 0x2FFE0000) != 0x20000000)
	{
		return;
	}
	
    SCB->VTOR = APP_ADDR;
	
    __asm volatile 
	(
		"MSR   MSP, %0 \n"
		"BX    %1      \n"
		:                      
		: "r"(app_sp), "r"(entry)
	);
	
    ((void (*)(void))entry)();
}


int main(void)
{
	
	// LED on PA4
    RCC->IOPENR  |=  RCC_IOPENR_GPIOAEN;
	
	//button3 aas input
	GPIOA->MODER &=~ GPIO_MODER_MODE8_Msk;
	
	//LED3 as output high
	GPIOA->ODR   |=  GPIO_ODR_OD6;
	GPIOA->MODER &=~ GPIO_MODER_MODE6_Msk;
	GPIOA->MODER |=  GPIO_MODER_MODE6_0;
	
	
    while (1)
	{
		
		GPIOA->ODR   ^=  GPIO_ODR_OD6;
		for(uint32_t i = 0; i < 50000; i++)
		{
			__NOP();
		}
		
		if((GPIOA->IDR & GPIO_IDR_ID8) == 0)
		{
			Jump_To_App();
		}
		
	}
}

