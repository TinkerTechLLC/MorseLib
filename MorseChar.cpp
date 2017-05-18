#include "MorseChar.h"

// ********************************************
// Constructors
// ********************************************
MorseChar::MorseChar(char character, uint8_t code, uint8_t code_length)
{
    this->null_object = false;
    this->character = character;
    this->code = code;
    this->code_length = code_length;
}

MorseChar::MorseChar(void)
{
    this->null_object = true;
}

// ********************************************
// Public Member Functions
// ********************************************
char MorseChar::getChar(void)
{
    return character;
}

uint8_t MorseChar::getCode(void)
{
    return code;
}

uint8_t MorseChar::getCodeLength(void)
{
    return code_length;
}

bool MorseChar::isNull(void)
{
    return null_object;
}
