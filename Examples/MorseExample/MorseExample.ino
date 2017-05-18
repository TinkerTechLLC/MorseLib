#include <TimerOne.h>
#include <Morse.h>

MorseChar output;
char message_buff[255]      = {'\0'};
int  current_message_byte   = 0;

void setup()
{
    Serial.begin(9600);
    MorseHandler mh = MorseHandler(13);
    MorseHandler::addHandler(mh);
    MorseHandler::setMessage("michael");
}

void loop()
{
    if(Serial.available())
    {
        char this_char = Serial.read();
        if(this_char == '\r')
        {
            message_buff[current_message_byte] = '\0';
            Serial.print("Message set: ");
            printMessage();
            MorseHandler::setMessage(message_buff);
            current_message_byte = 0;
        }
        else
        {
            message_buff[current_message_byte] = this_char;
            Serial.print("Getting character #");
            Serial.println(current_message_byte);
            current_message_byte++;
        }
    }
}

void printMessage()
{
    int i = 0;
    while(1)
    {
        if(message_buff[i] == '\0')
        {
            Serial.println("");
            return;
        }
        else
        {
            Serial.print(message_buff[i]);
        }
        i++;
    }
}
