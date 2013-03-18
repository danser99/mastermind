/*
	Programme de test pour la fonction
	comparerSequence
*/

#include <iostream>

#define DIFFICULTE_MAX    6
#define boolean bool

using namespace std;

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




void comparerSequence(const Symbole seq[],
		unsigned int &nbRouge_, unsigned int &nbVert_,
		unsigned int difficulte_,
		Symbole seq_[DIFFICULTE_MAX])
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
    // i itere sur seq
    // j itere sur seq_
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


int main()
{
	unsigned int diff = 3;

	Symbole seqATrouver[DIFFICULTE_MAX] = {
		ETOILE, PLUS, ETOILE, RIEN, RIEN, RIEN
		};

	Symbole essai[DIFFICULTE_MAX] = {
		PLUS, PLUS, PLUS, RIEN, RIEN, RIEN
		};

	unsigned int r=0;
	unsigned int v=0;

	comparerSequence(seqATrouver, r, v, diff, essai);

	cout << '\n' << "rouge: " << r;
	cout << '\n' << "vert: " << v;
	cout << '\n';
	

	return 0;
}
