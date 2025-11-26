/*
 * fsm.h
 *
 *  Created on: Nov 25, 2025
 *      Author: DELL
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"

#define MAX_BUFFER_SIZE 30

enum CommandType
{
	NONE,
	RST,
	OK
};

enum UartCommunicationState
{
	INIT,
	WAIT_RST,
	SEND_ADC,
	WAIT_ACK
};

enum CommandParserState
{
	WAITING,
	READING
};

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;
extern uint8_t str[30];

//extern uint8_t temp;
//extern uint8_t buffer_flag;

extern uint32_t ADC_value;

extern enum CommandType command_data;
extern uint8_t command_flag;

void get_uart_message(UART_HandleTypeDef *huart);
void fsm_command_parser();
void fsm_uart_communication();

#endif /* INC_FSM_H_ */
