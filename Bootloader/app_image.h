

#ifndef  _APP_IMAGE_H_
#define  _APP_IMAGE_H_
#include "stdint.h"


void     App_Image_Init_Data(void);
void     App_Image_Skip_Until_Data(void);
uint8_t  App_Image_Char_To_Num(uint8_t char_data);
uint8_t  App_Image_Checksum_Get(void);
uint8_t  App_Image_Byte_Count_Get(void);
uint16_t App_Image_Addr_Offset_Get(void);
uint16_t App_Image_Rec_Type_Get(void);
void     App_Image_Copy_Data_Block(void);
void     App_Image_Segreget_Fields(void);
uint8_t  App_Image_Byte_Cnt_Get(void);
uint32_t App_Image_Current_Addr_Get(void);
uint64_t App_Image_Data_Get(uint8_t index);

#endif


