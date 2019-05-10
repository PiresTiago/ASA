#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

int numSuppliers, numStations, numConnections;

typedef struct edge
{
    int source, dest;
    int flow, capacity;
    struct edge *next;
} * edge_t;

typedef struct vertex
{
    int height, excess;
    edge_t edgeList;
    struct vertex *next;
} * vertex_t;

/*GLOBAL VARS*/
vertex_t *graph = NULL;
vertex_t listOfVertices = NULL;

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

edge_t newEdge(int src, int dest, int flow, int cap)
{
    edge_t x = mallocAndVerify(sizeof(struct edge));

    if (!(x == NULL))
    {
        x->source = src;
        x->dest = dest;
        x->flow = flow;
        x->capacity = cap;
        x->next = NULL;
    }
    return x;
}

void addtoList(edge_t *head, int src, int dest, int flow, int cap)
{
    edge_t x = newEdge(src, dest, flow, cap);

    x->next = *head;
    *head = x;
}

void pushQueue(vertex_t v)
{
    vertex_t x= listOfVertices;

    x->next=v;
    v->next = listOfVertices->next;
    listOfVertices = x;
}

void popQueue()
{
    listOfVertices = listOfVertices->next;
}

void vertexAtBeginning(vertex_t v)
{
    vertex_t prev;
    for (prev = listOfVertices; prev->next != v; prev = prev->next);
    prev->next = v->next;
    v->next = listOfVertices;
    listOfVertices = v;
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

        addtoList(&graph[0]->edgeList, 0, i, 0, 0);
        addtoList(&graph[i]->edgeList, i, 0, 0, maxCapacityTmp);
        pushQueue(graph[i]);
    }

    for (j = i; j < (numSuppliers + numStations + 2); j++) /* Stations */
    {
        graph[j] = newVertex();
        graph[j + numStations] = newVertex();

        scanfAndVerify(&maxCapacityTmp);
        addtoList(&graph[j]->edgeList, j, j + numStations, 0, maxCapacityTmp);
        addtoList(&graph[j + numStations]->edgeList, j + numStations, j, 0, 0);
    }

    for (i = 1; i <= numConnections; i++) /*Connections*/
    {
        scanfAndVerify(&srcTmp);
        scanfAndVerify(&destTmp);
        scanfAndVerify(&pathCapTmp);

        if (srcTmp > numSuppliers + 1) /*Source is station */
        {
            addtoList(&graph[srcTmp + numStations]->edgeList, srcTmp + numStations, destTmp, 0, pathCapTmp);
            addtoList(&graph[destTmp]->edgeList, destTmp, srcTmp + numStations, 0, 0);
        }

        else /*Source is others*/
        {
            addtoList(&graph[srcTmp]->edgeList, srcTmp, destTmp, 0, pathCapTmp);
            addtoList(&graph[destTmp]->edgeList, destTmp, srcTmp, 0, 0);
        }
    }
}

void Push(vertex_t u, vertex_t v)
{
    edge_t uv, vu;
    int add = 0;

    for (uv = u->edgeList; uv != NULL; uv = uv->next)
    {
        if (uv->dest == v->edgeList->source) /*checks if uv edge destination == v*/
        {                                    /*uv edge exists and uv = uv edge*/
            add = min(u->excess, uv->capacity - uv->flow); /*vu edge exists and vu = vu edge*/
            uv->flow += add;
            break;
        }
    }


    for (vu = v->edgeList; vu != NULL; vu = vu->next)
    {
        if (vu->dest == u->edgeList->source) /*checks if vu edge destination == u*/
        {
            vu->flow = -uv->flow;
            break;
        }
    }

    u->excess -= add;
    if (u->excess==0)
        popQueue();
    v->excess += add;
    pushQueue(v);
}

void Relabel(vertex_t u)
{
    int minHeight = -1;
    edge_t edge;
    for (edge = u->edgeList; edge != NULL; edge = edge->next)
    {
        if (graph[edge->dest]->height >= u->height && edge->capacity != 0)
        {
            if (minHeight == -1)
                minHeight = graph[edge->dest]->height;
            else
                minHeight = min(graph[edge->dest]->height, minHeight);
        }
    }

    u->height = 1 + minHeight;
}
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
            /*printf("    ID:%d | f:%d\n",
                   j->dest, j->flow);*/
            printf("    DST:%d  CAPF:%d\n", j->dest, j->capacity- j->flow);
        }
    }
    printf("\n\nNum de Fornecedores:%d\nNum de Estações:%d\nNum de ligacoes:%d\n\n",
           numSuppliers, numStations, numConnections);
}
void printListOfVertices()
{
    vertex_t x;
    for (x = listOfVertices; x->next != NULL; x = x->next)
    {
        printf(" %d ->", x->edgeList->source);
    }
    printf(" %d\n", x->edgeList->source);
}
void Discharge(vertex_t u)
{
    edge_t edgeToNeighbor = u->edgeList; /*edge to first neighbor*/
    vertex_t v;

    while (u->excess > 0)
    {

        if (edgeToNeighbor == NULL)
        {
            Relabel(u);
            edgeToNeighbor = u->edgeList; /*edge to first neighbor*/            
        }
        else
        {
            v = graph[edgeToNeighbor->dest]; /*neighbor*/

            /*printf("ALTURA U:%d EXCESSO U:%d\n", u->height,u->excess);
            printf("ALTURA V:%d EXCESSO V:%d\n", v->height, v->excess);
            printf("SRC:%d  DST:%d\n", u->edgeList->source, edgeToNeighbor->dest);
            printf("FLOW AVAILABLE:%d\n\n", edgeToNeighbor->capacity-edgeToNeighbor->flow);
            printf("");*/
            if (edgeToNeighbor->capacity - edgeToNeighbor->flow > 0 && u->height == (v->height + 1)) /*(uv!=0) == (cf(u,v)>0)*/
                Push(u, v);
            else
                edgeToNeighbor = edgeToNeighbor->next; /*edge to next neighbor*/
        }
    }
}

void RelabelToFront()
{
    vertex_t u;
    int oldh = 0;
   /* Preflow();*/
    int x=0,y;
    u = listOfVertices;

    while (u != NULL)
    {
        /*printf("\n\nID VERTICE:%d\n\n", u->edgeList->source);*/
        oldh = u->height;

        Discharge(u);
        if (u->height > oldh)
            vertexAtBeginning(u);
        u = u->next;
        x++;
    }
}

int main()
{
    Preflow();
    RelabelToFront();
    printGraph();
    printListOfVertices();
    return EXIT_SUCCESS;
}