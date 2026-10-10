

#ifndef  _FLASH_H_
#define  _FLASH_H_
#include "stdint.h"


void     Flash_Enable_RCC(void);
void     Flash_Disable_RCC(void);
void     Flash_Wait_Busy_Config(void);
void     Flash_Wait_Busy1(void);
void     Flash_Lock(void);
void     Flash_Unlock(void);
void     Flash_Clear_OPTVERR_Flag(void);
void     Flash_Clear_FASTERR_Flag(void);
void     Flash_Clear_MISERR_Flag(void);
void     Flash_Clear_PGSERR_Flag(void);
void     Flash_Clear_SIZERR_Flag(void);
void     Flash_Clear_PGAERR_Flag(void);
void     Flash_Clear_WRPERR_Flag(void);
void     Flash_Clear_PROGERR_Flag(void);
void     Flash_Clear_OPERR_Flag(void);
void     Flash_Clear_EOP_Flag(void);
void     Flash_Clear_All_Error_Flags(void);
void     Flash_Erase_Page(uint32_t byte_addr);
uint64_t Flash_Read_DWord(uint32_t addr);
void     Flash_Write_DWord(uint32_t addr, uint64_t data);


#endif



