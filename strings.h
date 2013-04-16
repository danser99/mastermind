/**************************************************************
 *    strings.h
 *
 *    Contient les chaines de caracteres pour eviter
 *    la surcharge de la SRAM
 *
 **************************************************************/

#ifndef _mastermind_strings_h
#define _mastermind_strings_h

#include <Arduino.h>

//#ifdef LANGUE_FRANCAIS




/*
#define iSTR_ESSAI            7
#define iSTR_DIFF_FACILE      8
#define iSTR_DIFF_NORM        9
#define iSTR_DIFF_DIFF        10
#define iSTR_X                11
#define iSTR_O                12
#define iSTR_ETOILE           13
#define iSTR_PLUS             14
#define iSTR_RIEN             15
*/


// char[10]s jeu.cpp
const String STR_ESSAI = "Essai #";
const String STR_DIFF_FACILE = "X Facile";
const String STR_DIFF_NORM = "O Norm   ";
const String STR_DIFF_DIFF = " Diff";
const String STR_X = "X ";
const String STR_O = "O ";
const String STR_ETOILE = "* ";
const String STR_PLUS = "+ ";
const String STR_RIEN = "_ ";



// Strings mastermind.ino
/*
const char STR_TITRE[17] = "Mastermind";
const char STR_MEILLEURE_PARTIE[17] = "Meilleure partie";
const char STR_NP[17] = "Nouvelle partie?";
const char STR_NP_OPTIONS[16] = "X Non   O Oui";
const char STR_PARTIE_TERMINEE[16] = "Partie terminee";
const char STR_FELICITATIONS[16] = "FELICITATIONS!";
const char STR_MP[17] = "Meilleure partie";

// char[10]s jeu.cpp
const char STR_ESSAI[10] = "Essai ";
const char STR_DIFF_FACILE[10] = "X Facile";
const char STR_DIFF_NORM[10] = "O Norm   ";
const char STR_DIFF_DIFF[10] = " Diff";
const char STR_X[3] = "X ";
const char STR_O[3] = "O ";
const char STR_ETOILE[3] = "* ";
const char STR_PLUS[3] = "+ ";
const char STR_RIEN[3] = "_ ";
*/
//#endif // LANGUE_FRANCAIS

#endif // _mastermind_strings_h
