

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


#endif


