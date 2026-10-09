#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"
#include <assert.h>

#include "posGraphUtimate.h"
#include "morpion_to_S.h"
#include "gestionGraphUltimate.h"

#define INFINI 1000

void LaunchBot(posGraphUltimate position, int coup_precedent, int coup_a_jouer[]) {
    int maximum = -INFINI;
    int grille = coup_precedent;
    if(isNodeTerminal(position.morpion[9]) != 0){
        printf("La partie est finie...");
    }
    if (isNodeTerminal(position.morpion[grille-1]) != 0) {
        int coupSave[9];
        int evaluationSave[9];
        CoupOpti(position.morpion[9], -1, 0, 0, coupSave, evaluationSave,0.9);
        for(int i = 0;i<9;i++){
            if(evaluationSave[i]>maximum){
                grille = coupSave[i]+1;
                maximum = evaluationSave[i];
            }
        }
    }
    maximum = -INFINI;
    char joueur_temp = position.joueur;
    evaluation(position, grille, 2, 1, coup_a_jouer,1);
    position.joueur = joueur_temp;
    coup_a_jouer[0] = grille;
    coup_a_jouer[1] = coup_a_jouer[1] + 1;
}

int evaluation(posGraphUltimate position, int coup_precedent, int horizon, int iterator, int coup_a_jouer[], int Joueurautrait) {
    int temp = -INFINI;
    int coupSave[9]={0};
    int evaluationSave[9]= {0};
    int maxi = -INFINI;
    if(iterator !=1){
        position.joueur = (position.joueur == 'x') ? 'o' : 'x';
    }
    
    if (iterator == horizon ) {
        CoupOpti(position.morpion[coup_precedent - 1], -1, 0, Joueurautrait, coupSave, evaluationSave,0.9);
        int aux = -INFINI;
        for (int k = 0; k < 9; k++) {
            if (evaluationSave[k] > aux && coupSave[k] != -100) { // == -100 signifie que le coup est deja pri
                if(iterator ==1) coup_a_jouer[iterator] = coupSave[k];
                aux = evaluationSave[k];
            }
        }
        return aux;
    }
    else {
        int m = -INFINI;
        int i_max = -INFINI;
        char grille_saved_temp[9];
        
        strcpy(grille_saved_temp, position.morpion[coup_precedent - 1].pos);
        CoupOpti(position.morpion[coup_precedent - 1], -1, 0, Joueurautrait, coupSave, evaluationSave,0.9);
        for (int j = 0; j < 9; j++) {
            strcpy(position.morpion[coup_precedent-1].pos, TourAuto(position.morpion[coup_precedent-1].pos, intToCoup(j + 1), position.joueur));
            m = evaluation(position, j + 1, horizon, iterator+1, coup_a_jouer,1-Joueurautrait);
            int old_maxi = maxi;
            maxi = max(maxi, m + evaluationSave[j]);
            if (maxi > old_maxi && coupSave[j] != -100) {
                i_max = j;
                old_maxi = maxi;
            }
            strcpy(position.morpion[coup_precedent - 1].pos, grille_saved_temp);
        }
        if (iterator == 1) coup_a_jouer[1] = i_max;            
        return maxi;
    }
}