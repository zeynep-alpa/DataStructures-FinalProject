#include <stdio.h>
#include <stdlib.h>

// Faaliyetleri temsil etmek için bir kenar yapısı (struct Edge) tanımlanır.
struct Edge {
    int source;//kenarın kaynağı
    int target;//kenarın hedefi
    int weight;//kenarın ağırlığı(aktivite süresi)
    char label;//kenarın etiketi (aktivite adı)
};

// Graf yapısını temsil etmek için bir Graph yapısı (struct Graph) tanımlanır.
struct Graph {
    int numVertices;//grafın düğüm sayısı
    int numEdges;//grafın kenar sayısı (aktivite sayısı)
    struct Edge *edges;//grafın kenarlarını temsil eden Edge yapısı dizisi
};


// Belirli bir düğüm ve kenar sayısıyla bir graf yapısı oluşturulur ve geri döndürülür.
struct Graph* createGraph(int numVertices, int numEdges) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->edges = (struct Edge*) malloc(numEdges * sizeof(struct Edge));
    return graph;
}

// Bir grafa bir kenar eklenir.
void addEdge(struct Graph* graph, int index, int source, int target, int weight, char label) {
    graph->edges[index].source = source;
    graph->edges[index].target = target;
    graph->edges[index].weight = weight;
    graph->edges[index].label = label;
}

// Bir grafı ekrana yazdırır.
void printGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numEdges; i++) {
        struct Edge edge = graph->edges[i];
        printf("%d %d %d %c\n", edge.source, edge.target, edge.weight, edge.label);
    }
}

// Grafın hafızasını serbest bırakır.
void freeGraph(struct Graph* graph) {
    free(graph->edges);
    free(graph);
}

// İki sayı arasındaki maksimum değeri döndürür.
int max(int a, int b) {
    return (a > b) ? a : b;
}

// İki sayı arasındaki minimum değeri döndürür.
int min(int a, int b) {
    return (a < b) ? a : b;
}

// En erken tamamlanma süresini hesaplar.
int calculateSPCT(struct Graph* graph) {
    int* earliestCompletionTime = (int*) malloc(graph->numVertices * sizeof(int));

  // Tüm faaliyetlerin en erken tamamlanma süresini 0 ile başlatır.  
  for (int i = 0; i < graph->numVertices; i++) {
        earliestCompletionTime[i] = 0;
    }

  // En erken tamamlanma sürelerini günceller.  
  for (int i = 0; i < graph->numEdges; i++) {
        struct Edge edge = graph->edges[i];
        earliestCompletionTime[edge.target] = max(earliestCompletionTime[edge.target], earliestCompletionTime[edge.source] + edge.weight);
    }

    // En büyük en erken tamamlanma süresini bulur.
    int spct = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        spct = max(spct, earliestCompletionTime[i]);
    }

    free(earliestCompletionTime);

    return spct;
}

    // En erken başlama sürelerini hesaplar.
    int* calculateEST(struct Graph* graph) {
    int* earliestStartTime = (int*) malloc(graph->numVertices * sizeof(int));

    // Tüm faaliyetlerin en erken başlama süresini 0 ile başlatır.
      for (int i = 0; i < graph->numVertices; i++) {
        earliestStartTime[i] = 0;
    }

    // En erken başlama sürelerini günceller.
      for (int i = 0; i < graph->numEdges; i++) {
        struct Edge edge = graph->edges[i];
        earliestStartTime[edge.target] = max(earliestStartTime[edge.target], earliestStartTime[edge.source] + edge.weight);
    }

    return earliestStartTime;
}

// En geç başlama sürelerini hesaplar.
int* calculateLST(struct Graph* graph, int spct) {
    int* latestStartTime = (int*) malloc(graph->numVertices * sizeof(int));

    // Tüm faaliyetlerin en geç başlama süresini SPCT ile başlatır.
    for (int i = 0; i < graph->numVertices; i++) {
        latestStartTime[i] = spct;
    }

    // Tüm faaliyetlerin en geç başlama sürelerini günceller.
    for (int i = graph->numEdges - 1; i >= 0; i--) {
        struct Edge edge = graph->edges[i];
        latestStartTime[edge.source] = min(latestStartTime[edge.source], latestStartTime[edge.target] - edge.weight);
    }

    return latestStartTime;
}

// Kritik yolun faaliyetlerini ekrana yazdırır.
void findCriticalPath(struct Graph* graph, int* earliestStartTime, int* latestStartTime) {
    printf("Critical Path: ");

    // Slack değeri 0 olan faaliyetleri kritik yola ekler.
        for (int i = 0; i < graph->numEdges; i++) {
        struct Edge edge = graph->edges[i];
        int slack = latestStartTime[edge.target] - earliestStartTime[edge.source] - edge.weight;
        if (slack == 0) {
            printf("%c ", edge.label);
        }
    }

    printf("\n");
}

// Bir dosyadan grafi okur ve oluşturur.
struct Graph* readGraphFromFile(char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("The file couldn't be opened.\n");
        return NULL;
    }

    int numVertices, numEdges;
    fscanf(file, "%d", &numEdges);
    numVertices = numEdges + 1;

    struct Graph* graph = createGraph(numVertices, numEdges);

    for (int i = 0; i < numEdges; i++) {
        int source, target, weight;
        char label;
        fscanf(file, "%d %d %d %c", &source, &target, &weight, &label);
        addEdge(graph, i, source, target, weight, label);
    }

    fclose(file);

    return graph;
}

