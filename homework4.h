/*
 * homework4.h
 *
 *  Recreated on: 04/08/2020
 *      Author: Nick Hood
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);

// TODO: Define any constants that are local to homework.c using #define
#define PRESCALER 19
#define FIRST_MOD 8
#define SECOND_MOD 0x55

typedef enum {empty, firstLetter, secondLetter, thirdLetter} phraseState_t;

void initUART(uint32_t moduleInstance, const eUSCI_UART_ConfigV1 *config);
void enableUART(uint32_t moduleInstance);
void setPeripherals();
bool UARThasChar(uint32_t moduleInstance);
char UARTgetChar(uint32_t moduleInstance);
bool UARTcanSend(uint32_t moduleInstance);
void UARTputChar(uint32_t moduleInstance, char data);

#endif /* HOMEWORK4_H_ */
