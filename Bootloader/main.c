

#include "stm32g030xx.h"
#include "app_image.h"
#include "boot_uart.h"
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
	
	Boot_UART_Init(38400);
	
	
	
    while (1)
	{
		
		for(uint32_t i = 0; i < 50000; i++)
		{
			__NOP();
		}
		
	}
}