// Grafı ekrana yazar.
void displayGraph(struct Graph* graph) {
    printf("Graph:\n");
    printGraph(graph);
    printf("\n");
}

// SPCT'yi ekrana yazar.
void displaySPCT(struct Graph* graph, int spct) {
    printf("SPCT: %d\n", spct);
}

// EST'yi ekrana yazar.
void displayEST(struct Graph* graph, int* earliestStartTime) {
    printf("EST: ");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d ", earliestStartTime[i]);
    }
    printf("\n");
}

// LST'yi ekrana yazar.
void displayLST(struct Graph* graph, int* latestStartTime) {
    printf("LST: ");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d ", latestStartTime[i]);
    }
    printf("\n");
}

// Kritik yolu ekrana yazar.
void displayCriticalPath(struct Graph* graph, int* earliestStartTime, int* latestStartTime) {
    printf("Critical Path: ");

    // Slack değeri 0 olan faaliyetleri kritik yola ekler.
        for (int i = 0; i < graph->numEdges; i++) {
        struct Edge edge = graph->edges[i];
        int slack = latestStartTime[edge.target] - earliestStartTime[edge.source] - edge.weight;
        if (slack == 0) {
            printf("%c ", edge.label);
        }
    }

    printf("\n");
}

// Menüyü ekrana yazar.
void printMenu() {
    printf("\nProject Management Interface\n\n");
    printf("1. Read the graph from the file\n");
    printf("2. Update the durations of the activities\n");
    printf("3. Calculate the critical path\n");
    printf("4. Calculate the EST\n");
    printf("5. Calculate the LST\n");
    printf("6. Calculate the activity delays\n");
    printf("7. Show the SPTC\n");
    printf("0. Exit\n");
    printf("Make your selection: ");
}

int main() {
    struct Graph* graph = NULL;
    int spct = 0;
    int* earliestStartTime = NULL;
    int* latestStartTime = NULL;

    int choice;
    do {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (graph != NULL) {
                    freeGraph(graph);
                }
                graph = readGraphFromFile("graph.txt");
                if (graph == NULL) {
                    return 0;
                }
                displayGraph(graph);
                break;
            case 2:
                if (graph == NULL) {
                    printf("The graph hasn't been loaded yet.\n");
                } else {
                    printf("Update the activity durations:\n");
                    for (int i = 0; i < graph->numEdges; i++) {
                        int weight;
                        printf("%c: ", graph->edges[i].label);
                        scanf("%d", &weight);
                        graph->edges[i].weight = weight;
                    }
                    printf("The activity durations have been updated.\n");
                    displayGraph(graph);
                }
                break;
            case 3:
                if (graph == NULL) {
                    printf("The graph hasn't been loaded yet.\n");
                } else {
                    if (earliestStartTime == NULL || latestStartTime == NULL) {
                        spct = calculateSPCT(graph);
                        earliestStartTime = calculateEST(graph);
                        latestStartTime = calculateLST(graph, spct);
                    }
                    displayCriticalPath(graph, earliestStartTime, latestStartTime);
                }
                break;
            case 4:
                if (graph == NULL) {
                    printf("The graph hasn't been loaded yet.\n");
                } else {
                    if (earliestStartTime == NULL) {
                        earliestStartTime = calculateEST(graph);
                    }
                    displayEST(graph, earliestStartTime);
                }
                break;
            case 5:
                if (graph == NULL) {
                    printf("The graph hasn't been loaded yet.\n");
                } else {
                    if (latestStartTime == NULL) {
                        spct = calculateSPCT(graph);
                        latestStartTime = calculateLST(graph, spct);
                    }
                    displayLST(graph, latestStartTime);
                }
                break;
            case 6:
                if (graph == NULL) {
                    printf("The graph hasn't been loaded yet.\n");
                } else {
                    if (earliestStartTime == NULL || latestStartTime == NULL) {
                        spct = calculateSPCT(graph);
                        earliestStartTime = calculateEST(graph);
                        latestStartTime = calculateLST(graph, spct);
                    }
                    printf("Activity Delays:\n");
                    for (int i = 0; i < graph->numEdges; i++) {
                        struct Edge edge = graph->edges[i];
                        int slack = latestStartTime[edge.target] - earliestStartTime[edge.source] - edge.weight;
                        printf("%c: %d\n", edge.label, slack);
                    }
                }
                break;
          case 7:
    if (graph == NULL) {
        printf("The graph hasn't been loaded yet.\n");
    } else {
        if (earliestStartTime == NULL || latestStartTime == NULL) {
            spct = calculateSPCT(graph);
            earliestStartTime = calculateEST(graph);
            latestStartTime = calculateLST(graph, spct);
        }
        displaySPCT(graph, spct);
    }
    break;

            case 0:
                printf("Exiting the session.\n");
                break;
            default:
                printf("Invalid selection. Please try again.\n");
                break;
        }

    } while (choice != 0);

    if (earliestStartTime != NULL) {
        free(earliestStartTime);
    }
    if (latestStartTime != NULL) {
        free(latestStartTime);
    }
    if (graph != NULL) {
        freeGraph(graph);
    }

    return 0;
}