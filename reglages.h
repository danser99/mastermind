/**************************************************************
 *    reglages.h
 *
 *    Permet de regler les parametres du jeu de Mastermind
 *    Definit les ports utilises
 *
 **************************************************************/

#ifndef _reglages_h
#define _reglages_h

// --------------------------------------
// Definition des reglages du jeu

// Nombre de symboles a deviner selon le niveau de difficulte
#define FACILE            3
#define NORMAL            4
#define DIFFICILE         6

// Difficulte maximale, egale au nombre de DEL disponibles pour chaque couleur
#define DIFFICULTE_MAX    DIFFICILE

// Nombre d'essais maximal alloue
// Un nombre trop eleve peut entrainer un probleme d'affichage et/ou de memoire
#define NB_ESSAIS_MAX     24


// --------------------------------------
// Definition des ports de sortie

// Affichage DEL
// DEL rouges (reponse mal placee)
const int PIN_DEL_ROUGE[DIFFICULTE_MAX] = {
    31, 33, 35, 37, 39, 41 };
    
// DEL vertes (bonne reponse)
const int PIN_DEL_VERTE[DIFFICULTE_MAX] = {
    43, 45, 47, 49, 51, 53 };


// Affichage LCD
#define LCD_8BIT_MODE           // Si mode 8bits

#define PIN_LCD_RS         12
#define PIN_LCD_RW         10   // Optionel
#define PIN_LCD_ENBL       11
#define PIN_LCD_CONTRAST   9    // Doit etre PWM, PIN V0
#define PIN_LCD_DATA4      5
#define PIN_LCD_DATA5      4
#define PIN_LCD_DATA6      3
#define PIN_LCD_DATA7      2

#define PIN_LCD_DATA0      8
#define PIN_LCD_DATA1      7
#define PIN_LCD_DATA2      6
#define PIN_LCD_DATA3      1


#define LCD_CONTRAST_VAL   50    // Valeur pour ajustement du contrast


// --------------------------------------
// Definition des ports d'entree

// Boutons d'entree

// Symboles
#define PIN_BTN_X          26
#define PIN_BTN_O          28
#define PIN_BTN_CARRE      30
#define PIN_BTN_COEUR      32
#define PIN_BTN_ETOILE     34
#define PIN_BTN_PLUS       36

// Navigation
#define PIN_BTN_HAUT       38
#define PIN_BTN_BAS        40

// Effacer/Nouvelle partie
#define PIN_BTN_CLR        42


// --------------------------------------------
// Definition d'un port analogique non utilise
// Sert a l'initialisation de la fonction random() via randomSeed()

#define PIN_ANALOG_VIDE    0


// --------------------------------------------
// Definitions materielles

// Nombre de colonnes/lignes de l'ecran LCD
#define LCD_NB_COLONNES      16   // Minimum : 16 colonnes
#define LCD_NB_LIGNES        2    // Minimum : 2 lignes


// --------------------------------------------
// Definitions logicielles

// Ces symboles seront subtitues durant l'affichage pour
// les symboles crees par le logiciel
//#define SYMB_HAUT        '%'    // Pas utilise
//#define SYMB_BAS         '&'    // Pas utilise
#define SYMB_COEUR       '$'
#define SYMB_CARRE       '@'


#endif
