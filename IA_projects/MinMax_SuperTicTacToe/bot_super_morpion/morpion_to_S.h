#ifndef SMORPION_H
#define SMORPION_H

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"

#include "posGraphUtimate.h"

/********Prototypages*********/

void LaunchBot(posGraphUltimate position, int coup_precedent, int coup_a_jouer[]);
int evaluation(posGraphUltimate position,int coup_precedent, int horizon,int iterator, int coup_a_jouer[], int Joueurautrait);

#endif