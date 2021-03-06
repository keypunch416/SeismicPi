/*
 * HAL_UART.cpp
 *
 *  Created on: 3 Aug 2015
 *      Author: Steve
 */

#include "HAL.h"
#include "UART.h"
#include "Console.h"
#include "ISR_UART.h"

void UART_Start() {
	UART_Init();
}

void UART_Print(char c) {
	while(UART_SendChar(c));
}

void UART_Print(const char str[]) {
	while (*str != '\0') {
		while(UART_SendChar(*str));
		*str++;
	}
}

void UART_Print(char str[]) {
	while (*str != '\0') {
		while(UART_SendChar(*str));
		*str++;
	}
}

void UART_Print(char str[], uint8 strlen) {
	uint8 i = 0;
	for (i = 0; i < strlen; i++) {
		while(UART_SendChar(str[i]));
	}
}

char UART_GetChar(){
	unsigned char c = '\0';
	UART_RecvChar(&c);
	return c;
}

void UART_Flush(){
	while(UART_GetCharsInTxBuf()>0);
}

void UART_ISR_CPP(){
	Console_Input_Char();
}

extern "C" void UART_ISR(){
	UART_ISR_CPP();
}
