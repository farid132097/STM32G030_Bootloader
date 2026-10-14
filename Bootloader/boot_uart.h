


#ifndef  _BOOT_UART_H_
#define  _BOOT_UART_H_
#include "stdint.h"


void     Boot_UART_GPIO_Init(void);
void     Boot_UART_GPIO_DeInit(void);
void     Boot_UART_Reg_Init(uint32_t baud_rate);
void     Boot_UART_Reg_DeInit(void);
void     Boot_UART_Tx_Byte(uint8_t val);
uint8_t  Boot_UART_Rx_Byte(void);
void     Boot_UART_Tx_Buf(uint8_t *data, uint8_t start, uint8_t len);
void     Boot_UART_Init(uint32_t baud_rate);
void     Boot_UART_DeInit(void);



#endif


