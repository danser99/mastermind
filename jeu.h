/**************************************************************
 *    jeu.h
 *
 *    Fichier d'en-tete controlant le jeu de Mastermind
 *
 **************************************************************/

#ifndef _jeu_h
#define _jeu_h


#include <Arduino.h>
#include "io.h"

// Classe Essai, controle les essais du joueur
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
        Symbole *seq_;            // La sequence du joueur
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


#endif
