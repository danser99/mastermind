/**************************************************************
 *    mastermind.cpp
 *
 *    Test C++ du jeu de Mastermind electronique
 *
 **************************************************************/
 
//#include <LiquidCrystal.h>        // Pour affichage LCD
//#include <EEPROM.h>
/*#include "reglages.h"             // Reglages materiels
#include "io.h"                   // Controle des entrees/sorties
#include "jeu.h"                  // Controle du jeu
*/
#include <iostream>
#include <random>
using namespace std;

//----- REGLAGES
#define byte char
#define boolean bool
#include <string>
#define String string
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
#define NB_ESSAIS_MAX     99


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
#define SYMB_COEUR       "$"
#define SYMB_CARRE       "@"



// ----- IO.h

class LiquidCrystal{};        // Objet de la lib LiquidCrystal.h


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
extern byte CAR_CARRE_TABLE[8];
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

// Etat de la partie
enum EtatPartie { NOUVELLE, ENCOURS, FIN_PERDU, FIN_GAGNE };

// L'objet pour affichage LCD
//#ifdef LCD_8BIT_MODE
/*    LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_ENBL,
                      PIN_LCD_DATA0, PIN_LCD_DATA1,
                      PIN_LCD_DATA2, PIN_LCD_DATA3,
                      PIN_LCD_DATA4, PIN_LCD_DATA5,
                      PIN_LCD_DATA6, PIN_LCD_DATA7);
#else
    LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_ENBL,
                      PIN_LCD_DATA4, PIN_LCD_DATA5,
                      PIN_LCD_DATA6, PIN_LCD_DATA7);
#endif

*/



// ----- JEU.h

class Essai
{
    public:
        Essai(unsigned int difficulte, unsigned int noEssai);
        ~Essai();
        
        // Pour obtenir les elements
        unsigned int obtenirNbVert() const;
        unsigned int obtenirNbRouge() const;
        unsigned int obtenirRendu() const;
        
        // Pour controler
        boolean ajouterSymbole(Symbole symbole);
        void effacerSequence();
        void comparerSequence(const Symbole *seq);
        
        // Pour afficher
        void afficher(LiquidCrystal *lcd, boolean bas = false) const;

    private:  
        Symbole seq_[DIFFICULTE_MAX]; // La sequence du joueur
        unsigned int rendu_;      // La position du symbole a etre
				  // entre pour l'essai

        unsigned int nbVert_;     // Le nombre de bonne reponses
				  // pour la sequence
        unsigned int nbRouge_;    // Le nombre de reponses mal placees
				  // pour la sequence

        unsigned int difficulte_; // La difficulte de la partie
        unsigned int noEssai_;    // Le numero de l'essai courant
};


// Fonctions d'enregistrement et de lecture de meilleur score
// (utilise la EEPROM)
void enregistrerScore(unsigned int difficulte, unsigned int score);
unsigned int lireScore(unsigned int difficulte);

// Fonction de nouvelle partie
unsigned int choisirDifficulte(LiquidCrystal *lcd);

// Fonction de generation de sequence aleatoire
Symbole* genererSequence(unsigned int difficulte);

//Fonction pour obtenir une string a partir d'une sequence
String obtenirStr(const Symbole seq[], unsigned int longueur);


// Initialisations
void setup()
{
    // Initialisation des ports d'entree et de sortie
//    initialiserIO();

    // Initialisation des caracteres speciaux pour LCD
 //   lcd.createChar(CAR_HAUT, CAR_HAUT_TABLE);
   // lcd.createChar(CAR_BAS, CAR_BAS_TABLE);
    //lcd.createChar(CAR_CARRE, CAR_CARRE_TABLE);
    //lcd.createChar(CAR_COEUR, CAR_COEUR_TABLE);

    // Initialisation LCD
 //   lcd.begin(LCD_NB_COLONNES, LCD_NB_LIGNES);

    // Affichage ecran accueil
  //  afficherLcd(&lcd, "Mastermind", CENTRE);
   // delay(1500);        // Attente de 1 seconde
	//cout << "MASTERMIND\n";
    // Initialisation de la sequence aleatoire avec du bruit
    //randomSeed(analogRead(PIN_ANALOG_VIDE));

    // Nouvelle partie
   // etatPartie = NOUVELLE;
}



