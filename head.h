#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef struct Station{
    int* all_tache;
    double* temps_tache;
    int** exclusion;
    struct Station* next;
}station;

/* Structure d'un arc*/
struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
    struct Arc* arc_suivant;
};

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;

/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc;
    int num_sommet;
    char couleur;
    int temp;
};

/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;

/* Alias d'un Graphe */
typedef struct Graphe
{
    int taille;
    int orientation;
    int ordre;
    pSommet* pSommet;
} Graphe;

typedef struct tache1
{
    int num_tache;
    double temp;
}Tache;

// créer le graphe
Graphe* CreerGraphe(int ordre);

/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe * lire_graphe(char * nomFichier);

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2);

/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num);

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe);

void parcoursBFS(Graphe* graphe, int start, int* niveaux);

void calculerNiveauxDePrecedence(Graphe* graphe, int* niveaux);

int taille_nombre(char *filename);

int sommet_nombre(char *filename);

int nombreDescendants(Graphe* G, int tache);


/////Station/////////

station* CreerStations(int n, int ordre);

void AjouterTacheAStation(station* tete_station, int niveau_precedence, int tache);

////////precedence/////////

int verifierPrecedence(station** liste_stations, int** precedence, int ordre);

void lirePrecedence( char* nom_fichier, int** precedence, int ordre);

//////////exclusion////////

void charger_exclusions(station* stations, int ordre,char* nomFichier);

int resoudre_conflits(station* stations, int ordre);

int est_precedente(station* liste_station, int tache1, int tache2);

int est_exclue(station* liste_station, int tache1, int tache2);

//////temps tache/////
Tache* lire_taches(char* nom_fichier, int n);

int sommeTempsTaches(station** liste_station, int ordre, double k,Graphe* g);


#endif // GRAPHE_H_INCLUDED
