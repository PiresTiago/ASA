#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

int numSuppliers, numStations, numConnections;

typedef struct edge
{
    int source, dest;
    int flow;
    struct edge *next;
} * edge_t;

typedef struct vertex
{
    int height, excess;
    edge_t edgeList;
} * vertex_t;

/*GLOBAL VARS*/
vertex_t *graph = NULL;
int *listOfVertices;

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

edge_t newEdge(int src, int dest, int max)
{
    edge_t x = mallocAndVerify(sizeof(struct edge));

    if (!(x == NULL))
    {
        x->source = src;
        x->dest = dest;
        x->flow = max;
        x->next = NULL;
    }
    return x;
}

void addtoList(edge_t *head, int src, int dest, int max)
{
    edge_t x = newEdge(src, dest, max);

    x->next = *head;
    *head = x;
}

vertex_t newVertex()
{
    vertex_t vertex = mallocAndVerify(sizeof(struct vertex));

    vertex->height = 0;
    vertex->excess = 0;
    vertex->edgeList = NULL;

    return vertex;
}

void Preflow()
{
    int i = 0, j = 0, srcTmp = 0, destTmp = 0, pathCapTmp = 0;
    int maxCapacityTmp = 0, total = 0;

    scanfAndVerify(&numSuppliers);
    scanfAndVerify(&numStations);
    scanfAndVerify(&numConnections);

    total = numSuppliers + numStations * 2 + 2;

    graph = mallocAndVerify(sizeof(vertex_t) * total);

    graph[0] = newVertex(); /* Source */
    graph[0]->height = total;

    graph[1] = newVertex(); /* Supermarket*/

    for (i = 2; i < (numSuppliers + 2); i++) /* Suppliers */
    {
        graph[i] = newVertex();

        scanfAndVerify(&maxCapacityTmp);

        graph[i]->excess = maxCapacityTmp;
        graph[0]->excess -= maxCapacityTmp;

        addtoList(&graph[0]->edgeList, 0, i, 0);
        addtoList(&graph[i]->edgeList, i, 0, maxCapacityTmp);
    }

    for (j = i; j < (numSuppliers + numStations + 2); j++) /* Stations */
    {
        graph[j] = newVertex();
        graph[j + numStations] = newVertex();

        scanfAndVerify(&maxCapacityTmp);
        addtoList(&graph[j]->edgeList, j, j + numStations, maxCapacityTmp);
        addtoList(&graph[j + numStations]->edgeList, j + numStations, j, 0);
    }

    for (i = 1; i <= numConnections; i++) /*Connections*/
    {
        scanfAndVerify(&srcTmp);
        scanfAndVerify(&destTmp);
        scanfAndVerify(&pathCapTmp);

        if (srcTmp >= 2 && srcTmp <= numSuppliers + 1)
        {
            addtoList(&graph[srcTmp]->edgeList, srcTmp, destTmp, pathCapTmp);
            addtoList(&graph[destTmp]->edgeList, destTmp, srcTmp, 0);
        }

        else
        {
            addtoList(&graph[srcTmp + numStations]->edgeList, srcTmp + numStations, destTmp, pathCapTmp);
            addtoList(&graph[destTmp]->edgeList, destTmp, srcTmp + numStations, 0);
        }
    }
}

void Push(vertex_t u, vertex_t v)
{
    edge_t uv, vu;
    int add = 0;

    if (u->edgeList == NULL || v->edgeList == NULL)
        return;

    for (uv = u->edgeList; uv != NULL; uv = uv->next)
    {
        if (uv->dest == v->edgeList->source) /*checks if uv edge destination == v*/
        {                                    /*uv edge exists and uv = uv edge*/
            add = min(u->excess, uv->flow);
            uv->flow -= add;
            break;
        }
    }

    for (vu = v->edgeList; vu != NULL; vu = vu->next)
    {
        if (vu->dest == u->edgeList->source) /*checks if vu edge destination == u*/
        {                                    /*vu edge exists and vu = vu edge*/
            vu->flow += add;
            break;
        }
    }

    u->excess -= add;
    v->excess += add;
}

void Relabel(vertex_t u)
{
    int minHeight = u->height;
    edge_t edge;

    for (edge = u->edgeList; edge != NULL; edge = edge->next)
        minHeight = min(u->height, minHeight);

    u->height = 1 + minHeight;
}

void Discharge(vertex_t u)
{
    edge_t edgeToNeighbor = u->edgeList; /*edge to first neighbor*/
    edge_t uv;
    vertex_t v;

    while (u->excess > 0)
    {
        v = graph[edgeToNeighbor->dest]; /*neighbor*/
        if (v == NULL)
        {
            Relabel(u);
            edgeToNeighbor = u->edgeList; /*edge to first neighbor*/
        }

        for (uv = u->edgeList; uv != NULL; uv = uv->next)
        {
            if (uv->dest == v->edgeList->source) /*checks if uv edge destination == v*/
                break;                           /*uv edge exists and uv = uv edge*/
        }
    
        if (uv != 0 && u->height == (v->height + 1)) /*(uv!=0) == (cf(u,v)>0)*/
            Push(u, v);

        else edgeToNeighbor = edgeToNeighbor->next; /*edge to next neighbor*/
    }
}

/*
void RelabelToFront()
{
    int i = 0;
    edge_t j = NULL;
    for (i = 0; i < numSuppliers + 1; i++)
    {
        for (j = graphNode[i]->destinations; j != NULL; j = j->next)
            PushRelabel_PUSH(graphNode[i], graphNode[j->destination], j->destination, i);
    }

    for (i = numSuppliers + 1; i < (numSuppliers + numStations + 1); i++)
    {
        if (graphNode[i]->flow > 0)
        {
            PushRelabel_RELABEL(graphNode[i]);
            for (j = graphNode[i]->destinations; j != NULL; j = j->next)
                PushRelabel_PUSH(graphNode[i], graphNode[j->destination], j->destination, i);
        }
    }
}*/

void printGraph()
{
    int i;
    edge_t j;

    for (i = 0; i < (numSuppliers + numStations * 2 + 2); i++)
    {
        printf("ID: %d | h:%d | e:%d \n",
               i, graph[i]->height, graph[i]->excess);
        for (j = graph[i]->edgeList; j != NULL; j = j->next)
        {
            printf("    ID:%d | f:%d\n",
                   j->dest, j->flow);
        }
    }
    printf("\n\nNum de Fornecedores:%d\nNum de Estações:%d\nNum de ligacoes:%d\n\n",
           numSuppliers, numStations, numConnections);
}

int main()
{
    Preflow();
    printGraph();
    Discharge(graph[2]);
    printGraph();

    listOfVertices = mallocAndVerify(sizeof(int) * numSuppliers + numStations);
    return EXIT_SUCCESS;
}