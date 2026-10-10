

#include "stm32g030xx.h"
#include "flash.h"


void Flash_Enable_RCC(void)
{
	RCC->AHBENR|=RCC_AHBENR_FLASHEN;
}

void Flash_Disable_RCC(void)
{
	RCC->AHBENR&=~RCC_AHBENR_FLASHEN;
}

void Flash_Wait_Busy_Config(void)
{
	while((FLASH->SR & FLASH_SR_CFGBSY)!=0)
	{
		//add timeout
	}
}

void Flash_Wait_Busy1(void)
{
	while((FLASH->SR & FLASH_SR_BSY1)!=0)
	{
		//add timeout
	}
}

void Flash_Lock(void)
{
	FLASH->CR|= FLASH_CR_LOCK;
}

void Flash_Unlock(void)
{
	Flash_Wait_Busy1();
	if(FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}
void Flash_Clear_OPTVERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_OPTVERR)
	{
		FLASH->SR |= FLASH_SR_OPTVERR;
	}
}

void Flash_Clear_FASTERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_FASTERR)
	{
		FLASH->SR |= FLASH_SR_FASTERR;
	}
}

void Flash_Clear_MISERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_MISERR)
	{
		FLASH->SR |= FLASH_SR_MISERR;
	}
}

void Flash_Clear_PGSERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_PGSERR)
	{
		FLASH->SR |= FLASH_SR_PGSERR;
	}
}

void Flash_Clear_SIZERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_SIZERR)
	{
		FLASH->SR |= FLASH_SR_SIZERR;
	}
}

void Flash_Clear_PGAERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_PGAERR)
	{
		FLASH->SR |= FLASH_SR_PGAERR;
	}
}

void Flash_Clear_WRPERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_WRPERR)
	{
		FLASH->SR |= FLASH_SR_WRPERR;
	}
}

void Flash_Clear_PROGERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_PROGERR)
	{
		FLASH->SR |= FLASH_SR_PROGERR;
	}
}

void Flash_Clear_OPERR_Flag(void)
{
	if(FLASH->SR & FLASH_SR_OPERR)
	{
		FLASH->SR |= FLASH_SR_OPERR;
	}
}

void Flash_Clear_EOP_Flag(void)
{
	if(FLASH->SR & FLASH_SR_EOP)
	{
		FLASH->SR |= FLASH_SR_EOP;
	}
}

void Flash_Clear_All_Error_Flags(void)
{
	Flash_Clear_OPTVERR_Flag();
	Flash_Clear_FASTERR_Flag();
	Flash_Clear_MISERR_Flag();
	Flash_Clear_PGSERR_Flag();
	Flash_Clear_SIZERR_Flag();
	Flash_Clear_PGAERR_Flag();
	Flash_Clear_WRPERR_Flag();
	Flash_Clear_PROGERR_Flag();
	Flash_Clear_OPERR_Flag();
	Flash_Clear_EOP_Flag();
}

//Address is byte oriented, OR offset by 0x08000000
void Flash_Erase_Page(uint32_t byte_addr)
{
	Flash_Enable_RCC();                  
	Flash_Unlock();
	Flash_Clear_All_Error_Flags();
	//clculate PNB from addr
	uint32_t pnb = byte_addr;
	//clear memory mapping offset, offset by 0x08000000
	pnb &= 0x00FFFFFF;
	//devide by page size, 2048 bytes
	pnb /= 0x800;
	//set PER in CR
	FLASH->CR|=FLASH_CR_PER;
	//set PNB in CR
	FLASH->CR|=(pnb<<FLASH_CR_PNB_Pos);
	//set START in CR
	FLASH->CR|=FLASH_CR_STRT;
	//wait BSY1s
	Flash_Wait_Busy1();
	FLASH->CR&=~FLASH_CR_PER;
	Flash_Lock();
	Flash_Disable_RCC();
}

uint64_t Flash_Read_DWord(uint32_t addr)
{
	uint64_t data = *(__IO uint32_t*)(addr);
	data <<= 32;
	data|= *(__IO uint32_t*)(addr+4);
	return data;
}

void Flash_Write_DWord(uint32_t addr, uint64_t data)
{
	uint64_t curr_val;
	curr_val = Flash_Read_DWord(addr);
	if(curr_val != data){
		Flash_Enable_RCC();
		__disable_irq();
		Flash_Unlock();
		Flash_Wait_Busy1();
		if(FLASH->SR & FLASH_SR_CFGBSY)
		{
			*(__IO uint32_t*)(FLASH_BASE) = 0xFFFFFFFF;
		}
		Flash_Clear_All_Error_Flags();
		Flash_Wait_Busy_Config();
		FLASH->CR |= FLASH_CR_PG;
		*(__IO uint32_t*)(addr) = (data >> 32);
		*(__IO uint32_t*)(addr+4) = (data & 0xFFFFFFFF);
		Flash_Wait_Busy1();
		Flash_Wait_Busy_Config();
		Flash_Clear_EOP_Flag();
		FLASH->CR&=~FLASH_CR_PG;
		__enable_irq();
		Flash_Lock();
		Flash_Disable_RCC();
	}
}