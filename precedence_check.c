#include "head.h"

void lirePrecedence(char* nom_fichier, int** precedence, int ordre) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de précédence");
        exit(EXIT_FAILURE);
    }

    int s1, s2;
    while (fscanf(fichier, "%d %d", &s1, &s2) == 2) {
        if (s1 >= 0 && s1 < ordre && s2 >= 0 && s2 < ordre) {
            precedence[s1][s2] = 1;
        }
    }

    fclose(fichier);
}

int verifierPrecedence(station** liste_stations, int** precedence, int ordre) {
    int modification_effectuee = 0; // Drapeau pour indiquer si une modification a été faite

    // Parcours de toutes les stations pour vérifier la précédence
    station* current_station = *liste_stations;
    while (current_station != NULL) {
        for (int tache1 = 0; tache1 < ordre; tache1++) {
            if (current_station->all_tache[tache1] == 1) {
                // Vérifier la précédence pour la tache1
                for (int tache2 = 0; tache2 < ordre; tache2++) {
                    if (current_station->all_tache[tache1] == 1 && current_station->all_tache[tache2] == 1 && precedence[tache1][tache2]) {
                        // Déplacer la tâche de la station actuelle vers la station supérieure
                        if (current_station->next != NULL) {
                            current_station->all_tache[tache1] = 0;
                            current_station->next->all_tache[tache1] = 1;
                            printf("Tache %d déplacée vers la station suivante.\n", tache1);
                        } else {
                            // Créer une nouvelle station si la station actuelle est la dernière
                            station* nouvelle_station = (station*)malloc(sizeof(station));
                            nouvelle_station->all_tache = (int*)calloc(ordre, sizeof(int));
                            nouvelle_station->temps_tache = (double*)calloc(ordre, sizeof(double));
                            nouvelle_station->next = NULL;
                            nouvelle_station->all_tache[tache1] = 1;

                            current_station->next = nouvelle_station;
                            current_station->all_tache[tache1] = 0;
                            printf("Tache %d déplacée vers une nouvelle station.\n", tache1);
                        }
                        modification_effectuee = 1;
                    }
                }
            }
        }
        current_station = current_station->next;
    }

    return modification_effectuee;
}

