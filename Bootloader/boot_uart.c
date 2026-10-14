


#include "stm32g030xx.h"
#include "boot_uart.h"
#include "cdefs.h"

typedef struct boot_uart_t
{
	volatile uint8_t    RxPktBuf[64];
	volatile uint8_t    RxPktBufIndex;
	volatile uint8_t    RxPktCrcSts;
}boot_uart_t;


boot_uart_t BootUART;


void Boot_UART_Struct_Init(void)
{
	BootUART.RxPktBufIndex = 0;
	BootUART.RxPktCrcSts = FAILED;
}


void Boot_UART_GPIO_Init(void)
{
	//HW UART2, TX->PA2, RX->PA3
	if((RCC->IOPENR & RCC_IOPENR_GPIOAEN) != RCC_IOPENR_GPIOAEN){
		RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	}
	
	//init as input mode
	GPIOA->MODER  &=~ GPIO_MODER_MODE2_Msk;
	GPIOA->MODER  &=~ GPIO_MODER_MODE3_Msk;
	
	//select PA2->AF1, PA3->AF1
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFSEL2_Msk;
	GPIOA->AFR[0] |=  (1 << GPIO_AFRL_AFSEL2_Pos);
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFSEL3_Msk;
	GPIOA->AFR[0] |=  (1 << GPIO_AFRL_AFSEL3_Pos);
	
	//enable pull-up
	GPIOA->PUPDR  &=~ GPIO_PUPDR_PUPD2_Msk;
	GPIOA->PUPDR  |=  (1 << GPIO_PUPDR_PUPD2_Pos);
	GPIOA->PUPDR  &=~ GPIO_PUPDR_PUPD3_Msk;
	GPIOA->PUPDR  |=  (1 << GPIO_PUPDR_PUPD3_Pos);
}

void Boot_UART_GPIO_DeInit(void)
{
	//disable pull-up
	GPIOA->PUPDR  &=~ GPIO_PUPDR_PUPD2_Msk;
	GPIOA->PUPDR  &=~ GPIO_PUPDR_PUPD3_Msk;
	
	//clear AFR regs
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFSEL2_Msk;
	GPIOA->AFR[0] &=~ GPIO_AFRL_AFSEL3_Msk;
	
	//analog mode
	GPIOA->MODER  |=  GPIO_MODER_MODE2_Msk;
	GPIOA->MODER  |=  GPIO_MODER_MODE3_Msk;
	
	//clear rcc
	if((RCC->IOPENR & RCC_IOPENR_GPIOAEN) == RCC_IOPENR_GPIOAEN){
		RCC->IOPENR &=~ RCC_IOPENR_GPIOAEN;
	}
}

void Boot_UART_Reg_Init(uint32_t baud_rate)
{
	RCC->APBENR1  |=  RCC_APBENR1_USART2EN;
	SystemCoreClockUpdate();
	USART2->BRR   =  (uint16_t)(SystemCoreClock/baud_rate);
	USART2->CR1   &=~ USART_CR1_UE;
	USART2->CR1   |=  USART_CR1_TE;
	USART2->CR1   |=  USART_CR1_RE;
	USART2->CR1   |=  USART_CR1_UE;
	USART2->CR1   |=  USART_CR1_RXNEIE_RXFNEIE;
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
}

void Boot_UART_Reg_DeInit(void)
{
	USART2->CR1   &=~ USART_CR1_RXNEIE_RXFNEIE;
	USART2->CR1   &=~ USART_CR1_RE;
	USART2->CR1   &=~ USART_CR1_TE;
	USART2->CR1   &=~ USART_CR1_UE;
	NVIC_DisableIRQ(USART2_IRQn);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	RCC->APBENR1  &=~ RCC_APBENR1_USART2EN;
}


void Boot_UART_Tx_Byte(uint8_t val)
{
	USART2->TDR = (uint8_t)val;
	while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC);
	USART2->ICR|=USART_ICR_TCCF;
}

uint8_t Boot_UART_Rx_Byte(void)
{
	uint8_t val = 0;
	val = (uint8_t)USART2->RDR;
	return val;
}


void Boot_UART_Tx_Buf(uint8_t *data, uint8_t start, uint8_t len)
{
	uint8_t loop_cnt;
	len += start;
	for(loop_cnt = 0; loop_cnt < len; loop_cnt++)
	{
		Boot_UART_Tx_Byte( data[loop_cnt] );
	}
}


void Boot_UART_Init(uint32_t baud_rate)
{
	Boot_UART_GPIO_Init();
	Boot_UART_Reg_Init(baud_rate);
}


void Boot_UART_DeInit(void)
{
	Boot_UART_Reg_DeInit();
	Boot_UART_GPIO_DeInit();
}




