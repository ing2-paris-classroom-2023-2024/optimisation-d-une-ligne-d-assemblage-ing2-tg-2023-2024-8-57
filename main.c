#include "head.h"

int main()
{
    station* liste_station;
    Graphe * g;
    int temp_cycle_max =0;

    int max_lvl =0;
    ///////graphe////////
    g = lire_graphe("graphe.txt");
    int niveaux[g->ordre];


    /////////////precedence +temps cycle////////////
    FILE* f = fopen("cycle.txt", "r");
    fscanf(f,"%d",&temp_cycle_max);


    int** precedenc_mat = malloc(g->ordre * sizeof(int*));
    for (int i = 0; i < g->ordre; ++i) {
        precedenc_mat[i] = calloc(g->ordre, sizeof (int));
    }

    lirePrecedence("graphe.txt",precedenc_mat,g->ordre);
    for (int i = 0; i < g->ordre; ++i) {
        for (int j = 0; j < g->ordre; ++j) {
            printf("%d ",precedenc_mat[i][j]);
        }
        printf("\n");
    }

    calculerNiveauxDePrecedence(g, niveaux);

    for (int i = 0; i < g->ordre; ++i) {
        if(niveaux[i]>max_lvl){
            max_lvl = niveaux[i];
        }
    }

    Tache* taches = lire_taches("temps.txt", g->ordre+1);

    liste_station = CreerStations(max_lvl+1,g->ordre+1);

    for (int i = 0; i <g->ordre ; ++i) {
        AjouterTacheAStation(liste_station,niveaux[i],i);
    }

    station* current = liste_station;

    while (current != NULL){
        for (int i = 0; i < g->ordre+1; ++i) {
            current->temps_tache[i] = taches->temp;
        }
        current = current->next;
    }

    sommeTempsTaches(&liste_station,g->ordre,temp_cycle_max,g);

    ////////exclusion//////////
    current = liste_station;
    while (current != NULL) {
        charger_exclusions(current,g->ordre,"exclu.txt");
        current = current->next;
    }

    resoudre_conflits(liste_station,g->ordre);

    ////////check condi////////

    while(resoudre_conflits(liste_station,g->ordre)||sommeTempsTaches(&liste_station,g->ordre,temp_cycle_max,g)||verifierPrecedence(&liste_station,precedenc_mat,g->ordre)){}

    ///////print station///////

    current = liste_station;
    int k =0;
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

    while(liste_station!=NULL){
        free(liste_station);
        liste_station =liste_station->next;
    }

    free(liste_station);
    return 0;
}