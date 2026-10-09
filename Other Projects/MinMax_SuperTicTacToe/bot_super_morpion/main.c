#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../fonctions_aux/posGraph.h"
#include "../fonctions_aux/morpion.h"
#include "../fonctions_aux/gestionGraph.h"
#include "../fonctions_aux/minimax.h"

#include "posGraphUtimate.h"
#include "morpion_to_S.h"
#include "gestionGraphUltimate.h"

int main(int argc, char* argv[]){
    if(argc != 3){
        fprintf(stderr, "Format correct : ./sm-bot.exe \"[POSITION FEN]\" [TEMPS RESTANT]");
        exit(EXIT_FAILURE);
    } 
    
    posGraphUltimate position;
    char buffer[100];
    int count = 0;
    while(argv[1][count] != ' '){
        buffer[count] = argv[1][count];
        count++;
    }
    buffer[++count] = '\0';
    position = fenToPosGraphUltimate(buffer);         //Exemple de buffer : "6xoxOOOX2xo1ox1oXx2xo4oox4ox"
    
    char* last_play = malloc(5*sizeof(char));
    last_play[0] = argv[1][count++];
    last_play[1] = argv[1][count++];        //On stocke les deux derniers caractères du dernier coup
    last_play[2] = '\0';                     //Fin de chaîne
    position.joueur = argv[1][count+1];
    
    for(int i = 0; i<10;i++){
        position.morpion[i] = newPosition(position.morpion[i].pos,'o');
    }
    
    int coup_precedent = last_play[1]-48;
    int coup_a_jouer[3];
    LaunchBot(position,coup_precedent,coup_a_jouer);
    printf("%d\n", coup_a_jouer[0]*10 + coup_a_jouer[1]);
    return coup_a_jouer[0]*10 + coup_a_jouer[1];        //On renvoie le coup à jouer sous la forme <grille><case>
    
}