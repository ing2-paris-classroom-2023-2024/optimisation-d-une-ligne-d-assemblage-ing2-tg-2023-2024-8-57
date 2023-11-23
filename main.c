#include "head.h"

int taille_nombre(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        return -1;
    }

    int lines = 0;
    char ch;
    while(!feof(file)) {
        ch = fgetc(file);
        if(ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}


int sommet_nombre(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        return -1;
    }

    int num, max = -1;

    while(fscanf(file, "%d", &num) != EOF) {
        if (num > max) {
            max = num;
        }
    }

    fclose(file);
    return max;
}



/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet, int s1, int s2) {
    pArc Newarc = (pArc)malloc(sizeof(struct Arc));
    Newarc->sommet = s2;
    Newarc->arc_suivant = NULL;

    // Recherche de la position d'insertion
    if (sommet[s1]->arc == NULL) {
        sommet[s1]->arc = Newarc;
    } else {
        pArc current = sommet[s1]->arc;
        pArc prev = NULL;

        // Parcours jusqu'à la fin de la liste ou jusqu'à trouver un arc plus grand
        while (current != NULL && current->sommet < s2) {
            prev = current;
            current = current->arc_suivant;
        }

        // Si l'arc existe déjà, libérer la mémoire et ne pas insérer
        if (current != NULL && current->sommet == s2) {
            free(Newarc);
            return sommet;
        }

        // Insertion de l'arc dans la liste
        if (prev == NULL) { // Insertion en tête
            Newarc->arc_suivant = sommet[s1]->arc;
            sommet[s1]->arc = Newarc;
        } else { // Insertion au milieu ou en fin
            Newarc->arc_suivant = prev->arc_suivant;
            prev->arc_suivant = Newarc;
        }
    }

    return sommet;
}


// créer le graphe
Graphe* CreerGraphe(int ordre)
{
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<ordre; i++)
    {
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->num_sommet=i;
        Newgraphe->pSommet[i]->arc=NULL;
    }
    return Newgraphe;
}


Graphe * lire_graphe(char * nomFichier)
{
    Graphe* graphe;

    FILE * ifs = fopen(nomFichier,"r");
    int taille,ordre, s1, s2;

    taille= taille_nombre(nomFichier);
    ordre = sommet_nombre(nomFichier);

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    graphe=CreerGraphe(ordre); // créer le graphe d'ordre sommets
    graphe->ordre =ordre;

    ifs = fopen(nomFichier,"r");

    for (int i=0; i<taille; ++i)
    {
        fscanf(ifs,"%d%d",&s1,&s2);
        graphe->pSommet=CreerArete(graphe->pSommet, s1, s2);
    }
    return graphe;
}

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe)
{
    printf("graphe\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }

}


int main()
{
    station* liste_station;
    Graphe * g;

    char nom_fichier[50];

    int max_lvl =0;
    int k =0;

   // printf("entrer le nom du fichier du labyrinthe:");
   // gets(nom_fichier);

    g = lire_graphe("graphe.txt");
    int niveaux[g->ordre];


    calculerNiveauxDePrecedence(g, niveaux);


    for (int i = 0; i < g->ordre; ++i) {
        if(niveaux[i]>max_lvl){
            max_lvl = niveaux[i];
        }
    }

    /*Tache* taches = lire_taches("temps.txt", g->ordre);
    for (int i = 0; i <g->ordre; ++i) {
        printf("%d: %f s",i,taches[i].temp);
    }*/

    liste_station = CreerStations(max_lvl+1,g->ordre+1);

    for (int i = 0; i <g->ordre ; ++i) {
        AjouterTacheAStation(liste_station,niveaux[i],i);
    }

    station* current = liste_station;

    while (current != NULL) {
        // Accédez au maillon actuel ici
        // Par exemple, current->all_tache vous donne l'ensemble des tâches de la station actuelle
        charger_exclusions(current,g->ordre,"exclu.txt");
        // Avancez au maillon suivant
        current = current->next;
    }

    current = liste_station;

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
    printf("\n");

    resoudre_conflits(liste_station,g->ordre);

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

    return 0;
}