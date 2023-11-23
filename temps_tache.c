#include "head.h"

Tache* lire_taches(char* nom_fichier, int n) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Crée un tableau de tâches de taille n
    Tache* taches = (Tache*)malloc(n * sizeof(Tache*));
    if (taches == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Initialise toutes les tâches avec un temps de 0
    for (int i = 0; i < n; i++) {
        taches[i].num_tache = i + 1;
        taches[i].temp = 0;
    }

    int num;
    float temp;

    while (fscanf(fichier, "%d %f", &num, &temp) == 2) {
        if (num >= 1 && num <= n) {
            // Met à jour le temps de la tâche correspondante
            taches[num - 1].temp = temp;
        }
    }

    fclose(fichier);
    return taches;
}


