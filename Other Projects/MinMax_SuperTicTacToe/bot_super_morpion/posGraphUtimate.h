#ifndef GRAPHU_H
#define GRAPHU_H

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"

/*******Structures*********/

typedef struct{
    posGraph morpion[10]; // Le super-morpion est en fait un jeu de 10 morpions (le dernier morpion[9] est le grand morpion)
    char joueur;
}posGraphUltimate;

/******Prototypages******/

int CoupOpti(posGraph position, int parentID, int moveID, int isJoueurAuTrait, int coupopti[],int evaluation[],int ponderation);
posGraphUltimate fenToPosGraphUltimate(char* fen);

#endif