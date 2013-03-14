/********************************
 *
 * Programme Arduino pour vider
 * la EEPROM (remise a 0)
 *
 ********************************/
 
#include <EEPROM.h>

const int mem_size = 15;

void setup()
{
    for (int adr = 0; adr < mem_size; adr++)
        EEPROM.write(adr, 0);
}

void loop()
{
}
