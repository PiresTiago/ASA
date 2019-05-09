#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

int numSuppliers, numStations, numConnections;

typedef struct edgeNode
{
    int source, dest;
    int maxCapacity, flow;
    struct edgeNode* next;
} * enode_t;

typedef struct vertexNode
{
    int height, excess;
    enode_t edgeList;
} * gnode_t;

gnode_t* graph = NULL;

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
        printf("Error in scanf, exiting...%d\n", error);
        exit(EXIT_FAILURE);
    }
}

enode_t newEdgeNode(int src, int dest, int max)
{
    enode_t x = mallocAndVerify(sizeof(struct edgeNode));

    if (!(x == NULL))
    {
        x->source = src;
        x->dest = dest;
        x->maxCapacity = max;
        x->next = NULL;
    }
    return x;
}

void addtoList(enode_t *head, int src, int dest, int max)
{
    enode_t x = newEdgeNode(src, dest, max);

    x->next = *head;
    *head = x;
}

gnode_t newVertexNode()
{
    gnode_t node = mallocAndVerify(sizeof(struct vertexNode));

    node->height = 0;
    node->excess = 0;
    node->edgeList = NULL;
    return node;
}

void readInput()
{
    int i = 0, j = 0, srcTmp = 0, destTmp = 0, pathCapTmp = 0;
    int maxCapacityTmp = 0, total = 0;
    enode_t edge;

    scanfAndVerify(&numSuppliers);
    scanfAndVerify(&numStations);
    scanfAndVerify(&numConnections);

    total = numSuppliers + numStations*2 + 2;

    graph = mallocAndVerify(sizeof(gnode_t) * total);
    graph[0] = newVertexNode(); /* Source */
    graph[1] = newVertexNode(); /* Supermarket*/

    for (i = 2; i < (numSuppliers + 2); i++) /* Sources */
    {
       graph[i] = newVertexNode();
       scanfAndVerify(&maxCapacityTmp);
       addtoList(&graph[0]->edgeList, 0, i, maxCapacityTmp);
    }

    for (j = i; j < (numSuppliers + numStations + 2); j++) /* Stations */
    {
        graph[j] = newVertexNode();
        graph[j + numStations] = newVertexNode();
        scanfAndVerify(&maxCapacityTmp);
        addtoList(&graph[j]->edgeList, j, j + numStations, maxCapacityTmp);
    }

    for (i = 1; i <= numConnections; i++)
    {
        scanfAndVerify(&srcTmp);
        scanfAndVerify(&destTmp);
        scanfAndVerify(&pathCapTmp);

        if (srcTmp >= 2 && srcTmp <= numSuppliers + 1)
            addtoList(&graph[srcTmp]->edgeList, srcTmp, destTmp, pathCapTmp);
        else
            addtoList(&graph[srcTmp + numStations]->edgeList, srcTmp + numStations, destTmp, pathCapTmp);
        
        
    }

    for (edge = graph[0]->edgeList; edge != NULL; edge = edge->next)
    {
        edge->flow = edge->maxCapacity;
        graph[edge->dest]->excess = edge->maxCapacity;
        graph[0]->excess -= edge->maxCapacity;
    }
}

/*void PushRelabel_PUSH(gnode_t src, gnode_t dest, int destID, int srcID)
{
    lnode_t srcDest, destSrc;
    int add = 0;

    if (dest->sources == NULL || dest->destinations == NULL)
        return;

    for (srcDest = src->destinations; srcDest != NULL; srcDest = srcDest->next)
    {
        if (srcDest->destination == destID)
        {
            add = min(min(src->flow, src->capacity), srcDest->pathCapacity);
            srcDest->pathFlow += add;
            break;
        }
    }
    printf("VALUE ADD:%d\n", add);

    for (destSrc = dest->sources; destSrc != NULL; destSrc = destSrc->next)
    {
        if (destSrc->destination == srcID)
        {
            destSrc->pathFlow = -srcDest->pathFlow;
            break;
        }
    }

    src->flow -= add;
    dest->flow += add;
}

void PushRelabel_RELABEL(gnode_t src)
{
    int minHeight = src->height;
    lnode_t y;

    for (y = src->destinations; y != NULL; y = y->next)
        minHeight = min(graphNode[y->destination - 1]->height, minHeight);

    src->height = 1 + minHeight;
}

void PushRelabel()
{
    int i = 0;
    lnode_t j = NULL;
    for (i = 0; i < numSuppliers + 1; i++)
    {
        for (j = graphNode[i]->destinations; j != NULL; j = j->next)
            PushRelabel_PUSH(graphNode[i], graphNode[j->destination], j->destination, i);
    }

    for(i = numSuppliers + 1; i < (numSuppliers + numStations + 1); i++)
    {
        if (graphNode[i]->flow > 0)
        {
            PushRelabel_RELABEL(graphNode[i]);
            for (j = graphNode[i]->destinations; j != NULL; j = j->next)
                PushRelabel_PUSH(graphNode[i], graphNode[j->destination], j->destination, i);
        }
    }
}
*/
void testInput()
{
    int i;
    enode_t j;

    for (i = 0; i < (numSuppliers + numStations*2 + 2); i++)
    {
        printf("Indice: %d | Altura:%d | Excesso:%d \n",
               i, graph[i]->height, graph[i]->excess);
        for (j = graph[i]->edgeList; j != NULL; j = j->next)
        {
            printf("    Destino:%d FluxCaminho:%d CapCaminho:%d\n",
                   j->dest, j->flow, j->maxCapacity);
        }
    }
    printf("\n\nNum de Fornecedores:%d\nNum de Estações:%d\nNum de ligacoes:%d\n\n",
           numSuppliers, numStations, numConnections);
}

int main()
{
    readInput();
    testInput();
    /*PushRelabel_PUSH(graphNode[1], graphNode[4], 5, 2);*/
    /*PushRelabel();*/
    return EXIT_SUCCESS;
}