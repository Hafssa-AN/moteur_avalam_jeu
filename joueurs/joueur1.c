/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"



// Prototypes des fonctions pour les vérifications stratégiques
int creeTourCinq(T_Position positionActuelle, T_Coup coup ,int trait);
int domineAdversaire(T_Position positionActuelle, T_Coup coup,int trait);
int combineDeuxTours(T_Position positionActuelle, T_Coup coup,int trait);
int bloqueAdversaire(T_Position positionActuelle, T_Coup coup,int trait);

// La fonction principale pour choisir le meilleur coup
void choisirCoup(T_Position positionActuelle, T_ListeCoups listeCoups) {
    int meilleurScore = -1000;  // Initialiser avec un score très bas
    int indiceMeilleurCoup = -1;
    int scoreCoup;
	int trait=positionActuelle.trait;
	
    // Itérer sur tous les coups possibles et les évaluer
    for (int i = 0; i < listeCoups.nb; i++) {
        T_Coup coupActuel = listeCoups.coups[i];
		int couleurSommet = positionActuelle.cols[coupActuel.destination].couleur;

        scoreCoup = 0; // Réinitialiser le score pour chaque coup


        // Appliquer les stratégies
        if (creeTourCinq(positionActuelle, coupActuel,trait)) {
            scoreCoup += 10; // Priorité élevée pour créer une tour de cinq avec notre couleur en haut
        }
        if (domineAdversaire(positionActuelle, coupActuel,trait)) {
            scoreCoup += 8; // Prioriser les coups qui dominent un bonus adverse
        }
        if (combineDeuxTours(positionActuelle, coupActuel,trait)) {
            scoreCoup += 5; // Combinaison de tours pour atteindre cinq avec notre couleur en haut
        }
        if (bloqueAdversaire(positionActuelle, coupActuel,trait)) {
            scoreCoup += 7; // Bloquer les coups adverses lorsque c'est stratégiquement bénéfique
        }

        // Vérifier si le coup actuel a un score supérieur au meilleur actuel
        if (scoreCoup > meilleurScore) {
            meilleurScore = scoreCoup;
            indiceMeilleurCoup = i;
        }
    }

    // Après avoir évalué tous les coups, exécuter le meilleur trouvé
    if (indiceMeilleurCoup != -1) {
        ecrireIndexCoup(indiceMeilleurCoup);
    } else {
		//Choisit un coup au hasard
		ecrireIndexCoup(rand()%listeCoups.nb); 
    }
}


// Vérifie si un coup crée une tour de cinq avec notre couleur au sommet
int creeTourCinq(T_Position positionActuelle, T_Coup coup, int trait) {
    T_Position nouvellePosition = jouerCoup(positionActuelle, coup.origine, coup.destination);
	
    int nouvelleHauteur = nouvellePosition.cols[coup.destination].nb;
    int couleurSommet = nouvellePosition.cols[coup.destination].couleur;

    if (nouvelleHauteur == 5 && couleurSommet == trait) {
        return 1;
    }
    return 0;
}

int domineAdversaire(T_Position positionActuelle, T_Coup coup ,int trait) {
    T_Position nouvellePosition = jouerCoup(positionActuelle, coup.origine, coup.destination);
    T_Evolution evol = nouvellePosition.evolution;

    // Vérifie si on déplace un bonus sur un bonus adverse
    if (positionActuelle.cols[coup.origine].couleur == trait && 
        positionActuelle.cols[coup.destination].couleur != trait &&
        evol.bonusJ > 0 && evol.bonusR > 0) {
        return 1;
    }
    return 0;
}

int combineDeuxTours(T_Position positionActuelle, T_Coup coup ,int trait) {
    T_Position nouvellePosition = jouerCoup(positionActuelle, coup.origine, coup.destination);
    int nouvelleHauteur = nouvellePosition.cols[coup.destination].nb;
    int couleurSommet = nouvellePosition.cols[coup.destination].couleur;

    if (nouvelleHauteur <= 5 && couleurSommet == trait) {
        return 1;
    }
    return 0;
}

int bloqueAdversaire(T_Position positionActuelle, T_Coup coup ,int trait) {
    // Cette version simple suppose que nous bloquons en plaçant notre pièce au-dessus
    T_Position nouvellePosition = jouerCoup(positionActuelle, coup.origine, coup.destination);

    int couleurSommet = nouvellePosition.cols[coup.destination].couleur;

    if (couleurSommet == trait) {
        return 1;
    }
    return 0;
}