// Boucle principale
int main()
{

	
// Variables utilisees
LiquidCrystal lcd;
EtatPartie etatPartie;        // L'etat de la partie
unsigned int difficulte;      // La difficulte du jeu
unsigned int meilleurScore;   // Le meilleur score pour la difficulte
Essai *essais[NB_ESSAIS_MAX]; // Le tableau des essais
Symbole seqATrouver[DIFFICULTE_MAX] = {RIEN}; // La sequence de symboles a trouver
Symbole entree;               // Le bouton appuye
bool essaiEntre;              // Si l'essai en cours a ete entre
unsigned int noEssai;         // Le numero de l'essai en cours
unsigned int essaiAffiche;    // Le numero de l'essai affiche (pour navigation)

	//setup();
cout << "MASTERMIND\n";
    // Initialisation de la sequence aleatoire avec du bruit
    //randomSeed(analogRead(PIN_ANALOG_VIDE));

    // Nouvelle partie
    etatPartie = NOUVELLE;

while (1) {
    switch (etatPartie) {

        case NOUVELLE :
            // Demande du choix de difficulte
            difficulte = choisirDifficulte(&lcd);
            //difficulte = FACILE;
            // Lecture du meilleur score
            //meilleurScore = lireScore(difficulte);
            
            
            // Initialisation sequence aleatoire
            // Selon temps choix difficulte
            //randomSeed(millis());
            
            // Affichage meilleurScore
          /*  if (meilleurScore != 0) {
                
                String texte = " essai";
                if (meilleurScore > 1)
                    texte += String("s");
                    
                afficherLcd(&lcd, "Meilleure partie", GAUCHE,
                            String(meilleurScore) + texte, CENTRE);
                delay(1000);
            }
         */   
            // Generation de sequence aleatoire
            for (int i = 0; i < difficulte; i++)
                seqATrouver[i] = Symbole(rand()%PLUS+1);//X, PLUS + 1));
            
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
			int e;
            cin >> e;// = lireBoutons();
			entree = Symbole(e);
            // Verification attente de nouvel essai
            if (essaiEntre && entree != RIEN) {
                essaiEntre = false;
                // Le cas entree == HAUT doit etre execute normalement
                if (entree != HAUT) {
                    essaiAffiche = noEssai;
                    essais[noEssai]->afficher(&lcd);
                    break;
                }
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
                    if (essaiAffiche > 0)
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
                        essais[noEssai]->afficher(&lcd);
                    }
                    // Si l'essai est commence
                    // On efface l'essai en cours
                    else if (essaiAffiche == noEssai &&
			     essais[noEssai]->obtenirRendu()) {
                        essais[noEssai]->effacerSequence();
                    }
                    // Sinon, on demande pour une nouvelle partie
                    else {
                        //afficherLcd(&lcd, "Nouvelle partie?", GAUCHE,
                        //                  "X Non   O Oui",    CENTRE);
						cout << "nouvelle partie? (1) non, (2) oui\n";
                        entree = RIEN;
                        while (entree != X && entree != O && entree != CLR) {
							int e;
                           cin >> e;// = lireBoutons();
							entree=Symbole(e);
                        }

                        // Si choix de nouvelle partie
                        if (entree == O) {
                            etatPartie = NOUVELLE;
                            // Desallocation memoire dynamique
                            for (int i = 0; i <= noEssai; i++)
                                delete essais[i];
                        }
                        else {    // Sinon, retour a l'essai courant
                            essais[noEssai]->afficher(&lcd);
                        }
                    }
                    break;

                case RIEN :
                default : ; // Aucune action
            }
            break;


        case FIN_PERDU :
            // Attente d'interaction
            //if (lireBoutons() != RIEN) {
			cin >> e;
                // Affichage de la solution
                //String ligne2 = String("SOLUTION: ") +
                 //               obtenirStr(seqATrouver, difficulte);
                cout<< "Partie Terminee";//, CENTRE,
                         //  ligne2, GAUCHE, false, false);

                //delay(3000);    // Attente 3 sec
                
                // Desallocation memoire dynamique
                for (int i = 0; i < noEssai; i++)
                    delete essais[i];
                    
                etatPartie = NOUVELLE;    // Nouvelle partie
            //}
            break;


        //case FIN_GAGNE:
        default :
            String texte = " essais";
            //delay(100);        // Leger delai
            cout<<"FELICITATIONS!";//, CENTRE);
           // delay(1000);
            
            // Cas gagner en 1 essai
            if (noEssai == 1)
                texte = " essai";
            
           /* afficherLcd(&lcd, "Partie terminee", GAUCHE,
                        String("en ") + String(noEssai) + texte, GAUCHE);
                        
            if (noEssai < meilleurScore || meilleurScore == 0) {
                delay(1500);
                afficherLcd(&lcd, "Meilleure partie", GAUCHE,
                        String(noEssai) + texte, CENTRE);
                        
                // Enregistrement meilleurScore
                enregistrerScore(difficulte, noEssai);
            }

            delay(3000);
*/
            // Desallocation memoire dynamique
            for (int i = 0; i < noEssai; i++)
                delete essais[i];

            etatPartie = NOUVELLE;
    }
}
}



