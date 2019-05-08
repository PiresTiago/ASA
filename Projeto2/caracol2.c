#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listNode
{
    int num;
    struct node *next;
} * lnode_t;

typedef struct graphNode
{
    int destination, capacity, pathCapacity;
    int height, excess;
} * gnode_t;

int numSuppliers, numStations, numConnections;
gnode_t *graphNode = NULL; /*index number+1 is vertex id*/

void *mallocAndVerify(size_t size)
{

    void *x = malloc(size);

    if (x == NULL)
    {
        printf("Error in malloc, exiting...\n");
        exit(EXIT_FAILURE);
    }
    return x;
}

void scanfAndVerify(int *number)
{
    int error = scanf("%d", number);

    if (error < 0)
    {
        printf("Error in scanf, exiting...\n");
        exit(EXIT_FAILURE);
    }
}

gnode_t newGraphNode()
{
    gnode_t node = mallocAndVerify(sizeof(struct graphNode));

    return node;
}

gnode_t *readInput()
{
    int i = 0, srcTmp;

    scanfAndVerify(&numSuppliers);
    scanfAndVerify(&numStations);
    scanfAndVerify(&numConnections);

    graphNode = mallocAndVerify(sizeof(gnode_t) * (numSuppliers + numStations + 1));

    for (i = 0; i < (numSuppliers + numStations + 1); i++)
    {
        graphNode[i] = newGraphNode();
    }

    for (i = 1; i < (numSuppliers + 1); i++)
    {
        scanfAndVerify(&graphNode[i]->capacity);
    }

    for (i = numSuppliers + 1; i < (numSuppliers + numStations + 1); i++)
    {
        scanfAndVerify(&(graphNode[i])->capacity);
    }

    for (i = 1; i < (numSuppliers + numStations + 1); i++)
    {
        scanfAndVerify(&srcTmp);
        scanfAndVerify(&graphNode[srcTmp - 1]->destination);
        scanfAndVerify(&graphNode[srcTmp - 1]->pathCapacity);
    }

    return graphNode;
}

int main()
{
    int i;

    graphNode = readInput();

    for (i = 0; i < (numSuppliers + numStations + 1); i++)
    {
        printf("Indice:%d Destino:%d Capacidade:%d Caminho:%d\n",
               i + 1, graphNode[i]->destination, graphNode[i]->capacity, graphNode[i]->pathCapacity);
    }
    printf("\n\nNum de Fornecedores:%d\nNum de Estações:%d\nNum de ligacoes:%d\n\n",
           numSuppliers, numStations, numConnections);

    return EXIT_SUCCESS;
}
