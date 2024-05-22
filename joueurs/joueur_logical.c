/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


// Définir les constantes pour votre couleur et celle de l'adversaire
#define MA_COULEUR 1 // Ajustez cette valeur en fonction de votre configuration de jeu
#define COULEUR_ADVERSAIRE 2 // Ajustez également cette valeur

// Prototypes des fonctions pour les vérifications stratégiques
int creeTourCinq(T_Position positionActuelle, T_Coup coup);
int domineAdversaire(T_Position positionActuelle, T_Coup coup);
int combineDeuxTours(T_Position positionActuelle, T_Coup coup);
int bloqueAdversaire(T_Position positionActuelle, T_Coup coup);

// La fonction principale pour choisir le meilleur coup
void choisirCoup(T_Position positionActuelle, T_ListeCoups listeCoups) {
    int meilleurScore = -1000;  // Initialiser avec un score très bas
    int indiceMeilleurCoup = -1;
    int scoreCoup;

    // Itérer sur tous les coups possibles et les évaluer
    for (int i = 0; i < listeCoups.nb; i++) {
        T_Coup coupActuel = listeCoups.coups[i];
        scoreCoup = 0; // Réinitialiser le score pour chaque coup

        // Appliquer les stratégies
        if (creeTourCinq(positionActuelle, coupActuel)) {
            scoreCoup += 10; // Priorité élevée pour créer une tour de cinq avec notre couleur en haut
        }
        if (domineAdversaire(positionActuelle, coupActuel)) {
            scoreCoup += 8; // Prioriser les coups qui dominent un bonus adverse
        }
        if (combineDeuxTours(positionActuelle, coupActuel)) {
            scoreCoup += 5; // Combinaison de tours pour atteindre cinq avec notre couleur en haut
        }
        if (bloqueAdversaire(positionActuelle, coupActuel)) {
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
        printf("Aucun coup valide trouvé.\n");
    }
}




// Vérifie si un coup crée une tour de cinq avec notre couleur au sommet
int creeTourCinq(T_Position positionActuelle, T_Coup coup) {
    T_Position nouvellePosition = jouerCoup(positionActuelle, coup.origine, coup.destination);
    int nouvelleHauteur = nouvellePosition.cols[coup.destination].nb;
    int couleurSommet = nouvellePosition.cols[coup.destination].couleur;

    if (nouvelleHauteur == 5 && couleurSommet == MA_COULEUR) {
        return 1;
    }
    return 0;
}

