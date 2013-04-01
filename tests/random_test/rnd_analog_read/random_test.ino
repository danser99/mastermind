/*  Test sequence aleatoire.
    Avec randomSeed(entree analogue)
*/


// TEST ANALOG, :1ere val toujours 0
void setup()
{    
    Serial.begin(9600);
    /*Serial.println(String("["));
    // 1000 premieres valeurs
    for (int i = 0; i<1000; i++) {
        randomSeed(analogRead(0));
        Serial.println(String(random(1,7))
                    + String(","));
    }
    Serial.println(String("]"));
    */
    
    // 1000 groupes de 3 valeurs
    for (int i = 0; i<1000; i++) {
        randomSeed(analogRead(0));
        //randomSeed(2);
        Serial.println(
            String("(") +
            String(random(1,7)) +
            String(",") +
            String(random(1,7)) +
            String(",") +
            String(random(1,7)) +
            String("),")
            );
    }
    
    
}

void loop()
{
}