/**************************************************************
 *    jeu.cpp
 *
 *    Fichier d'implementation controlant le jeu de Mastermind
 *
 **************************************************************/

//#include <EEPROM.h>
//#include <LiquidCrystal.h>


/**************************************************************
 *    Essai::Essai
 *
 *    Constructeur par parametres de la classe Essai
 *    
 *    Arguments: difficulte : La difficulte de la partie
 *               noEssai    : Le numero de l'essai
 *    Retour: Aucun
 *
 **************************************************************/
Essai::Essai(unsigned int difficulte, unsigned int noEssai)
{
    // Initialisation de la sequence

    for (int i = 0; i < DIFFICULTE_MAX; i++)
        seq_[i] = RIEN;
    
    // Initialisation des attributs
    difficulte_ = difficulte;
    noEssai_ = noEssai;
    
    rendu_ = 0;
    nbVert_ = 0;
    nbRouge_ = 0;    
}



/**************************************************************
 *    Essai::~Essai
 *
 *    Destructeur de la classe Essai
 *    
 *    Arguments: Aucun
 *    Retour:    Aucun
 *
 **************************************************************/
Essai::~Essai()
{
}



/**************************************************************
 *    Essai::obtenirNbVert
 *
 *    Retourne la valeur de nbVert
 *    
 *    Arguments: Aucun
 *    Retour: nbVert
 *
 **************************************************************/
unsigned int Essai::obtenirNbVert() const
{
    return nbVert_;
}



/**************************************************************
 *    Essai::obtenirNbRouge
 *
 *    Retourne la valeur de nbRouge
 *    
 *    Arguments: Aucun
 *    Retour: nbRouge
 *
 **************************************************************/
unsigned int Essai::obtenirNbRouge() const
{
    return nbRouge_;
}



/**************************************************************
 *    Essai::obtenirRendu
 *
 *    Retourne l'endroit rendu dans la sequence
 *    (prochain element a etre devine)
 *    
 *    Arguments: Aucun
 *    Retour: rendu
 *
 **************************************************************/
unsigned int Essai::obtenirRendu() const
{
    return rendu_;
}



/**************************************************************
 *    Essai::ajouterSymbole
 *
 *    Ajoute un symbole a la sequence
 *    
 *    Arguments: symbole : le symbole a ajouter
 *    Retour:    true si la sequence est pleine/prete a etre evaluee
 *               false sinon
 *
 **************************************************************/
boolean Essai::ajouterSymbole(Symbole symbole)
{
    // Verification que la sequence n'est pas deja pleine
    if (rendu_ >= difficulte_)
        return true;
    
    // Ajout du symbole
    seq_[rendu_] = symbole;
    rendu_++;
    
    // Verification si la sequence est pleine
    return (rendu_ >= difficulte_);
}



/**************************************************************
 *    Essai::effacerSequence
 *
 *    Efface la sequence
 *    
 *    Arguments: Aucun
 *    Retour:    Aucun
 *
 **************************************************************/
