#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

typedef struct listNode
{
    int pathCapacity, pathFlow, destination;
    struct listNode *next;
} * lnode_t;

typedef struct graphNode
{
    int capacity;
    int height, flow;
    lnode_t destinations, sources;
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

lnode_t newNode(int dest, int cap)
{
    lnode_t x = mallocAndVerify(sizeof(struct listNode));

    if (!(x == NULL))
    {
        x->destination = dest;
        x->pathCapacity = cap;
        x->pathFlow = 0;
        x->next = NULL;
    }
    return x;
}

void addtoList(lnode_t *head, int dest, int cap)
{
    lnode_t x = newNode(dest, cap);

    x->pathFlow = 0; /*Pre-Flow*/
    x->next = *head;
    *head = x;
}

gnode_t newGraphNode()
{
    gnode_t node = mallocAndVerify(sizeof(struct graphNode));

    node->height = 0; /*Pre-Flow*/
    node->flow = 0;   /*Pre-Flow*/

    return node;
}

gnode_t *readInput()
{
    int i = 0, srcTmp, destTmp, pathCapTmp;
    lnode_t x, y;

    scanfAndVerify(&numSuppliers);
    scanfAndVerify(&numStations);
    scanfAndVerify(&numConnections);

    graphNode = mallocAndVerify(sizeof(gnode_t) * (numSuppliers + numStations + 1));
    graphNode[0] = newGraphNode();

    for (i = 1; i < (numSuppliers + 1); i++) /*Sources*/
    {
        graphNode[i] = newGraphNode();
        scanfAndVerify(&graphNode[i]->capacity);
        graphNode[i]->flow = graphNode[i]->capacity;
        graphNode[i]->height = numSuppliers + numStations + 1; /*Src heights=num of vertices*/
    }

    for (i = numSuppliers + 1; i < (numSuppliers + numStations + 1); i++) /*Stations*/
    {
        graphNode[i] = newGraphNode();
        scanfAndVerify(&(graphNode[i])->capacity);
    }

    for (i = 1; i < (numSuppliers + numStations + 1); i++)
    {
        scanfAndVerify(&srcTmp);
        scanfAndVerify(&destTmp);
        scanfAndVerify(&pathCapTmp);

        addtoList(&graphNode[srcTmp - 1]->destinations, destTmp, pathCapTmp);
        addtoList(&graphNode[destTmp - 1]->sources, srcTmp, pathCapTmp);
    }

    for (i = 1; i < numSuppliers; i++)
    {
        for (x = graphNode[i]->destinations; x != NULL; x = x->next)
        {
            x->pathFlow = min(graphNode[i]->capacity, x->pathCapacity);
            for (y = graphNode[x->destination - 1]->destinations; y != NULL; y = y->next)
            {
                if (y->destination == i)
                {
                    y->pathFlow = -x->pathFlow;
                    break;
                }
            }
            graphNode[x->destination - 1]->flow = x->pathFlow;
            graphNode[i]->flow -= x->pathFlow;
        }
    }

    return graphNode;
}

void PushRelabel_PUSH(gnode_t src, gnode_t dest, int destID, int srcID)
{
    lnode_t srcDest, destSrc;
    int add = 0;

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

    for (destSrc = dest->destinations; destSrc != NULL; destSrc = destSrc->next)
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

}

void testInput()
{
    int i;
    lnode_t j;

    for (i = 0; i < (numSuppliers + numStations + 1); i++)
    {
        printf("Indice:%d | Altura:%d | Fluxo:%d | Capacidade:%d\n",
               i + 1, graphNode[i]->height, graphNode[i]->flow, graphNode[i]->capacity);
        for (j = graphNode[i]->destinations; j != NULL; j = j->next)
        {
            printf("    Destino:%d FluxCaminho:%d CapCaminho:%d\n",
                   j->destination, j->pathFlow, j->pathCapacity);
        }
        for (j = graphNode[i]->sources; j != NULL; j = j->next)
        {
            printf("    Origem:%d FluxCaminho:%d CapCaminho:%d\n",
                   j->destination, j->pathFlow, j->pathCapacity);
        }
    }
    printf("\n\nNum de Fornecedores:%d\nNum de Estações:%d\nNum de ligacoes:%d\n\n",
           numSuppliers, numStations, numConnections);
}

int main()
{
    graphNode = readInput();
    testInput();
    PushRelabel_PUSH(graphNode[1], graphNode[4], 5, 2);
    testInput();
    return EXIT_SUCCESS;
}