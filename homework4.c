#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool success = false;

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        19,
        8,
        0X55,
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
        EUSCI_A_UART_8_BIT_LEN
    };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    setPeripherals();

    // TODO: Initialize EUSCI_A0
    initUART(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    enableUART(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        rChar = UARTgetChar(EUSCI_A0_BASE);

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if (rChar != 0xFF)
        {
            if (UARTcanSend(EUSCI_A0_BASE))
            {
                UARTputChar(EUSCI_A0_BASE, rChar);
            }

            success = charFSM(rChar);
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if (success)
        {
            int i = 0;
            while(response[i])
            {
                if (UARTcanSend(EUSCI_A0_BASE))
                {
                    UARTputChar(EUSCI_A0_BASE, response[i]);
                    i++;
                }
            }
        }

        success = false;
    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;
    static phraseState_t currPhraseState = empty;
    // if the char is 2 then go into the first letter state
    // while in first letter state, if next char is 5 go to second letter state
    switch(currPhraseState)
    {
    case empty:
        if (rChar == '2')
            currPhraseState = firstLetter;
        else
            currPhraseState = empty;
        break;
    case firstLetter:
        if (rChar == '5')
            currPhraseState = secondLetter;
        else
            currPhraseState = empty;
        break;
    case secondLetter:
        if (rChar == '3')
            currPhraseState = thirdLetter;
        else
            currPhraseState = empty;
        break;
    case thirdLetter:
        if (rChar == '4')
            finished = true;
        currPhraseState = empty;
        break;
    }

    return finished;
}

// Initializes UART module
void initUART(uint32_t moduleInstance, const eUSCI_UART_ConfigV1 *config)
{
    UART_initModule(moduleInstance, config);
}

// enables the UART module
void enableUART(uint32_t moduleInstance)
{
    UART_enableModule(moduleInstance);
}

// sets Rx and Tx to use their primary functions
void setPeripherals()
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
}

// returns true if there is data to receive and false if not
bool UARThasChar(uint32_t moduleInstance)
{
    return (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
}

// returns the char received if there was one and returns 0xFF if no char was received
char UARTgetChar(uint32_t moduleInstance)
{
    char charReceived = 0xFF;
    if (UARThasChar(moduleInstance))
    {
        charReceived = UART_receiveData(moduleInstance);
    }
    return charReceived;
}

// returns true if data can be transmitted using UART
bool UARTcanSend(uint32_t moduleInstance)
{
    return (UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
}

// transmits data using UART
void UARTputChar(uint32_t moduleInstance, char data)
{
    UART_transmitData(moduleInstance, data);
}
