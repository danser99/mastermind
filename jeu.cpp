/**************************************************************
 *    jeu.cpp
 *
 *    Fichier d'implementation controlant le jeu de Mastermind
 *
 **************************************************************/
 
#include "reglages.h"
#include "jeu.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>


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
    if (rendu_ >= difficulte_)
        return true;
    else
       return false;
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
    
    for (i = 0; i < difficulte_; i++) {
        if (seq_[i] == seq[i]) {
            nbVert++;
            trouve[i] = true;
        }
        else
            trouve[i] = false;
    }
    
    // Verification pour nbRouge
    // i itere sur seq
    // j itere sur seq_
    for (i = 0; i < difficulte_; i++)
        if (!trouve[i]) {
            for (j = 0; j < difficulte_; j++)
                if (seq_[j] == seq[i]) {
                    nbRouge++;
                    trouve[i] = true;
                    
                    // Passage au prochain element de la seq. a trouver
                    break;    // Evite les repetitions
                }
        }

    nbRouge_ = nbRouge;
    nbVert_ = nbVert;
    
    delete [] trouve;
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
    String ligne1 = String("Essai ") + String(noEssai_);
    String ligne2 = "";
    
    // Sequence pour ligne2
    for (int i = 0; i < difficulte_; i++) {
        switch (seq_[i]) {
            case X :
                ligne2 += String("X ");
                break;
            case O :
                ligne2 += String("O ");
                break;
            case CARRE :
                ligne2 += (String(char(254)) + String(" "));
                break;
            case COEUR :
                ligne2 += String(SYMB_COEUR + String(" "));
                break;
            case ETOILE :
                ligne2 += String("* ");
                break;
            case PLUS :
                ligne2 += String("+ ");
                break;
            case RIEN :
            default :
                ligne2 += String("_ ");            
        }
    }
        
    ligne2.trim();    // Retire l'espace a la fin
    
    afficherLcd(lcd, ligne1, GAUCHE, ligne2, CENTRE,
                (noEssai_ - 1), // False si noEssai = 1
                bas);
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
{
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
    }    
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
{
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

    return score;
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
                String("O Norm   ") + String(char(254)) + String(" Diff"),
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
 *    genererSequence
 *
 *    Genere une sequence aleatoire
 *    
 *    Arguments: difficulte : la difficulte de la sequence
 *    Retour:    La sequence aleatoire
 *
 **************************************************************/
Symbole* genererSequence(unsigned int difficulte)
{
    Symbole seq[DIFFICULTE_MAX] = {RIEN};

    for (int i = 0; i < difficulte; i++)
        seq[i] = Symbole(random(X, PLUS + 1));
        
    return seq;
}
