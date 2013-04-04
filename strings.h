/**************************************************************
 *    strings.h
 *
 *    Contient les chaines de caracteres pour eviter
 *    la surcharge de la SRAM
 *
 **************************************************************/

#ifndef _mastermind_strings_h
#define _mastermind_strings_h

//#ifdef LANGUE_FRANCAIS

// Strings mastermind.ino
const char STR_TITRE[11] = "Mastermind";
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


//#endif // LANGUE_FRANCAIS

#endif // _mastermind_strings_h
