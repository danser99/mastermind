/* TEST ALLUMER DEL*/

#include <LiquidCrystal.h>
#include "reglages.h"             // Reglages materiels
#include "io.h"                   // Controle des entrees/sorties


void setup()
{
    initialiserIO();
    randomSeed(0);
}

void loop()
{
    for (int i = 0; i < 7; i++){
        allumerDel(i,i);
        delay(500);
    }
    allumerDel(0,0);
    delay(500);
    allumerDel(1,0);
    delay(500);
    allumerDel(0,2);
    delay(500);
}
