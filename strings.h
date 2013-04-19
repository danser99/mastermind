/**************************************************************
 *    strings.h
 *
 *    Contient les chaines de caracteres
 * **************************************************************/

#ifndef _mastermind_strings_h
#define _mastermind_strings_h

#include <Arduino.h>

//#ifdef LANGUE_FRANCAIS

// Strings mastermind.ino
const String STR_TITRE = "Mastermind";
const String STR_MEILLEURE_PARTIE = "Meilleure partie";
const String STR_NP = "Nouvelle partie?";
const String STR_NP_OPTIONS = "X Non   O Oui";
const String STR_PARTIE_TERMINEE = "Partie terminee";
const String STR_FELICITATIONS = "FELICITATIONS!";
const String STR_MP = "Meilleure partie";
const String STR_SOL = "SOL: ";

// Strings jeu.cpp
const String STR_ESSAI = "Essai #";
const String STR_DIFF_FACILE = "X Facile";
const String STR_DIFF_NORM = "O Norm   ";
const String STR_DIFF_DIFF = " Diff";
const String STR_X = "X ";
const String STR_O = "O ";
const String STR_ETOILE = "* ";
const String STR_PLUS = "+ ";
const String STR_RIEN = "_ ";

//#endif // LANGUE_FRANCAIS

#endif // _mastermind_strings_h
