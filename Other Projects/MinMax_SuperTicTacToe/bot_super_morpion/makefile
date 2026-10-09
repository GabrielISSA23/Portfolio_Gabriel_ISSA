all : supermorpion.exe

supermorpion.exe : main.c morpion_to_S.c gestionGraphUltimate.c posGraphUltimate.c ../fonctions_aux/gestionGraph.c ../fonctions_aux/posGraph.c ../fonctions_aux/minimax.c ../fonctions_aux/morpion.c
	gcc main.c morpion_to_S.c posGraphUltimate.c gestionGraphUltimate.c ../fonctions_aux/gestionGraph.c ../fonctions_aux/morpion.c ../fonctions_aux/minimax.c ../fonctions_aux/posGraph.c -o sm-bot.exe