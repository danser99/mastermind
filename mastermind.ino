/**************************************************************
 *    mastermind.ino
 *
 *    Jeu de Mastermind electronique
 *
 **************************************************************/
 
#include <LiquidCrystal.h>        // Pour affichage LCD
#include <EEPROM.h>
#include "reglages.h"             // Reglages materiels
#include "io.h"                   // Controle des entrees/sorties
#include "jeu.h"                  // Controle du jeu


// Etat de la partie
enum EtatPartie { NOUVELLE, ENCOURS, FIN_PERDU, FIN_GAGNE };

// Variables utilisees
EtatPartie etatPartie;        // L'etat de la partie
unsigned int difficulte;      // La difficulte du jeu
unsigned int meilleurScore;   // Le meilleur score pour la difficulte
Essai *essais[NB_ESSAIS_MAX]; // Le tableau des essais
Symbole *seqATrouver;         // La sequence de symboles a trouver
Symbole entree;               // Le bouton appuye
bool essaiEntre;              // Si l'essai en cours a ete entre
unsigned int noEssai;         // Le numero de l'essai en cours
unsigned int essaiAffiche;    // Le numero de l'essai affiche (pour navigation)

// L'objet pour affichage LCD
#ifdef LCD_8BIT_MODE
    LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_ENBL,
                      PIN_LCD_DATA0, PIN_LCD_DATA1,
                      PIN_LCD_DATA2, PIN_LCD_DATA3,
                      PIN_LCD_DATA4, PIN_LCD_DATA5,
                      PIN_LCD_DATA6, PIN_LCD_DATA7);
#else
    LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_ENBL,
                      PIN_LCD_DATA4, PIN_LCD_DATA5,
                      PIN_LCD_DATA6, PIN_LCD_DATA7);
#endif



// Initialisations
void setup()
{
    // Initialisation des ports d'entree et de sortie
    initialiserIO();

    // Initialisation des caracteres speciaux pour LCD
    lcd.createChar(CAR_HAUT, CAR_HAUT_TABLE);
    lcd.createChar(CAR_BAS, CAR_BAS_TABLE);
    lcd.createChar(CAR_CARRE, CAR_CARRE_TABLE);
    lcd.createChar(CAR_COEUR, CAR_COEUR_TABLE);

    // Initialisation LCD
    lcd.begin(LCD_NB_COLONNES, LCD_NB_LIGNES);

    // Affichage ecran accueil
    afficherLcd(&lcd, "Mastermind", CENTRE);
    delay(1500);        // Attente de 1 seconde

    // Initialisation de la sequence aleatoire avec du bruit
    randomSeed(analogRead(PIN_ANALOG_VIDE));

    // Nouvelle partie
    etatPartie = NOUVELLE;
}



