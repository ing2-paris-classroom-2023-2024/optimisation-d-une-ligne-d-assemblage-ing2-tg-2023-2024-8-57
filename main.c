#include "head.h"

int main()
{
    station* liste_station;
    Graphe * g;
    int temp_cycle_max =0;

    int k=0;
    ///////graphe////////
    g = lire_graphe("graphe.txt");


    /////////////precedence +temps cycle////////////
    FILE* f = fopen("cycle.txt", "r");
    fscanf(f,"%d",&temp_cycle_max);

    int** exclusion = malloc((g->ordre+1) * sizeof(int*));
    for (int i = 0; i < (g->ordre+1); ++i) {
        exclusion[i] = calloc((g->ordre+1), sizeof (int));
    }

    int* temp = malloc((g->ordre+1) * sizeof(int));

    int** precedenc_mat = malloc(g->ordre * sizeof(int*));
    for (int i = 0; i < g->ordre; ++i) {
        precedenc_mat[i] = calloc(g->ordre, sizeof (int));
    }

    lirePrecedence("graphe.txt",precedenc_mat,g->ordre);


    Tache* taches = lire_taches("temps.txt", g->ordre+1);

    liste_station = init_station();
    station* current = liste_station;

    while (current != NULL){
        for (int i = 0; i < g->ordre+1; ++i) {
            current->temps_tache[i] = taches->temp;
        }
        current = current->next;
    }

    sommeTempsTaches(&liste_station,g->ordre,temp_cycle_max,g);


    ////////exclusion//////////
    charger_exclusions(exclusion,"exclu.txt");
    //resoudre_conflits(liste_station,g->ordre);

    ////////check condi////////

    while(sommeTempsTaches(&liste_station,g->ordre,temp_cycle_max,g)||verifierPrecedence(&liste_station,precedenc_mat,g->ordre)){}

    ///////print station///////

    current = liste_station;
    k =0;
    while (current != NULL) {
        printf("station :%d \n",k);
        for (int i = 0; i < g->ordre; ++i) {
            if(current->all_tache[i] == 1)
                printf("%d ",i);
        }
        printf("\n");
        k++;
        current = current->next;
    }



    //////////free///////////
    free(taches);
    free(precedenc_mat);

    free(liste_station);
    return 0;
}
