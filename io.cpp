/**************************************************************
 *    io.cpp
 *
 *    Fichier d'implementation controlant les entrees et sorties
 *    pour le jeu de Mastermind
 *
 **************************************************************/

#include "reglages.h"
#include "io.h"
#include <LiquidCrystal.h>



// Caracteres
// Fleche haut
byte CAR_HAUT_TABLE[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00100,
        B01110,
        B11111
        };
    
// Fleche bas
byte CAR_BAS_TABLE[8] = {
        B11111,
        B01110,
        B00100,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
        };
    
// Coeur
byte CAR_COEUR_TABLE[8] = {
        B00000,
        B01010,
        B11111,
        B11111,
        B11111,
        B01110,
        B00100,
        B00000
        };



/**************************************************************
 *    initialiserIO
 *
 *    Fonction qui intialise les ports des boutons en entree
 *    et des DEL et LCD en sortie
 *    
 *    Arguments: Aucun
 *    Retour: Aucun
 *
 **************************************************************/
void initialiserIO()
{
    // Boutons
    pinMode(PIN_BTN_X,         INPUT);
    pinMode(PIN_BTN_O,         INPUT);
    pinMode(PIN_BTN_CARRE,     INPUT);
    pinMode(PIN_BTN_COEUR,     INPUT);
    pinMode(PIN_BTN_ETOILE,    INPUT);
    pinMode(PIN_BTN_PLUS,      INPUT);
    pinMode(PIN_BTN_HAUT,      INPUT);
    pinMode(PIN_BTN_BAS,       INPUT);
    pinMode(PIN_BTN_CLR,       INPUT);
    
    // DEL
    for (int i = 0; i < DIFFICULTE_MAX; i++) {
        pinMode(PIN_DEL_ROUGE[i], OUTPUT);
        pinMode(PIN_DEL_VERTE[i], OUTPUT);
        
    // LCD Contrast
    pinMode(PIN_LCD_CONTRAST, OUTPUT);
    analogWrite(PIN_LCD_CONTRAST, LCD_CONTRAST_VAL);
    }
}



/**************************************************************
 *    allumerDel
 *
 *    Fonction allume le nombre de DEL rouges et vertes specifies
 *    
 *    Arguments: nbRouge : nombre de DEL rouges a allumer
 *               nbVert  : nombre de DEL vertes a allumer
 *    Retour: Aucun
 *
 **************************************************************/
void allumerDel(unsigned int nbRouge, unsigned int nbVert)
{
    int i;
    
    // Eteindre toutes les DEL
    for (i = 0; i < DIFFICULTE_MAX; i++) {
        digitalWrite(PIN_DEL_ROUGE[i], LOW);
        digitalWrite(PIN_DEL_VERTE[i], LOW);
    }
    
    // Allumer le nombre de DEL rouges specifie
    for (i = 0; i < nbRouge; i++)
        digitalWrite(PIN_DEL_ROUGE[i], HIGH);

    // Allumer le nombre de DEL vertes specifie
    for (i = 0; i < nbVert; i++)
        digitalWrite(PIN_DEL_VERTE[i], HIGH);  
}



/**************************************************************
 *    lireBoutons
 *
 *    Fonction qui lit l'etat des boutons d'entree
 *    
 *    Arguments: Aucun
 *    Retour:    Symbole du bouton appuye
 *               Retourne RIEN si la lecture n'est pas comprise
 *               (2 boutons appuyes simultanement)
 *
 **************************************************************/
Symbole lireBoutons()
{
    // Pour sauvegarde de l'etat precedent des boutons
    // L'index suit l'enum Symbole
    static int etatPrecedent[NB_SYMBOLES] = {LOW};
    
    int lecture;               // Lecture effectuee
    Symbole ret = RIEN;        // Valeur de retour
    boolean ok = true;         // Si la valeur a changee
    int nbAppuyes = 0;         // Le nombre de boutons appuyes
    
    // Lecture de l'etat du bouton X
    lecture = digitalRead(PIN_BTN_X);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = X;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[X] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[X] = lecture;     // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton O
    lecture = digitalRead(PIN_BTN_O);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = O;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[O] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[O] = lecture;     // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton CARRE
    lecture = digitalRead(PIN_BTN_CARRE);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = CARRE;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[CARRE] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[CARRE] = lecture; // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton COEUR
    lecture = digitalRead(PIN_BTN_COEUR);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = COEUR;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[COEUR] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[COEUR] = lecture; // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton ETOILE
    lecture = digitalRead(PIN_BTN_ETOILE);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = ETOILE;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[ETOILE] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[ETOILE] = lecture;     // Si l'etat a change,
					     // on enregistre le nouvel etat


    // Lecture de l'etat du bouton PLUS
    lecture = digitalRead(PIN_BTN_PLUS);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = PLUS;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[PLUS] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[PLUS] = lecture;  // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton HAUT
    lecture = digitalRead(PIN_BTN_HAUT);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = HAUT;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[HAUT] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[HAUT] = lecture;  // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton BAS
    lecture = digitalRead(PIN_BTN_BAS);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = BAS;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[BAS] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[BAS] = lecture;   // Si l'etat a change, on enregistre
					// le nouvel etat


    // Lecture de l'etat du bouton CLR
    lecture = digitalRead(PIN_BTN_CLR);
    if (lecture == HIGH) {    // Si le bouton est appuye
        nbAppuyes++;
        ret = CLR;
    }
    // Si l'etat n'a pas change et que le bouton est active
    if (lecture == etatPrecedent[CLR] && lecture == HIGH)
        ok = false;                     // On devalide l'entree
    else
        etatPrecedent[CLR] = lecture;   // Si l'etat a change, on enregistre
					// le nouvel etat


    delay(10);    // Anti-rebonds, attendre 10 ms
    
    if (!ok || nbAppuyes != 1)    // Si plus d'un bouton est appuye ou si le
        ret = RIEN;               // bouton est garde enfonce, on retourne RIEN
    
    return ret;
}



