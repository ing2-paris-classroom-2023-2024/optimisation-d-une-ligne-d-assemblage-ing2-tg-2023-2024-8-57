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

int nombreDescendants(Graphe* G, int tache) {
    // Vérification des limites
    if (tache < 0 || tache >= G->ordre) {
        return 0; // Numéro de tâche invalide
    }

    // Initialisation du compteur de descendants
    int descendants = 0;

    // Parcours de tous les sommets du graphe
    for (int i = 0; i < G->ordre; i++) {
        pSommet sommet = G->pSommet[i];

        // Parcours des arcs sortants du sommet
        pArc arcCourant = sommet->arc;
        while (arcCourant != NULL) {
            if (arcCourant->sommet == tache) {
                // Si l'arc sortant du sommet mène à la tâche spécifiée, elle est un descendant
                descendants++;
            }
            arcCourant = arcCourant->arc_suivant; // Passage à l'arc suivant
        }
    }

    return descendants;
}
