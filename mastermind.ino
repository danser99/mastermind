/**************************************************************
 *    mastermind.ino
 *
 *    Jeu de Mastermind electronique
 *
 **************************************************************/
 
#include <LiquidCrystal.h>        // Pour affichage LCD
#include "reglages.h"             // Reglages materiels
#include "io.h"                   // Controle des entrees/sorties
#include "jeu.h"                  // Controle du jeu


// Variables utilisees
unsigned int difficulte;      // La difficulte du jeu
Essai *essais[NB_ESSAIS_MAX]; // Le tableau des essais
Symbole *seqATrouver;         // La sequence de symboles a trouver
Symbole entree;               // Le bouton appuye
unsigned int noEssai;         // Le numero de l'essai en cours
unsigned int essaiAffiche;    // Le numero de l'essai affiche (pour navigation)

LiquidCrystal lcd(PIN_LCD_REG, PIN_LCD_ENBL, PIN_LCD_DATA0, PIN_LCD_DATA1,
        PIN_LCD_DATA2, PIN_LCD_DATA3);    // L'objet pour affichage LCD



// Initialisations
void setup()
{
    // Initialisation des ports d'entree et de sortie
    initialiserIO();
    
    // Initialisation des caracteres speciaux pour LCD
    lcd.createChar(CAR_HAUT, CAR_HAUT_TABLE);
    lcd.createChar(CAR_BAS, CAR_BAS_TABLE);
    lcd.createChar(CAR_COEUR, CAR_COEUR_TABLE);
    
    // Initialisation LCD
    lcd.begin(LCD_NB_COLONNES, LCD_NB_LIGNES);
    
    // Affichage ecran accueil
    afficherLcd(&lcd, "Mastermind", CENTRE);
    delay(1000);        // Attente de 1 seconde
    
    // Initialisation de la sequence aleatoire avec du bruit
    randomSeed(analogRead(PIN_ANALOG_VIDE));
}



// Boucle principale
void loop()
{
  
}