// Boucle principale
void loop()
{
    switch (etatPartie) {

        case NOUVELLE :
            // Demande du choix de difficulte
            difficulte = choisirDifficulte(&lcd);
            
            // Lecture du meilleur score
            meilleurScore = lireScore(difficulte);
            
            // Affichage meilleurScore
            if (meilleurScore != 0) {
                
                String texte = " essai";
                if (meilleurScore > 1)
                    texte += String("s");
                    
                afficherLcd(&lcd, "Meilleure partie", GAUCHE,
                            String(meilleurScore) + texte, CENTRE);
                delay(1000);
            }
            
            // Generation de sequence aleatoire
            seqATrouver = genererSequence(difficulte);
            
            // Initialisation des essais
            noEssai = 0;
            essaiAffiche = 0;
            essaiEntre = false;
            essais[noEssai] = new Essai(difficulte, noEssai);
            
            // Affichage de l'essai #1
            essais[noEssai]->afficher(&lcd);
            
            // Debut de partie
            etatPartie = ENCOURS;
            break;


        case ENCOURS :
            // Lecture des entrees
            entree = lireBoutons();

            // Verification attente de nouvel essai
            if (essaiEntre && entree != RIEN) {
                essaiEntre = false;
                essais[noEssai]->afficher(&lcd);
                break;
            }

            switch (entree) {

                case X :
                case O :
                case CARRE :
                case COEUR :
                case ETOILE :
                case PLUS :
                    // Ajout du symbole
                    essaiEntre = essais[noEssai]-> ajouterSymbole(entree);

                    // Essai qui sera affiche
                    essaiAffiche = noEssai;

                    // Si l'essai est entre en entier
                    if (essaiEntre) {
                        essais[noEssai]->comparerSequence(seqATrouver);
                        essais[noEssai++]->afficher(&lcd, true);

                        // Gestion partie terminee (trop d'essais)
                        if (noEssai > NB_ESSAIS_MAX) {
                            etatPartie = FIN_PERDU;
                            break;
                        }
                        // Gestion partie terminee (trouve)
                        if (essais[noEssai-1]->obtenirNbVert() >= difficulte) {
                            etatPartie = FIN_GAGNE;
                            break;
                        }

                        // Nouvel essai
                        essais[noEssai] = new Essai (difficulte, noEssai);
                    }
                    else
                        essais[noEssai]->afficher(&lcd, false);

                    break;

                case HAUT :
                    // Si ce n'est pas le permier essai,
                    // affichage de l'essai precedent
                    if (essaiAffiche > 1)
                        essais[--essaiAffiche]->afficher(&lcd, true);
                    break;

                case BAS :
                    // Si ce n'est pas l'essai courant,
                    // affichage de l'essai suivant
                    if (essaiAffiche < noEssai)
                        essais[++essaiAffiche]->afficher(&lcd,
						(essaiAffiche < noEssai));
                    break;

                case CLR :
                    // Si l'essai affiche n'est pas l'essai courant
                    // on retourne a l'affichage de l'essai courant
                    if (essaiAffiche != noEssai) {
                        essaiAffiche = noEssai;
                        essais[noEssai]->afficher(&lcd, false);
                    }
                    // Si l'essai est commence
                    // On efface l'essai en cours
                    else if (essaiAffiche == noEssai &&
			     essais[noEssai]->obtenirRendu()) {
                        essais[noEssai]->effacerSequence();
                    }
                    // Sinon, on demande pour une nouvelle partie
                    else {
                        afficherLcd(&lcd, "Nouvelle partie?", GAUCHE,
                                    "X Non   O Oui", CENTRE);
                        entree = RIEN;
                        while (entree != X || entree != O || entree != CLR) {
                            entree = lireBoutons();
                        }
                        
                        // Si choix de nouvelle partie
                        if (entree == O) {
                            etatPartie = NOUVELLE;
                            // Desallocation memoire dynamique
                            for (int i = 0; i <= noEssai; i++)
                                delete essais[i];
                        }
                    }
                    break;

                case RIEN :
                default : ; // Aucune action
            }
            break;


        case FIN_PERDU :
            // Attente d'interaction
            if (lireBoutons() != RIEN) {
                // Affichage de la solution
                String ligne2 = String("SOLUTION: ") +
                                obtenirStr(seqATrouver, difficulte);
                afficherLcd(&lcd, "Partie Terminee", CENTRE,
                            ligne2, GAUCHE, false, false);

                delay(3000);    // Attente 3 sec
                
                // Desallocation memoire dynamique
                for (int i = 0; i < noEssai; i++)
                    delete essais[i];
                    
                etatPartie = NOUVELLE;    // Nouvelle partie
            }
            break;


        case FIN_GAGNE:
        default :
            String texte = " essais";
            delay(100);        // Leger delai
            afficherLcd(&lcd, "FELICITATIONS!", CENTRE);
            delay(1000);
            
            // Cas gagner en 1 essai
            if (noEssai == 1)
                texte = " essai";
            
            afficherLcd(&lcd, "Partie terminee", GAUCHE,
                        String("en ") + String(noEssai) + texte, GAUCHE);
                        
            if (noEssai < meilleurScore) {
                delay(1500);
                afficherLcd(&lcd, "Meilleure partie", GAUCHE,
                        String(noEssai) + texte, CENTRE);
                        
                // Enregistrement meilleurScore
                enregistrerScore(difficulte, noEssai);
            }

            delay(3000);

            // Desallocation memoire dynamique
            for (int i = 0; i < noEssai; i++)
                delete essais[i];

            etatPartie = NOUVELLE;
    }
}
