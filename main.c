#include <stdio.h>
#include <stdlib.h>

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

// Fonction principale
int main() {
    printf("Début du programme.\n"); // Débogage

    char fileEdges[] = "arrete.txt";
    char fileTimes[] = "temps2.txt";

    int totalVertices = findTotalVertices(fileEdges);
    printf("Nombre total de sommets trouvé : %d\n", totalVertices); // Débogage

    if (totalVertices == -1) {
        return 1; // Erreur lors de la lecture du fichier
    }

    Graph *graph = createGraph(totalVertices); // Pas besoin de +1 si on commence à 0

    // Lire le fichier des arêtes et ajouter des arêtes
    FILE *file = fopen(fileEdges, "r");
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

    printGraph(graph); // Affiche la liste d'adjacence et les temps

    printf("Fin du programme.\n"); // Débogage
    return 0;
}