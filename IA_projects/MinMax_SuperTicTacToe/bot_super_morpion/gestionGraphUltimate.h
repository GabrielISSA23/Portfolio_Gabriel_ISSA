#ifndef GGRAPHU_H
#define GGRAPHU_H

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"

#include "posGraphUtimate.h"
#include "morpion_to_S.h"

void writeDotTable(posGraph position, char* filename);
void writeMorpionUltimate(posGraphUltimate positionU);

#endif