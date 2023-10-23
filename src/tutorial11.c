#include <stdio.h>
#include "qutyio.h"

/** Tutorial 11

INTRODUCTION:

In this week's tutorial you will implement a serial command parser
which accepts input from a user via the UART interface and responds
to a number of commands.

Implementing a serial interface is a core requirement for Assessment 2,
and use of a state machine is the preferred way to achieve the required
behavior. Refer to the Assessment 2 functional specifications.
*/

typedef enum
{
    START,
    ESCAPE,
    CMD,
    CMD_ON,
    CMD_OFF,
    CMD_SET,
    VAL,
    CHECK,
    CHECK_PASS,
    ACK,
    NACK
} SP_STATE;

int main(void)
{
    display_init();
    display_hex(0);
    display_off();

    serial_init();

    SP_STATE state = START;

    /** EX: 11.0

    DESCRIPTION:

    The state machine shown in "state_machine_tut11.png" implements a serial
    command parser with the following functionality:

        Start character:  'a'
        Escape character: 'b'
        Commands:
        - 'c' turn on display
        - 'd' turn off display
        - 'e' display payload byte as hex

    Command 'e' is followed by a payload byte of one character and a checksum
    byte which should be equal to the payload byte plus one.

    On receipt of a valid command the command parser responds with "ACK\n".

    On receipt of an invalid command (including a failed checksum) the
    command parser responds with "NACK\n".

    TASK: Implement this serial command parser using a state machine.

    We have already declared an enum with all of the required states
    and performed all initialisation above.

    You can use the following stdio functions to access the serial interface:
    - getchar()
    - printf()

    The following functions provided by "qutyio.h" can be used to control the
    display:
    - display_on()
    - display_off()
    - display_hex()

    EXAMPLES:

    INPUT: abc
    RESPONSE: ACK\n
    RESULT: Display turns on

    INPUT: abb
    RESPONSE: NACK\n
    RESULT: No action taken

    INPUT: aaabd
    RESPONSE: ACK\n
    RESULT: Display turns off

    INPUT: abeAB
    RESPONSE: ACK\n
    RESULT: Display shows "41" (if on)abc

    INPUT: abeAA
    RESPONSE: NACK\n
    RESULT: No action taken
    */

    /** CODE: Write your code for Ex 11.0 below this line. */


    uint8_t x = 0; // for storing the received character in VAL state
    uint8_t y = 0; // for storing the received character in CHECK state

     uint8_t inputChar;

while (1)
{
    switch (state)
    {
        case START:
            inputChar = getchar();  // Read a character only in this state
            if (inputChar == 'a')
                state = ESCAPE;
            break;

        case ESCAPE:
            inputChar = getchar();  // Read a character only in this state
            if (inputChar == 'b')
                state = CMD;
            else
                state = START;
            break;

        case CMD:
            inputChar = getchar();  // Read a character only in this state
            if (inputChar == 'c')
                state = CMD_ON;
            else if (inputChar == 'd')
                state = CMD_OFF;
            else if (inputChar == 'e')
                state = CMD_SET;
            else
            {
                printf("NACK\n");
                state = NACK;
            }
            break;

        case CMD_ON:
            display_on();
            printf("ACK\n");
            state = ACK;
            break;

        case CMD_OFF:
            display_off();
            printf("ACK\n");
            state = ACK;
            break;

        case CMD_SET:
            x = getchar(); // Read the next character only in this state
            state = VAL;
            break;

        case VAL:
            y = getchar(); // Read the next character only in this state
            state = CHECK;
            break;

        case CHECK:
            if (y == (x + 1))
                state = CHECK_PASS;
            else 
            {
                printf("NACK\n");
                state = NACK;
            }
            break;

        case CHECK_PASS:
            display_hex(x); // display x as hex
            printf("ACK\n");
            state = ACK;
            break;

        case NACK:
            state = START;
            break;

        case ACK:
            state = START;
            break;

        default:
            state = START;
            display_hex(0);
            display_off();
    }
}

}