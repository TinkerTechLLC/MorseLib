#ifndef _MORSEHANDLER_h
#define _MORSEHANDLER_h

#include <stdint.h>
#include "MorseChar.h"

class MorseHandler
{

public:

    // Constructors
    MorseHandler(void);                         // Null object for placeholding
    MorseHandler(uint8_t pin);                  // For pin toggling
    MorseHandler(void (*callback)(MorseChar));  // For triggering callback

    // Public Variables
    static const uint8_t MAX_HANDLERS = 5;
    static const uint8_t MESSAGE_BUFF_SIZE = 255;


    // Static Functions
    static void      init(void);
    static void      setBaudRate(uint32_t rate);
    static uint32_t  getBaudRate(void);
    static void      setTogglePin(int);
    static void      setMessage(char*);
    static void      handleOutgoingMessage(void);
    static MorseChar charToMorse(char character);
    static void      addHandler(MorseHandler new_handler);

private:

    // Member Variables
    bool    null_object;
    int8_t  toggle_pin;
    void    (*callbackFunction)(MorseChar);
    bool    callback_set;

    // Member Functions

    // Static Variables
    static const uint8_t NEW_MESSAGE_DELAY = 6;    // How long to wait when starting a new message in cycles
    static const uint8_t NEW_CHAR_DELAY    = 3;    // How long to wait when starting a new char in cycles
    static bool          is_initialized;
    static uint32_t      signal_period;
    static MorseChar*    alphabet;
    static MorseHandler* handler;
    static uint8_t       handler_count;
    static char*         message;
    static bool          new_message;

    // Static Functions
    static void timerTest(void);
    static void printMessage(void);
};

#endif
