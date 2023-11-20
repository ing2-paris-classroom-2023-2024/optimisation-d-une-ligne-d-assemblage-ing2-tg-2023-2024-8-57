
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

    int num;
    int count = 0;
    int numbers[1000] = {}; // Adjust size as necessary

    while(fscanf(file, "%d", &num) != EOF) {
        int found = 0;
        for (int i = 0; i < count; ++i) {
            if (numbers[i] == num) {
                found = 1;
                break;
            }
        }

        if (!found) {
            numbers[count++] = num;
        }
    }

    fclose(file);
    return count;
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
pSommet* CreerArete(pSommet* sommet,int s1,int s2)
{
    if(sommet[s1]->arc==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc=Newarc;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1]->arc;
        while( !(temp->arc_suivant==NULL))
        {
            temp=temp->arc_suivant;
        }
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2)
        {
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
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

    ifs = fopen(nomFichier,"r");

    // créer les arêtes du graphe
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
    Graphe * g;

    char nom_fichier[50];

    printf("entrer le nom du fichier du labyrinthe:");
    gets(nom_fichier);

    g = lire_graphe(nom_fichier);


    /// afficher le graphe
    graphe_afficher(g);

    return 0;
}

