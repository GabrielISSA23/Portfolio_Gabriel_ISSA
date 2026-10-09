#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"

#include "posGraphUtimate.h"

#define INF 1000

posGraphUltimate fenToPosGraphUltimate(char* fen){
    posGraphUltimate position;
    int decalage = 0;
    char* temp_BigMorpion = malloc(10*sizeof(char));
    strcpy(temp_BigMorpion, ".........");
    for(int i=0; i<9;i++){
        if(fen[i+decalage] == 'O'){         //On traite les cas où les morpions sont déjà gagnants
            strcpy(position.morpion[i].pos,"ooooooooo");
            temp_BigMorpion[i] = 'o';
        }
        else if(fen[i+decalage] == 'X'){
            strcpy(position.morpion[i].pos,"xxxxxxxxx");
            temp_BigMorpion[i] = 'x';
        }
        else{
            int somme_carac = 0;        //sinon, on stocke l'état
            int comptage = 0;
            
            for(int j = 0; j<9;j++){
                if(fen[j+decalage+i] == 'x' || fen[j+decalage+i] == 'o'){
                    somme_carac++;
                }
                else {
                    somme_carac += fen[j+decalage+i] -48;
                }
                position.morpion[i].pos[j] = fen[j+decalage+i];
                comptage = j;
                if(somme_carac == 9) break;
            }
            position.morpion[i].pos[comptage+1] = '\0';
            decalage += comptage;
        }
    }
    strcpy(position.morpion[9].pos, toPosGraph(temp_BigMorpion));
    free(temp_BigMorpion);
    return position;
}


int nombreDeNoeudUlt = -1;


int CoupOpti(posGraph position, int parentID, int moveID, int isJoueurAuTrait, int coupopti[], int evaluation[],int ponderation) {
    int val = -INF; // Par convention, cette valeur est -infini
    int min_eval = -INF;

    if (isNodeTerminal(position) != 0) {
        if (isNodeTerminal(position) == 2) {
            return 0;
        }
        return 100;
    } else {
        for (int i = 0; i < 9; i++) {
            char *positionConvert = toTab(position.pos);
            
            if (positionConvert[i] == '.') {
                // On copie la position actuelle dans un nouveau tableau
                char newPositionPos[9];
                strcpy(newPositionPos, positionConvert);

                // On effectue les modifications nécessaires
                newPositionPos[i] = position.joueur;

                // On créé une nouvelle structure avec le tableau modifié
                posGraph newPosition;
                strcpy(newPosition.pos, toPosGraph(newPositionPos));

                if (position.joueur == 'o') {
                    newPosition.joueur = 'x';
                } else {
                    newPosition.joueur = 'o';
                }

                if (parentID == -1) {
                    // On appelle récursivement la fonction avec la nouvelle position
                    val = max(val, -CoupOpti(newPosition, 0, nombreDeNoeudUlt, 1 - isJoueurAuTrait, coupopti, evaluation,ponderation));
                    coupopti[i] = i;
                    evaluation[i] = val;
                } else {
                    val = max(val, -CoupOpti(newPosition, moveID, nombreDeNoeudUlt, 1 - isJoueurAuTrait, coupopti, evaluation,ponderation));
                }
            }
            if (parentID == -1 && (positionConvert[i] == 'o' || positionConvert[i] == 'x')) {
                coupopti[i] = -100;
                evaluation[i] = -1000;
            }
            free(positionConvert);
        }
        return val;
    }
}
