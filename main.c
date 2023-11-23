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
// Fonction auxiliaire pour effectuer un tri topologique sur le graphe
void topologicalSortUtil(Graph* graph, int v, bool visited[], int stack[], int* stackIndex) {
    visited[v] = true;
    printf("Visite du sommet: %d\n", v); // Débogage: indiquer quel sommet est visité

    Node* temp = graph->array[v].head;
    while (temp) {
        if (!visited[temp->vertex]) {
            topologicalSortUtil(graph, temp->vertex, visited, stack, stackIndex);
        }
        temp = temp->next;
    }

    stack[(*stackIndex)++] = v;
    printf("Ajouté à la pile: %d (Index de pile: %d)\n", v, *stackIndex - 1); // Débogage: indiquer quand un sommet est ajouté à la pile
}



void dfs(Graph* graph, int vertex, int* visited, int* stack, int* stackIndex) {
    visited[vertex] = 1;
    Node* temp = graph->array[vertex].head;

    while (temp != NULL) {
        if (!visited[temp->vertex]) {
            dfs(graph, temp->vertex, visited, stack, stackIndex);
        }
        temp = temp->next;
    }
    stack[(*stackIndex)++] = vertex;
}

void printTopologicalOrder(int* stack, int stackIndex) {
    for (int i = stackIndex - 1; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

// Modification: Cette fonction renvoie maintenant un int* pour la pile (stack)
int* topologicalSort(Graph* graph, int* stackIndex) {
    int* visited = (int*)calloc(graph->numVertices, sizeof(int));
    int* stack = (int*)malloc(graph->numVertices * sizeof(int));
    *stackIndex = 0;

    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            dfs(graph, i, visited, stack, stackIndex);
        }
    }

    free(visited);
    return stack; // Retourne la pile contenant l'ordre topologique
}


// Fonction pour déterminer si un sommet ou ses prédécesseurs sont dans un groupe donné
bool hasPredecessorInGroup(Graph* graph, int vertex, Group* group) {
    for (int i = 0; i < group->numVertices; i++) {
        int groupVertex = group->vertices[i];
        // Vérifiez si le sommet ou un de ses prédécesseurs est dans le groupe
        if (vertex == groupVertex) {
            return true;
        }
        // Vérifier les prédécesseurs du sommet
        Node* temp = graph->array[vertex].head;
        while (temp) {
            if (temp->vertex == groupVertex) {
                return true;
            }
            temp = temp->next;
        }
    }
    return false;
}

Group* groupVertices(Graph* graph, int* numGroups) {

    int stackIndex;
    int* order = topologicalSort(graph, &stackIndex); // Maintenant, nous passons l'adresse de stackIndex
    *numGroups = 0;
    Group* groups = (Group*) calloc(graph->numVertices, sizeof(Group));
    *numGroups = 0; // Initialisation du nombre de groupes
    for (int i = 0; i < graph->numVertices; i++) {
        int vertex = order[i];
        bool placed = false;

        // Parcourir les groupes existants pour placer le sommet
        for (int j = 0; j < *numGroups; j++) {
            if (!hasPredecessorInGroup(graph, vertex, &groups[j])) {
                // Placer le sommet dans le groupe actuel
                groups[j].vertices = realloc(groups[j].vertices, (groups[j].numVertices + 1) * sizeof(int));
                groups[j].vertices[groups[j].numVertices] = vertex;
                groups[j].numVertices++;
                placed = true;
                break;
            }
        }

        // Si le sommet n'a pas été placé, créez un nouveau groupe
        if (!placed) {
            groups[*numGroups].vertices = (int*) malloc(sizeof(int));
            groups[*numGroups].vertices[0] = vertex;
            groups[*numGroups].numVertices = 1;
            (*numGroups)++;
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