/**************************************************************
 *    io.h
 *
 *    Fichier d'en-tete controlant les entrees et sorties
 *    pour le jeu de Mastermind
 *
 **************************************************************/

#ifndef _io_h
#define _io_h


#include <Arduino.h>

class LiquidCrystal;        // Objet de la lib LiquidCrystal.h


// Entrees possibles
enum Symbole {  RIEN = 0,
                X,
                O,
                CARRE,
                COEUR,
                ETOILE,
                PLUS,
                HAUT,
                BAS,
                CLR,
                NB_SYMBOLES    // Dernier element
            };
                

// Position possibles pour affichage LCD
enum Position { GAUCHE, DROITE, CENTRE };


// Caracteres propres au jeu
// Espace de stockage
const int CAR_OK      = 0; // Si aucun remplacement necessaire
const int CAR_HAUT    = 1;
const int CAR_BAS     = 2;
const int CAR_COEUR   = 3;
const int CAR_CARRE   = 4;

// Caracteres
extern byte CAR_HAUT_TABLE[8];    // Fleche haut
extern byte CAR_BAS_TABLE[8];     // Fleche bas
extern byte CAR_CARRE_TABLE[8];   // Carre
extern byte CAR_COEUR_TABLE[8];   // Coeur


// Pour initialiser les entrees
void initialiserIO();

// Pour allumer le nombre de DEL desire
void allumerDel(unsigned int nbRouge, unsigned int nbVert);

// Pour determiner si un bouton est appuye
Symbole lireBoutons();

// Pour affichage LCD
// Pour le remplacement de caracteres
int verifierCar(char car);

// Pour affichage
void afficherLcd(LiquidCrystal *lcd,
	String ligne1, Position posLigne1 = GAUCHE,
        String ligne2 = "", Position posLigne2 = GAUCHE,
        boolean haut = false, boolean bas = false);

#endif
