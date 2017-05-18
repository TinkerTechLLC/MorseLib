#ifndef _MORSECHAR_h
#define _MORSECHAR_h

#include <stdint.h>

class MorseChar
{
public:
    // Constructor
    MorseChar(void);
    MorseChar(char character, uint8_t code, uint8_t code_length);

    // Member Functions
    char    getChar(void);
    uint8_t getCode(void);
    uint8_t getCodeLength(void);
    bool    isNull(void);

    uint8_t code;
    uint8_t code_length;


private:
    // Member Variables
    char    character;
    bool    null_object;   // Just used for indicating the end of the code array
};

#endif
