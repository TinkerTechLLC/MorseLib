
#include "MorseChar.h"
#include "MorseHandler.h"
#include <stddef.h>
#include <ctype.h>
#include <Arduino.h>
#include <TimerOne.h>

#define MICROS_PER_SEC 1e6
#define MILLIS_PER_SEC 1e3

// ********************************************
// Static Variable Initialization
// ********************************************
uint32_t MorseHandler::signal_period  = MICROS_PER_SEC / 10; // 4Hz frequency for debugging
bool     MorseHandler::is_initialized = false;
bool     MorseHandler::new_message    = false;
uint8_t  MorseHandler::handler_count  = 0;

// Create the data for the static array here...
MorseChar alphabet_data[] =
{
    MorseChar('a', 0b01,   2),
    MorseChar('b', 0b1000, 4),
    MorseChar('c', 0b1010, 4),
    MorseChar('d', 0b100,  3),
    MorseChar('e', 0b0,    1),
    MorseChar('f', 0b0010, 4),
    MorseChar('g', 0b110,  3),
    MorseChar('h', 0b0000, 4),
    MorseChar('i', 0b00,   2),
    MorseChar('j', 0b0111, 4),
    MorseChar('k', 0b101,  3),
    MorseChar('l', 0b0100, 4),
    MorseChar('m', 0b11,   2),
    MorseChar('n', 0b10,   2),
    MorseChar('o', 0b111,  3),
    MorseChar('p', 0b0110, 4),
    MorseChar('q', 0b1101, 4),
    MorseChar('r', 0b010,  3),
    MorseChar('s', 0b000,  3),
    MorseChar('t', 0b1,    1),
    MorseChar('u', 0b001,  3),
    MorseChar('v', 0b0001, 4),
    MorseChar('w', 0b011,  3),
    MorseChar('x', 0b1001, 4),
    MorseChar('y', 0b1011, 4),
    MorseChar('z', 0b1100, 4),

    MorseChar('0', 0b11111, 5),
    MorseChar('1', 0b01111, 5),
    MorseChar('2', 0b00111, 5),
    MorseChar('3', 0b00011, 5),
    MorseChar('4', 0b00001, 5),
    MorseChar('5', 0b00000, 5),
    MorseChar('6', 0b10000, 5),
    MorseChar('7', 0b11000, 5),
    MorseChar('8', 0b11100, 5),
    MorseChar('9', 0b11110, 5),

    MorseChar('.',  0b010101, 6),
    MorseChar(',',  0b110011, 6),
    MorseChar(':',  0b111000, 6),
    MorseChar('?',  0b001100, 6),
    MorseChar('\'', 0b011110, 6),
    MorseChar('-',  0b100001, 6),
    MorseChar('/',  0b10010,  5),
    MorseChar('@',  0b011010, 6),
    MorseChar('=',  0b10001,  5),
    MorseChar('(',  0b10110,  5),
    MorseChar(')',  0b101101, 6),
    MorseChar('+',  0b01010,  5),
    MorseChar('!',  0b101011, 6),
    // Put a null object here so we can find the end of the array
    MorseChar()
};

// ... and then assign it here
MorseChar* MorseHandler::alphabet = alphabet_data;


// ********************************************
// Constructor
// ********************************************
MorseHandler::MorseHandler(void)
{
    this->null_object       = true;
    this->toggle_pin        = 0;
    this->callbackFunction  = NULL;
    this->callback_set      = false;
}

MorseHandler::MorseHandler(uint8_t toggle_pin)
{
    MorseHandler::init();
    this->null_object       = false;
    this->toggle_pin        = toggle_pin;
    this->callbackFunction  = NULL;
    this->callback_set      = false;
    pinMode(toggle_pin, OUTPUT);
    digitalWrite(toggle_pin, LOW);
}

MorseHandler::MorseHandler(void (*callback)(MorseChar))
{
    MorseHandler::init();
    this->null_object       = false;
    this->toggle_pin        = 0;
    this->callbackFunction  = callback;
    this->callback_set      = true;
}

// ********************************************
// Public Member Functions
// ********************************************


// ********************************************
// Public Static Functions
// ********************************************
MorseHandler handler_init[MorseHandler::MAX_HANDLERS] = {MorseHandler()};
MorseHandler* MorseHandler::handler = handler_init;

