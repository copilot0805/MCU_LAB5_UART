/*
 * fsm.c
 *
 *  Created on: Nov 25, 2025
 *      Author: DELL
 */

#include "fsm.h"
#include <stdio.h>
#include <string.h>
#include "software_timer.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

uint8_t command_flag = 0;
enum CommandType command_data = NONE;

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t str[30];

uint32_t ADC_value = 0;

enum UartCommunicationState FSM_UART_STATE = INIT;
enum CommandParserState FSM_CMD_PARSER_STATE = WAITING;


void get_uart_message(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		buffer[index_buffer++] = temp;
		if(index_buffer == 30) index_buffer = 0;
		buffer_flag = 1;
		HAL_UART_Transmit(&huart2, &temp, 1, HAL_MAX_DELAY);

		HAL_UART_Receive_IT(&huart2, &temp, 1);
	}
}


void fsm_command_parser()
{
	switch (FSM_CMD_PARSER_STATE){
	case WAITING:

		if(temp == '!'){
			index_buffer = 0;
			FSM_CMD_PARSER_STATE = READING;
		}
		break;
	case READING:
		if(temp == '!'){
			index_buffer = 0;
			FSM_CMD_PARSER_STATE = READING;
		}
		if(temp == '#'){
			if(buffer[0] == 'R' && buffer[1] == 'S' && buffer[2] == 'T' && buffer[3] == '#'){
				command_flag = 1;
				command_data = RST;
			}
			if(buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '#'){
				command_flag = 1;
				command_data = OK;
			}
			FSM_CMD_PARSER_STATE = WAITING;
		}
		break;
	default:
		break;
	}
}

void fsm_uart_communication()
{
	switch(FSM_UART_STATE){
	case INIT:
		if(1){
			FSM_UART_STATE = WAIT_RST;
		}
		break;
	case WAIT_RST:
		if(command_data == RST && command_flag == 1){
			ADC_value = HAL_ADC_GetValue(&hadc1);
			//sprintf(str, "\r\n!ADC=%ld#\r\n", ADC_value);
			sprintf((char *)str, "\r\n!ADC=%ld#\r\n", ADC_value);
			command_flag = 0;
			FSM_UART_STATE = SEND_ADC;
		}
		break;
	case SEND_ADC:
		//HAL_UART_Transmit(&huart2, str, sizeof(str), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen((char*)str), HAL_MAX_DELAY);
		if(1){
			setTimer(0,3000);
			FSM_UART_STATE = WAIT_ACK;
		}
		break;
	case WAIT_ACK:
		if(command_data == OK && command_flag == 1){
			FSM_UART_STATE = WAIT_RST;
			command_flag = 0;
		}
		if(isFlag(0)){
			FSM_UART_STATE = SEND_ADC;
		}
		break;
	default:
		break;
	}
}




