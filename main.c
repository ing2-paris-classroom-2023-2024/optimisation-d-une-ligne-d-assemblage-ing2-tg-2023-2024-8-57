#include <stdio.h>
#include <stdlib.h>
// Nouvelle structure pour stocker les groupes
typedef struct {
    float totalTime;
    int* vertices;
    int numVertices;
} Group;
// Structure pour un sommet
typedef struct Node {
    int vertex;
    float time;
    struct Node* next;
} Node;

// Structure pour la liste d'adjacence
typedef struct AdjList {
    Node* head;
} AdjList;

// Structure pour le graphe
typedef struct Graph {
    int numVertices;
    AdjList* array;
} Graph;

// Fonction pour créer un nouveau sommet
Node* createNode(int vertex, float time) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->time = time;
    newNode->next = NULL;
    return newNode;
}

// Fonction pour créer un graphe
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->numVertices = vertices;

    graph->array = (AdjList*) malloc(vertices * sizeof(AdjList));

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
    printf("Début de l'ajout des temps depuis le fichier %s\n", filename); // Débogage
    FILE* file = fopen(filename, "r");
    int vertex;
    float time;

    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de temps.\n");
        return;
    }

    while (fscanf(file, "%d %f", &vertex, &time) != EOF) {
        printf("Ajout du temps : sommet %d - temps %f\n", vertex, time); // Débogage
        Node* temp = graph->array[vertex].head;
        while (temp) {
            temp->time = time;
            temp = temp->next;
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

// Fonction pour grouper les sommets
Group* groupVertices(Graph* graph, int* numGroups) {
    int* order = topologicalSort(graph);
    Group* groups = (Group*) calloc(graph->numVertices, sizeof(Group));
    *numGroups = 0;

    for (int i = 0; i < graph->numVertices; i++) {
        int vertex = order[i];
        float time = graph->array[vertex].head ? graph->array[vertex].head->time : 0;
        bool placed = false;

        for (int j = 0; j <= *numGroups; j++) {
            if (groups[j].totalTime + time <= 1) {
                groups[j].totalTime += time;
                groups[j].vertices = realloc(groups[j].vertices, (groups[j].numVertices + 1) * sizeof(int));
                groups[j].vertices[groups[j].numVertices++] = vertex;
                placed = true;
                break;
            }
        }

        if (!placed) {
            groups[++(*numGroups)].totalTime += time;
            groups[*numGroups].vertices = (int*) malloc(sizeof(int));
            groups[*numGroups].vertices[0] = vertex;
            groups[*numGroups].numVertices = 1;
        }
    }

    free(order);
    return groups;
}
// Fonction principale
int main() {
    printf("Début du programme.\n");

    char fileEdges[] = "arrete.txt";
    char fileTimes[] = "temps2.txt";

    int totalVertices = findTotalVertices(fileEdges);
    printf("Nombre total de sommets trouvé : %d\n", totalVertices);

    if (totalVertices == -1) {
        return 1; // Erreur lors de la lecture du fichier
    }

    Graph* graph = createGraph(totalVertices);

    // Lire le fichier des arêtes et ajouter des arêtes
    FILE* file = fopen(fileEdges, "r");
    int src, dest;

    if (!file) {
        printf("Erreur lors de l'ouverture du fichier d'arêtes.\n");
        return 1;
    }

    while (fscanf(file, "%d %d", &src, &dest) != EOF) {
        addEdge(graph, src, dest);
    }
    fclose(file);

    // Ajouter les temps aux sommets
    addTimesToGraph(graph, fileTimes);

    // Grouper les sommets en respectant les contraintes
    int numGroups;
    Group* groups = groupVertices(graph, &numGroups);

    // Affichage des groupes
    for (int i = 0; i <= numGroups; i++) {
        printf("Groupe %d (Temps total: %0.2f): ", i, groups[i].totalTime);
        for (int j = 0; j < groups[i].numVertices; j++) {
            printf("%d ", groups[i].vertices[j]);
        }
        printf("\n");
        free(groups[i].vertices);
    }

    free(groups);

    printf("Fin du programme.\n");
    return 0;
}