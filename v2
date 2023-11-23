#include "head.h"

void parcoursBFS(Graphe* graphe, int start, int* niveaux) {
    int queue[graphe->ordre];
    int head = 0, tail = 0;

    // Ajouter le sommet de départ à la queue
    queue[tail++] = start;
    niveaux[start] = 0;  // Le niveau du sommet de départ est 0

    while (head != tail) {
        int current = queue[head++];  // Défiler le sommet actuel

        // Pour chaque voisin du sommet actuel
        for (pArc arc = graphe->pSommet[current]->arc; arc != NULL; arc = arc->arc_suivant) {
            int voisin = arc->sommet;
            // Si le voisin n'a pas encore été visité
            if (niveaux[voisin] == -1) {
                niveaux[voisin] = niveaux[current] + 1;  // Attribuer le niveau suivant
                queue[tail++] = voisin;  // Enfiler le voisin
            }
        }
    }
}

void calculerNiveauxDePrecedence(Graphe* graphe, int* niveaux) {
    // Initialisation des niveaux à -1 (non visité)
    for (int i = 0; i < graphe->ordre; i++) {
        niveaux[i] = -1;
    }

    // Parcours en largeur à partir de chaque sommet
    for (int i = 0; i < graphe->ordre; i++) {
        if (niveaux[i] == -1) {  // Si le sommet n'a pas encore été visité
            parcoursBFS(graphe, i, niveaux);
        }
    }
}

station* CreerStations(int n, int ordre) {
    station* tete = NULL; // Pointeur vers la première station
    station* prev = NULL; // Pointeur vers la station précédente

    // Créez n stations
    for (int i = 0; i < n; i++) {
        station* nouvelle_station = (station*)malloc(sizeof(station));

        // Vérifiez si l'allocation de mémoire a réussi
        if (nouvelle_station == NULL) {
            // Gestion de l'erreur ici
            printf("Erreur d'allocation de mémoire pour la station %d\n", i);
            // Libérez la mémoire précédemment allouée
            while (tete != NULL) {
                station* temp = tete;
                tete = tete->next;
                free(temp->all_tache);
                free(temp->exclusion);
                free(temp);
            }
            return NULL; // Retournez NULL en cas d'erreur
        }

        // Allouez le tableau de nombres avec la taille de l'ordre du graphe
        nouvelle_station->all_tache = (int*)malloc(ordre * sizeof(int));

        // Allouez la matrice d'exclusion de taille ordre x ordre
        nouvelle_station->exclusion = (int*)malloc(ordre * sizeof(int));
        for (int j = 0; j < ordre; j++) {
            nouvelle_station->exclusion[j] = (int*)malloc(ordre * sizeof(int));
        }

        // Vérifiez si l'allocation de mémoire a réussi
        if (nouvelle_station->all_tache == NULL || nouvelle_station->exclusion == NULL) {
            // Gestion de l'erreur ici
            printf("Erreur d'allocation de mémoire pour la station %d\n", i);
            // Libérez la mémoire précédemment allouée
            while (tete != NULL) {
                station* temp = tete;
                tete = tete->next;
                free(temp->all_tache);
                free(temp->exclusion);
                free(temp);
            }
            return NULL; // Retournez NULL en cas d'erreur
        }

        // Initialisez le tableau de nombres à zéro (ou à la valeur par défaut)
        for (int j = 0; j < ordre; j++) {
            nouvelle_station->all_tache[j] = 0; // Initialisez à zéro, vous pouvez changer cela selon vos besoins
            for (int k = 0; k < ordre; k++) {
                nouvelle_station->exclusion[j][k] = 0; // Initialisez à zéro
            }
        }

        nouvelle_station->next = NULL; // Initialisez le pointeur suivant à NULL

        if (prev != NULL) {
            prev->next = nouvelle_station; // Si la station précédente existe, reliez-la à la nouvelle station
        }

        if (tete == NULL) {
            tete = nouvelle_station; // Si c'est la première station, définissez-la comme la tête de liste
        }

        prev = nouvelle_station; // Mettez à jour la station précédente
    }

    return tete; // Retournez la tête de la liste des stations
}


void AjouterTacheAStation(station* tete_station, int niveau_precedence, int tache) {
    // Parcourez la liste des stations jusqu'à atteindre la station correspondant au niveau de précédence
    station* station_courante = tete_station;
    int i = 0;
    while (i < niveau_precedence && station_courante != NULL) {
        station_courante = station_courante->next;
        i++;
    }

    // Vérifiez si la station a été trouvée
    if (station_courante != NULL) {
        // Ajoutez la tâche au tableau de la station
        station_courante->all_tache[tache] = 1; // Vous pouvez définir une valeur appropriée ici
    } else {
        // Gestion de l'erreur si la station n'a pas été trouvée (peut se produire si le niveau de précédence est incorrect)
        printf("Erreur : Station de niveau de précédence %d non trouvée.\n", niveau_precedence);
    }
}