void Essai::effacerSequence()
{
    // Reinitialisation des attributs
    for (int i = 0; i < difficulte_; i++)
        seq_[i] = RIEN;
    
    rendu_ = 0;
    nbVert_ = 0;
    nbRouge_ = 0;
}



/**************************************************************
 *    Essai::comparerSequence
 *
 *    Compare l'essai avec la sequence a trouver
 *    Pose nbVert au nombre de bonnes reponses devinees
 *    Pose nbRouge au nombre de reponses mal placees
 *    
 *    Arguments: seq[] : La sequence a trouver
 *    Retour:    Aucun
 *
 **************************************************************/
void Essai::comparerSequence(const Symbole seq[])
{
    unsigned int nbRouge = 0;
    unsigned int nbVert = 0;
    int i, j;     // Pour iterations
    
    // Pour verifier si la position a ete deja trouvee (vert)
    boolean trouve[DIFFICULTE_MAX];
    // Pour verifier si l'essai a ete deja utilise (rouge)
    boolean utilise[DIFFICULTE_MAX];
    
    for (i = 0; i < difficulte_; i++) {
        if (seq_[i] == seq[i]) {
            nbVert++;
            trouve[i] = true;
	    utilise[i] = true;
        }
        else {
            trouve[i] = false;
	    utilise[i] = false;
	}
    }
    
    // Verification pour nbRouge
    // i itere sur seq (a trouver)
    // j itere sur seq_ (essai)
    for (i = 0; i < difficulte_; i++)
        if (!trouve[i]) {
            for (j = 0; j < difficulte_; j++)
                if (seq_[j] == seq[i] && !utilise[j]) {
                    nbRouge++;
                    trouve[i] = true;
		    utilise[j] = true;
                    
                    // Passage au prochain element de la seq. a trouver
                    break;    // Evite les repetitions
                }
        }

    nbRouge_ = nbRouge;
    nbVert_ = nbVert;
}



/**************************************************************
 *    Essai::afficher
 *
 *    Affiche la sequence sur ecran LCD
 *    et allume le nombre de DEL correspondant a nbRouge et nbVert
 *    
 *    Arguments: *lcd : Pointeur sur l'objet LiquidCrystal pour affichage
 *                bas : si la navigation vers le bas est possible
 *                      (affichage de la fleche bas)
 *    Retour:    Aucun
 *
 **************************************************************/
void Essai::afficher(LiquidCrystal *lcd, boolean bas) const
{
	//ostringstream conv;
	//conv << noEssai_ + 1;
    String ligne1 = String("Essai ");// + String(conv.str());
    String ligne2 = obtenirStr(seq_, difficulte_);
    cout << ligne1 << noEssai_+1 <<'\n' << ligne2 << '\n';
    //afficherLcd(lcd, ligne1, GAUCHE, ligne2, CENTRE,
     //           (noEssai_), // False si 1er essai
       //         bas);
    allumerDel(nbRouge_, nbVert_);
}



/**************************************************************
 *    enregistrerScore
 *
 *    Enregistre le score pour la difficulte specifiee
 *    Utilise la EEPROM
 *    
 *    Arguments: difficulte : difficulte de la partie
 *                            sert d'adresse pour EEPROM
 *               score      : le score a enregistrer
 *    Retour:    Aucun
 *
 **************************************************************/
void enregistrerScore(unsigned int difficulte, unsigned int score)
{/*
    int adr = 0;
    byte val = 0;
    int ref = 0x000000FF;
    
    switch (difficulte) {
        case NORMAL :
            adr =  NORMAL;
            break;
            
        case DIFFICILE :
            adr = NORMAL * 2;
            break;
            
        case FACILE :
        default :
            adr = 0;            
    }
    
    // EEPROM.write ecrit 1 byte a l'adresse donnee
    // unsigned int = 4 bytes
    // Ecriture de chaque byte separement
    // Adresse adr+0 : Byte moins significatif
    for (int i = 0; i < 4; i++) {
        val = (score & ref) >> (8 * i);
        ref = ref >> 8;
        EEPROM.write(adr + i, val);
    }    */
}