char message_init[MorseHandler::MESSAGE_BUFF_SIZE] = {'\0'};
char* MorseHandler::message = message_init;

void MorseHandler::init(void)
{
    // Only run once
    if(!is_initialized)
    {
        Timer1.initialize(signal_period);
        Timer1.attachInterrupt(MorseHandler::handleOutgoingMessage);

        // Initialize all the handlers as null objects
        for(int i = 0; i < MAX_HANDLERS; i++)
        {
            handler[i] = MorseHandler();
        }
        handler_count = 0;

        // Clear the message buffer
        for(int i = 0; i < MESSAGE_BUFF_SIZE; i++)
        {
            message[i] = '\0';
        }
        new_message = false;

        is_initialized = true;
    }
}

void MorseHandler::setBaudRate(uint32_t rate)
{
    signal_period = MICROS_PER_SEC / rate;
}

uint32_t MorseHandler::getBaudRate(void)
{
    return (MICROS_PER_SEC / signal_period);
}

MorseChar MorseHandler::charToMorse(char character)
{
    MorseChar ret_char;
    int i = 0;
    while(1)
    {
        if(alphabet[i].isNull() || alphabet[i].getChar() == tolower(character))
        {
            ret_char = alphabet[i];
            break;
        }
        i++;
    }
    return ret_char;
}

void MorseHandler::setMessage(char* msg)
{
    int i = 0;
    while(i < MESSAGE_BUFF_SIZE)
    {
        message[i] = msg[i];
        if(message[i] == '\0')
        {
            break;
        }
        i++;
    }
    MorseHandler::printMessage();
    new_message = true;
}

void MorseHandler::printMessage(void)
{
    int i = 0;
    while(1)
    {
        if(message[i] == '\0')
        {
            Serial.println("");
            return;
        }
        else
        {
            Serial.print(message[i]);
        }
        i++;
    }
}

void MorseHandler::handleOutgoingMessage(void)
{
    static uint8_t   char_count;
    static MorseChar current_char;
    static uint8_t   current_bit;
    static bool      first_message_received = false;
    static bool      new_bit                = true;
    static uint8_t   on_cycles              = 0;
    static uint8_t   off_cycles             = 0;

    if(new_message)
    {
        current_char = MorseHandler::charToMorse(message[0]);
        first_message_received = true;
        new_message = false;
        char_count = 0;
        current_bit = 0;
        new_bit = true;
        off_cycles = NEW_MESSAGE_DELAY;
    }

    if(!first_message_received)
    {
        return;
    }

    if(new_bit)
    {
        if(current_bit == current_char.code_length)
        {
            char_count++;
            if(message[char_count] == '\0')
            {
                new_message = true;
                return;
            }
            off_cycles = NEW_CHAR_DELAY; // Stay off for a while to indicate a new character
            current_char = MorseHandler::charToMorse(message[char_count]);
            current_bit = 0;
        }

        if(current_char.code >> (current_char.code_length - current_bit - 1) & 0b1)
        {
            on_cycles = 2;  // dash
        }
        else
        {
            on_cycles = 1;  // dot
        }
        current_bit++;
        new_bit = false;
    }

    if(off_cycles > 0)
    {
        for(int i = 0; i < handler_count; i++)
        {
            digitalWrite(handler[i].toggle_pin, LOW);
        }
        off_cycles--;
    }
    else{
        if(on_cycles > 0)
        {
            for(int i = 0; i < handler_count; i++)
            {
                digitalWrite(handler[i].toggle_pin, HIGH);
            }
            on_cycles--;
        }
        else    // Add a pause (off cycle) between dots and dashes
        {
            for(int i = 0; i < handler_count; i++)
            {
                digitalWrite(handler[i].toggle_pin, LOW);
            }
            new_bit = true;
        }
    }
}

// ********************************************
// Private Member Functions
// ********************************************

// ********************************************
// Private Static Functions
// ********************************************
void MorseHandler::addHandler(MorseHandler new_handler)
{
    if(handler_count < MAX_HANDLERS)
    {
        handler[handler_count] = new_handler;
        handler_count++;
    }
}
