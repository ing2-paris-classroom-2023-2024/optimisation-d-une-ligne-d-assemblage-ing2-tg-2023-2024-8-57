#include "head.h"

void charger_exclusions(station* stations, int ordre,char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
        return;
    }

    int tache1, tache2;
    while (fscanf(fichier, "%d %d", &tache1, &tache2) == 2) {
        // Mettre à jour la matrice d'exclusion pour les deux tâches
        stations->exclusion[tache1][tache2] = 1;
        stations->exclusion[tache2][tache1] = 1;
    }

    fclose(fichier);
}


// Fonction pour résoudre les conflits de station en tenant compte de la précédence et des exclusions

// Résoudre les conflits entre les tâches dans toutes les stations // Assurez-vous d'inclure le bon header

void resoudre_conflits(station* stations, int ordre) {
    int conflit_resolu = 1;  // Indicateur pour vérifier si un conflit a été résolu

    while (conflit_resolu) {
        conflit_resolu = 0;  // Réinitialise l'indicateur de conflit résolu

        // Parcours de toutes les stations
        station* current_station = stations;
        while (current_station != NULL) {
            // Parcourir toutes les paires de tâches incompatibles
            for (int tache1 = 0; tache1 < ordre; tache1++) {
                for (int tache2 = tache1 + 1; tache2 < ordre; tache2++) {
                    if (stations->exclusion[tache1][tache2] == 1) {
                        // Les tâches tache1 et tache2 sont incompatibles
                        if (current_station->all_tache[tache1] == 1 && current_station->all_tache[tache2] == 1) {
                            // Les tâches incompatibles se trouvent dans la même station
                            // Déterminez laquelle a moins de tâches après elle
                            int nb_taches_apres_tache1 = 0;
                            int nb_taches_apres_tache2 = 0;
                            for (int i = tache1 + 1; i < ordre; i++) {
                                nb_taches_apres_tache1 += current_station->all_tache[i];
                            }
                            for (int i = tache2 + 1; i < ordre; i++) {
                                nb_taches_apres_tache2 += current_station->all_tache[i];
                            }

                            if (nb_taches_apres_tache1 < nb_taches_apres_tache2) {
                                // Déplacez tache1 vers la station suivante
                                current_station->all_tache[tache1] = 0;
                                station* station_suivante = current_station->next;
                                station_suivante->all_tache[tache1] = 1;
                            } else {
                                // Déplacez tache2 vers la station suivante
                                current_station->all_tache[tache2] = 0;
                                station* station_suivante = current_station->next;
                                station_suivante->all_tache[tache2] = 1;
                            }

                            conflit_resolu = 1;  // Indiquer qu'un conflit a été résolu
                            break;
                        }
                    }
                }
            }
            current_station = current_station->next;
        }
    }
}