/**************************************************************
 *    lireScore
 *
 *    Lit le score enregistre pour la difficulte specifiee
 *    Utilise la EEPROM
 *    
 *    Arguments: difficulte : difficulte de la partie
 *                            sert d'adresse pour EEPROM
 *    Retour:    Le score enregistre
 *
 **************************************************************/
unsigned int lireScore(unsigned int difficulte)
{/*
    int adr = 0;
    unsigned int score = 0;
    
    switch (difficulte) {
        case NORMAL :
            adr =  NORMAL;
            break;
            
        case DIFFICILE :
            adr = NORMAL * 2;
            break;
            
        case FACILE :
        default :
            adr = 0;            
    }
    
    // EEPROM.read lit 1 byte a l'adresse donnee
    // unsigned int = 4 bytes
    // Lecture de chaque byte separement
    // Adresse adr+0 : Byte moins significatif
    for (int i = 0; i < 4; i++)
        score += ((unsigned int) EEPROM.read(adr + i) & 0x000000FF) << (8 * i);

    return score;*/
}



/**************************************************************
 *    choisirDifficulte
 *
 *    Affiche un choix de difficulte et obtient le choix du joueur
 *    
 *    Arguments: *lcd : Pointeur sur l'objet LiquidCrystal pour affichage
 *    Retour:    La difficulte choisie
 *
 **************************************************************/
unsigned int choisirDifficulte(LiquidCrystal *lcd)
{
    Symbole entree = RIEN;
    unsigned int dif;

    afficherLcd(lcd, String("X Facile"), GAUCHE,
                String("O Norm   ") + String(SYMB_CARRE) + String(" Diff"),
                GAUCHE);

    while ( entree != X && entree != O && entree != CARRE )
        entree = lireBoutons();
        
    if (entree == X)
        dif = FACILE;
    else if (entree == O)
        dif = NORMAL;
    else if (entree == CARRE)
        dif = DIFFICILE;
    else    // Erreur
        dif = FACILE;
    
    return dif;
}



/**************************************************************
 *    obtenirStr
 *
 *    Convertie un tableau de symboles en String
 *
 *    Arguments: seq : la sequence a convertir
 *               longueur : la longueur du tableau
 *    Retour:    La chaine de texte contenant la sequence
 *
 **************************************************************/
String obtenirStr(const Symbole seq[], unsigned int longueur)
{
    String s = "";

    for (int i = 0; i < longueur; i++) {
        switch (seq[i]) {
            case X :
                s += String("X ");
                break;
            case O :
                s += String("O ");
                break;
            case CARRE :
                s += (String(SYMB_CARRE) + String(" "));
                break;
            case COEUR :
                s += String(SYMB_COEUR + String(" "));
                break;
            case ETOILE :
                s += String("* ");
                break;
            case PLUS :
                s += String("+ ");
                break;
            case RIEN :
            default :
                s += String("_ ");
        }
    }

    //s.trim();    // Retire l'espace a la fin
    return s;
}


/**************************************************************
 *    io.cpp
 *
 *    Fichier d'implementation controlant les entrees et sorties
 *    pour le jeu de Mastermind
 *
 **************************************************************/


//#include <LiquidCrystal.h>


/*
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

// Carre
byte CAR_CARRE_TABLE[8] = {
        B00000,
        B11111,
        B10001,
        B10001,
        B10001,
        B10001,
        B11111,
        B00000
        };

// Coeur
byte CAR_COEUR_TABLE[8] = {
        B00000,
        B01010,
        B10101,
        B10001,
        B10001,
        B01010,
        B00100,
        B00000
        };
*/


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
{/*
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
    }
        
    // LCD Contrast
    pinMode(PIN_LCD_CONTRAST, OUTPUT);
    analogWrite(PIN_LCD_CONTRAST, LCD_CONTRAST_VAL);
    */
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
{/*
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
        digitalWrite(PIN_DEL_VERTE[i], HIGH);  */
	cout << "\n==> " << nbRouge << " rouge; " << nbVert << " vert;\n";
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
{/*
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
    
    return ret;*/
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
int verifierCar(string car)
{
    if (car == SYMB_COEUR)
        return CAR_COEUR;
    else if (car == SYMB_CARRE)
        return CAR_CARRE;
        
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
 {/*
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
*/
 }
