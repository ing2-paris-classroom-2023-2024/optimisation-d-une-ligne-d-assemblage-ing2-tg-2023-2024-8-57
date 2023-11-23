#include "head.h"

Tache* lire_taches(char* nom_fichier, int n) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Crée un tableau de tâches de taille n
    Tache* taches = (Tache*)malloc(n * sizeof(Tache));
    if (!taches) {
        perror("Erreur d'allocation de mémoire pour taches");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    // Initialise toutes les tâches avec un temps
    for (int i = 0; i < n; i++) {
        taches[i].num_tache = i + 1;
        taches[i].temp = 0;
    }

    int num;
    double temp;

    while (fscanf(fichier, "%d %lf", &num, &temp) == 2) {
        if (num >= 1 && num <= n) {
            // Met à jour le temps de la tâche correspondante
            taches[num - 1].temp = temp;
        }
    }

    fclose(fichier);
    return taches;
}


int sommeTempsTaches(station** liste_station, int ordre, double k, Graphe* g) {
    station* current_station = *liste_station;
    station* previous_station = NULL;
    int modification_effectuee = 0; // Drapeau pour indiquer si une modification a été faite

    while (current_station != NULL) {
        double somme = 0.0;
        int taches_presentes = 0;

        for (int tache = 0; tache < ordre; tache++) {
            if (current_station->all_tache[tache] == 1) {
                somme += current_station->temps_tache[tache];
                taches_presentes++;
            }
        }

        if (somme > k) {
            int tache_a_deplacer = -1;
            int min_descendants = ordre + 1;

            for (int tache = 0; tache < ordre; tache++) {
                if (current_station->all_tache[tache] == 1) {
                    int descendants = nombreDescendants(g, tache);
                    if (descendants < min_descendants) {
                        tache_a_deplacer = tache;
                        min_descendants = descendants;
                    }
                }
            }

            if (tache_a_deplacer != -1) {
                // Logique pour déplacer la tâche
                if (current_station->next == NULL) {
                    station* nouvelle_station = malloc(sizeof(station));
                    nouvelle_station->all_tache = malloc(ordre * sizeof(int));
                    nouvelle_station->temps_tache = malloc(ordre * sizeof(double));
                    nouvelle_station->next = NULL;
                    if (previous_station != NULL) {
                        previous_station->next = nouvelle_station;
                    } else {
                        *liste_station = nouvelle_station;
                    }
                    current_station->all_tache[tache_a_deplacer] = 0;
                    nouvelle_station->all_tache[tache_a_deplacer] = 1;

                    printf("Tache %d deplacee vers une nouvelle station.\n", tache_a_deplacer);
                } else {
                    current_station->all_tache[tache_a_deplacer] = 0;
                    current_station->next->all_tache[tache_a_deplacer] = 1;

                    printf("Tache %d deplacee vers la station suivante.\n", tache_a_deplacer);
                }

                modification_effectuee = 1; // Indique qu'une modification a été effectuée
            }
        }

        previous_station = current_station;
        current_station = current_station->next;
    }

    return modification_effectuee;
}
