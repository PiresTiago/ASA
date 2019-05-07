#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int num;
    struct node *next;
} * node_t;

typedef struct router
{
    int discovery, low, parent;
    bool visited, articPoint;
    node_t head;
} * router_t;

/******************/
/*Global Variables*/
/******************/
bool secondDFS = false;
int time = 0, numSubnetworks = 0, subnetworkId = 0, articPointsCnt = 0,
    maxSubnetworkSize = 0, tmpSubnetworkSize = 0;
/******************/

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
    int x = scanf("%d\n", number);

    if (x < 0)
    {
        printf("Error in scanf, exiting...\n");
        exit(EXIT_FAILURE);
    }
}

node_t newNode(int number)
{
    node_t x = mallocAndVerify(sizeof(struct node));

    if (!(x == NULL))
    {
        x->num = number;
        x->next = NULL;
    }

    return x;
}

router_t newRouter()
{
    router_t router = mallocAndVerify(sizeof(struct router));

    if (!(router == NULL))
    {
        router->head = NULL;
        router->visited = false;
        router->parent = -1;
        router->articPoint = false;
    }

    return router;
}

void addtoList(node_t *head, int number)
{
    node_t x = newNode(number);

    x->next = *head;
    *head = x;
}

void clearList(node_t head)
{
    node_t tmp = head, prev = NULL;

    while (tmp != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
}

void DFS(int i, router_t router[])
{
    int numOfChilds = 0;
    node_t x;
    
    router[i]->visited = true;
    if (router[i]->articPoint && secondDFS)
        return;
    else if (i + 1 > subnetworkId)
        subnetworkId = i + 1;
    router[i]->discovery = router[i]->low = time++;

    for (x = router[i]->head; x != NULL; x = x->next)
    {
        if (router[x->num - 1]->visited == false && 
        !(router[x->num - 1]->articPoint && secondDFS))
        {
            tmpSubnetworkSize++;
            numOfChilds++;
            router[x->num - 1]->parent = i;

            DFS(x->num - 1, router);
            if (router[i]->low > router[x->num - 1]->low)
                router[i]->low = router[x->num - 1]->low;

            if (router[i]->parent == -1 && numOfChilds > 1
             && !router[i]->articPoint && !secondDFS)
            {
                articPointsCnt++;
                router[i]->articPoint = true;
            }
            else if (router[i]->parent != -1 && router[x->num - 1]->low >= 
            router[i]->discovery && !router[i]->articPoint && !secondDFS)
            {
                articPointsCnt++;
                router[i]->articPoint = true;
            }
        }
        else if (x->num - 1 != router[i]->parent)
            if (router[i]->low > router[x->num - 1]->discovery)
            {
                router[i]->low = router[x->num - 1]->discovery;
            }
    }
}

router_t *readInput(router_t *router, int **subnetworks, int *numRouters)
{
    int numConnections = 0, i = 0, num1 = 0, num2 = 0;

    scanfAndVerify(numRouters);
    scanfAndVerify(&numConnections);

    router = mallocAndVerify(sizeof(router_t) * *numRouters);
    *subnetworks = mallocAndVerify(sizeof(int) * *numRouters);

    for (i = 0; i < *numRouters; i++)
    {
        router[i] = NULL;
        (*subnetworks)[i] = 0;
    }

    for (i = 0; i < numConnections; i++)
    {
        scanfAndVerify(&num1);
        scanfAndVerify(&num2);
        if (router[num1 - 1] == NULL)
            router[num1 - 1] = newRouter();

        if (router[num2 - 1] == NULL)
            router[num2 - 1] = newRouter();

        addtoList(&router[num1 - 1]->head, num2);
        addtoList(&router[num2 - 1]->head, num1);
    }
    return router;
}

int main()
{
    int numRouters, i, *subnetworks = NULL;
    router_t *router = NULL;
    router = readInput(router, &subnetworks, &numRouters);

    for (i = 0; i < numRouters; i++)
    {
        if (router[i]->visited == false)
        {
            subnetworkId = 0;
            numSubnetworks++;
            DFS(i, router);
            subnetworks[subnetworkId - 1] = subnetworkId;
        }
    }

    for (i = 0; i < numRouters; i++)
    {
        router[i]->visited = false;
        router[i]->parent = -1;
    }

    secondDFS = true;
    tmpSubnetworkSize = 1;

    for (i = 0; i < numRouters; i++)
    {
        if (router[i]->visited == false)
        {
            DFS(i, router);

            if (tmpSubnetworkSize > maxSubnetworkSize)
                maxSubnetworkSize = tmpSubnetworkSize;
            tmpSubnetworkSize = 1;
        }
    }

    printf("%d\n", numSubnetworks);
    for (i = 0; i < numRouters - 1; i++)
    {
        if (subnetworks[i] != 0)
            printf("%d ", subnetworks[i]);
        clearList(router[i]->head);
        free(router[i]);
    }
    clearList(router[i]->head);
    free(router[i]);
    if (subnetworks[i] != 0)
        printf("%d", subnetworks[i]);
    printf("\n%d\n%d\n", articPointsCnt, maxSubnetworkSize);

    free(router);
    free(subnetworks);

    return 0;
}