/**************************************************************
 *    verifierCar
 *
 *    Fonction qui verifie si un caractere doit etre remplace
 *    
 *    Arguments: car : Le caractere a comparer
 *    Retour:    CAR_OK (false) si le caractere ne doit pas etre remplace
 *               CAR_COEUR si le caractere doit etre remplace par un coeur
 *
 **************************************************************/
int verifierCar(char car)
{
    if (car == SYMB_COEUR)
        return CAR_COEUR;
        
    /*  Ces fonctions ne sont pas utilisees
    else if (car == SYMB_HAUT)
        return CAR_HAUT;
    else if (car == SYMB_BAS)
        return CAR_BAS;
    */
    
    else
        return CAR_OK;
}



/**************************************************************
 *    afficherLcd
 *
 *    Fonction qui affiche sur l'ecran LCD le texte desire
 *
 *    *Pour afficher 15-16 car, utiliser la position GAUCHE
 *
 *    
 *    Arguments: *lcd     : Pointeur sur l'objet LiquidCrystal pour affichage
 *                ligne1  : Chaine du haut a afficher (String)
 *                posLigne1 : Endroit ou le texte de la ligne1 sera place
 *                ligne2  : Ligne 2 a afficher
 *                posLigne2
 *                haut    : Si vrai, affiche une fleche indiquant la navigation
 *                              possible vers le haut (historique precedent)
 *                bas     : Si vrai, affiche une fleche indiquant la navigation
 *                              possible vers le bas (historique suivant)
 *    Retour:    Aucun
 *
 *    Remarque: Les symboles definis dans reglages.h (SYMB_COEUR) 
 *              seront remplaces par les symboles correspondants pour
 *              la ligne 2 (Aucun remplacement necessaire pour la ligne 1)
 *                
 *
 **************************************************************/
 void afficherLcd(LiquidCrystal *lcd,
                  String ligne1, Position posLigne1,
                  String ligne2, Position posLigne2,
                  boolean haut, boolean bas)
 {
     int carRemplace = 0;
     int i;           // Pour iterations
     String ligne;    // Ligne traitee

     lcd->clear();    // Efface l'ecran

     // Traitement de la ligne 1
     switch (posLigne1) {
         // Si le texte est a gauche
         case GAUCHE :
	     // Inscription au debut de la ligne
             lcd->setCursor(0,0);
             break;
             
         // Si le texte est a droite
         case DROITE :
             // Inscription a la fin de la ligne
             lcd->setCursor(LCD_NB_COLONNES - 2 - ligne1.length(), 0);
             break;
                     
         // Si le texte est au centre
         case CENTRE :
         {
             // Inscription au centre de la ligne
             int espacesVides = (LCD_NB_COLONNES - 1 - ligne1.length())/2;
             lcd->setCursor(espacesVides, 0);
	     break;
         }
         
         // Par defaut
         default :
             lcd->setCursor(0,0);
     }

     lcd->print(ligne1);	// Affichage de la ligne 1


     // Traitement de la ligne 2
     // Chaque caractere est traite individuellement
     switch (posLigne2) {
         // Si le texte est a gauche
         case GAUCHE :
             lcd->setCursor(0,1);          // Inscription au debut de la ligne
             break;

         // Si le texte est a droite
         case DROITE :
             // Inscription a la fin de la ligne
             lcd->setCursor(LCD_NB_COLONNES - 2 - ligne2.length(), 1);
             break;

         // Si le texte est au centre
         case CENTRE :
         {
             // Inscription au centre de la ligne
             int espacesVides = (LCD_NB_COLONNES - 1 - ligne2.length())/2;
             lcd->setCursor(espacesVides, 1);
	     break;
         }

         // Par defaut
         default :
             lcd->setCursor(0,1);
     }

     // Affichage de la ligne 2, 1 caractere a la fois
     for (i = 0; i < ligne2.length(); i++) {
         // Verification si le caractere doit etre remplace
         carRemplace = verifierCar(ligne2.charAt(i));
         if (carRemplace)        	    // Si oui, on affiche le
             lcd->write(carRemplace);	    // caractere de remplacement
         else                    	    // Sinon, on affiche le
             lcd->print(ligne2.charAt(i));  // caractere de la ligne
     }


     // Impression des caracteres haut/bas
     if (haut) {
         lcd->setCursor(LCD_NB_COLONNES - 1, 0);
         lcd->write(CAR_HAUT);
     }
     if (bas) {
         lcd->setCursor(LCD_NB_COLONNES - 1, 1);
         lcd->write(CAR_BAS);
     }

 }
