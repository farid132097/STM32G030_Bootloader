

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
	
	uint8_t button_cnt = 0;
	
	// LED on PA6
    RCC->IOPENR  |=  RCC_IOPENR_GPIOAEN;
	
	//button3 aas input
	GPIOA->MODER &=~ GPIO_MODER_MODE8_Msk;
	
	//LED3 as output high
	GPIOA->ODR   |=  GPIO_ODR_OD6;
	GPIOA->MODER &=~ GPIO_MODER_MODE6_Msk;
	GPIOA->MODER |=  GPIO_MODER_MODE6_0;
	
	
	App_Image_Init_Data();
	App_Image_Segreget_Fields();
	
	
	
    while (1)
	{
		
		if((GPIOA->IDR & GPIO_IDR_ID8) == 0)
		{
			for(uint32_t i = 0; i < 50000; i++)
			{
				__NOP();
			}
			button_cnt++;
			while((GPIOA->IDR & GPIO_IDR_ID8) == 0);
			
			if(button_cnt == 1)
			{
				//write data
				/*do{
					App_Image_Segreget_Fields();
					//Flash_Write_DWord(App_Image_Current_Addr_Get() + 0x00, App_Image_Data_Get(0));
					//Flash_Write_DWord(App_Image_Current_Addr_Get() + 0x08, App_Image_Data_Get(1));
				}
				while(App_Image_Byte_Cnt_Get() != 16);*/
			}
			App_Image_Segreget_Fields();
			Flash_Write_DWord(App_Image_Current_Addr_Get() + 0x00, App_Image_Data_Get(0));
			Flash_Write_DWord(App_Image_Current_Addr_Get() + 0x08, App_Image_Data_Get(1));
		}
		
		
		GPIOA->ODR   ^=  GPIO_ODR_OD6;
		
		if(button_cnt == 0)
		{
			for(uint32_t i = 0; i < 50000; i++)
			{
				__NOP();
			}
		}
		else if(button_cnt == 1)
		{
			for(uint32_t i = 0; i <100000; i++)
			{
				__NOP();
			}
		}
		else if(button_cnt == 2)
		{
			//Jump_To_App();
		}
		
	}
}

