#include "head.h"
// Fonction pour créer un nouveau sommet
Node* createNode(int vertex, float time) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->time = time;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int vertices) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->array = (AdjList*) malloc(vertices * sizeof(AdjList));
    graph->times = (float*) calloc(vertices, sizeof(float)); // Initialisation du tableau des temps

    for (int i = 0; i < vertices; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}

// Fonction pour ajouter une arête au graphe
void addEdge(Graph* graph, int src, int dest) {
    printf("Ajout de l'arête : %d -> %d\n", src, dest); // Débogage: affichage de l'ajout d'une arête
    Node* newNode = createNode(dest, 0); // Le temps sera ajouté plus tard
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// Fonction pour trouver le nombre total de sommets
int findTotalVertices(char* filename) {
    FILE* file = fopen(filename, "r");
    int maxVertex = 0, src, dest;

    if (!file) {
        printf("Erreur lors de l'ouverture du fichier d'arêtes.\n");
        return -1;
    }

    while (fscanf(file, "%d %d", &src, &dest) != EOF) {
        if (src > maxVertex) {
            maxVertex = src;
        }
        if (dest > maxVertex) {
            maxVertex = dest;
        }
    }

    fclose(file);
    return maxVertex + 1; // +1 car les sommets peuvent commencer à 0
}

// Fonction pour ajouter les temps aux sommets
void addTimesToGraph(Graph* graph, char* filename) {
    FILE* file = fopen(filename, "r");
    int vertex;
    float time;

    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de temps");
        exit(EXIT_FAILURE);
    }

    // Initialiser tous les temps à une valeur qui indique qu'ils ne sont pas définis.
    for (int i = 0; i < graph->numVertices; i++) {
        graph->times[i] = -1.0;
    }

    while (fscanf(file, "%d %f", &vertex, &time) == 2) {
        if (vertex < graph->numVertices) {
            graph->times[vertex] = time;
        }
    }

    fclose(file);
}

// Fonction pour imprimer le graphe
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        Node* temp = graph->array[v].head;
        printf("\nListe d'adjacence du sommet %d\n", v);
        while (temp) {
            printf("(%d, %0.2f) -> ", temp->vertex, temp->time);
            temp = temp->next;
        }
        printf("\n");
    }
}
// Fonction pour effectuer un tri topologique sur le graphe
void topologicalSortUtil(Graph* graph, int v, bool visited[], int stack[], int* stackIndex) {
    // Marquer le sommet actuel comme visité
    visited[v] = true;

    // Récurse pour tous les sommets adjacents à ce sommet
    Node* temp = graph->array[v].head;
    while (temp) {
        if (!visited[temp->vertex]) {
            topologicalSortUtil(graph, temp->vertex, visited, stack, stackIndex);
        }
        temp = temp->next;
    }

    // Pousser le sommet actuel dans la pile qui stocke le résultat
    stack[(*stackIndex)++] = v;
}

// La fonction qui fait le tri topologique et retourne une pile avec l'ordre des sommets
int* topologicalSort(Graph* graph) {
    int* stack = (int*) malloc(graph->numVertices * sizeof(int));
    bool* visited = (bool*) calloc(graph->numVertices, sizeof(bool));
    int stackIndex = 0;

    // Appeler la fonction auxiliaire récursive pour stocker le tri topologique
    // en commençant par tous les sommets un par un
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            topologicalSortUtil(graph, i, visited, stack, &stackIndex);
        }
    }

    free(visited);
    return stack; // La pile contient l'ordre des sommets en tri topologique
}
Group* groupVertices(Graph* graph, int* numGroups,int max_temps) {
    int* order = topologicalSort(graph); // Assurez-vous que cette fonction renvoie l'ordre topologique correct
    *numGroups = 0;
    Group* groups = (Group*) calloc(graph->numVertices, sizeof(Group));

    // Initialiser les groupes
    for (int i = 0; i < graph->numVertices; i++) {
        groups[i].vertices = NULL;
        groups[i].numVertices = 0;
        groups[i].totalTime = 0.0;
    }

    // Parcourir les sommets en ordre topologique inversé pour les ajouter aux groupes
    for (int i = graph->numVertices - 1; i >= 0; i--) {
        int vertex = order[i];
        float vertexTime = graph->times[vertex];

        if (vertexTime <= 0) continue; // Ignorer les sommets sans temps

        bool placed = false;
        for (int j = 0; j < *numGroups; j++) {
            if (groups[j].totalTime + vertexTime <= max_temps) {
                groups[j].vertices = realloc(groups[j].vertices, (groups[j].numVertices + 1) * sizeof(int));
                groups[j].vertices[groups[j].numVertices] = vertex;
                groups[j].totalTime += vertexTime;
                groups[j].numVertices++;
                placed = true;
                break;
            }
        }

        if (!placed) {
            groups[numGroups].vertices = (int) malloc(sizeof(int));
            groups[*numGroups].vertices[0] = vertex;
            groups[*numGroups].totalTime = vertexTime;
            groups[*numGroups].numVertices = 1;
            (*numGroups)++;
        }
    }

    free(order);
    return groups;
}

station* init_station() {

    station* liste_station;
    printf("Début du programme.\n");
    int temp_cycle_max =0;

    FILE* f = fopen("cycle.txt", "r");
    fscanf(f,"%d",&temp_cycle_max);

    char fileEdges[] = "graphe.txt";
    char fileTimes[] = "temps.txt";

    // Trouver le nombre total de sommets à partir du fichier d'arêtes
    int totalVertices = findTotalVertices(fileEdges);
    printf("Nombre total de sommets trouvé : %d\n", totalVertices);

    if (totalVertices <= 0) {
        printf("Erreur lors de la lecture du fichier d'arêtes ou aucun sommet trouvé.\n");
    }

    // Créer le graphe
    Graph* graph = createGraph(totalVertices);

    // Lire le fichier des arêtes et ajouter des arêtes
    FILE* fileEdgesFp = fopen(fileEdges, "r");
    int src, dest;
    if (!fileEdgesFp) {
        printf("Erreur lors de l'ouverture du fichier d'arêtes.\n");
    }
    while (fscanf(fileEdgesFp, "%d %d", &src, &dest) != EOF) {
        addEdge(graph, src, dest);
    }
    fclose(fileEdgesFp);

    // Ajouter les temps aux sommets
    addTimesToGraph(graph, fileTimes);

    // Effectuer le tri topologique et grouper les sommets
    int numGroups;
    Group* groups = groupVertices(graph, &numGroups,temp_cycle_max);

    // Affichage des groupes
    printf("Nombre total de groupes formés : %d\n", numGroups);
    for (int i = 0; i < numGroups; i++) {
        printf("Groupe %d (Temps total: %0.2f): ", i, groups[i].totalTime);
        for (int j = 0; j < groups[i].numVertices; j++) {
            printf("%d ", groups[i].vertices[j]);
        }
        printf("\n");
    }


    // Libération de la mémoire allouée pour le graphe
    for (int i = 0; i < graph->numVertices; i++) {
        Node* temp;
        while ((temp = graph->array[i].head) != NULL) {
            graph->array[i].head = graph->array[i].head->next;
            free(temp);
        }
    }
    
    free(graph->array);
    free(graph);

    liste_station = CreerStations(numGroups,totalVertices);
    station* current = liste_station;
    for (int i = 0; i < numGroups; ++i) {
        for (int j = 0; j < totalVertices; ++j) {
            if(groups[i].vertices[j] == j){
                current->all_tache[j] = groups[i].vertices[j];
            }
        }
        current =current->next;
    }
    return liste_station;

}
