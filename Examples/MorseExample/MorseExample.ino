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
            Serial.println("outer adsf buff");

            printMessage();
            MorseHandler::setMessage(message_buff);
            Serial.println("message set");
        }
        else
        {
            message_buff[current_message_byte] = this_char;
            Serial.println("getting char");
        }
    }
}

void printMessage()
{
    Serial.println("outer message buff");
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
