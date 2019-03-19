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

bool SECOND_DFS = false;
int time = 0, num_subnetworks = 0, subnetworkId = 0;
node_t subnetworksIds = NULL;
int articPointsCnt = 0, max_subnet_size = 0, tmp_subnet_size = 0;

node_t NEW(int number)
{
    node_t x = (node_t)malloc(sizeof(struct node));
    
    if (!(x == NULL))
    {
        x->num = number;
        x->next = NULL;    
    }
    
    return x;
}

router_t NewRouter()
{
    router_t router = (router_t)malloc(sizeof(struct router));
    
    if (!(router == NULL))
    {
        router->head = NULL;
        router->visited = false;
        router->parent = -1;
        router->articPoint = false;   
    }

    return router;
}

void mergeSortAux(int vector[], int l, int mid, int r)
{
    int i, j, k;
    int aux[r];
    for (i = mid + 1; i > l; i--)
        aux[i - 1] = vector[i - 1];
    for (j = mid; j < r; j++)
        aux[r + mid - j] = vector[j + 1];
    for (k = l; k <= r; k++)
    {
        if (aux[j] < aux[i])
            vector[k] = aux[j--];
        else
            vector[k] = aux[i++];
    }
}

void mergeSort(int vector[], int l, int r)
{
    int mid = (r + l) / 2;
    if (r <= l)
        return;
    mergeSort(vector, l, mid);
    mergeSort(vector, mid + 1, r);
    mergeSortAux(vector, l, mid, r);
}

void listToArray(node_t *head, int vector[])
{
    node_t tmp = *head, prev = NULL;
    int i = 0;
    
    while (tmp != NULL)
    {
        vector[i++] = tmp->num;
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
}

void addtoList(node_t *head, int number)
{
    node_t x = NEW(number);

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
    router[i]->visited = true;
    if (router[i]->articPoint && SECOND_DFS)
        return;
    if (i + 1 > subnetworkId)
        subnetworkId = i + 1;
    router[i]->discovery = router[i]->low = time++;

    node_t x;
    for (x = router[i]->head; x != NULL; x = x->next)
    {

        if (router[x->num - 1]->visited == false)
        {
            if (router[x->num - 1]->articPoint && SECOND_DFS)
                continue;

            tmp_subnet_size++;
            numOfChilds++;
            router[x->num - 1]->parent = i;

            DFS(x->num - 1, router);
            if (router[i]->low > router[x->num - 1]->low)
                router[i]->low = router[x->num - 1]->low;

            if (router[i]->parent == -1 && numOfChilds > 1 && !SECOND_DFS)
            {

                if (!router[i]->articPoint)
                {
                    articPointsCnt++;
                    router[i]->articPoint = true;
                }
            }
            else if (router[i]->parent != -1 && router[x->num - 1]->low >= router[i]->discovery && !SECOND_DFS)
            {
                if (!router[i]->articPoint)
                {
                    articPointsCnt++;
                    router[i]->articPoint = true;
                }
            }
        }
        else if (x->num - 1 != router[i]->parent)
            if (router[i]->low > router[x->num - 1]->discovery)
            {
                router[i]->low = router[x->num - 1]->discovery;
            }
    }
}

void readConnections(router_t router[], int numRouters, int numConnections)
{
    int i = 0, res = 0, num1 = 0, num2 = 0;
    
    for (i = 0; i < numRouters; i++)
        router[i] = NULL;

    for (i = 0; i < numConnections; i++)
    {
        res = scanf("%d %d", &num1, &num2);
        if  (!res) 
            exit(EXIT_FAILURE);
        if (router[num1 - 1] == NULL)
            router[num1 - 1] = NewRouter();

        if (router[num2 - 1] == NULL)
            router[num2 - 1] = NewRouter();

        addtoList(&(router[num1 - 1])->head, num2);
        addtoList(&(router[num2 - 1])->head, num1);
    }
}

int main()
{
    int num_routers, num_connections, res;

    res = scanf("%d", &num_routers);
    res = scanf("%d", &num_connections);
    
    if (!res)
        exit(EXIT_FAILURE);
        
    router_t *router = (router_t*)malloc(sizeof(router_t)*num_routers);
    

    readConnections(router, num_routers, num_connections);

    int i = 0;

    for (i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false)
        {
            subnetworkId = 0;
            num_subnetworks++;
            DFS(i, router);
            addtoList(&subnetworksIds, subnetworkId);
        }
    }

    for (i = 0; i < num_routers; i++)
    {
        router[i]->visited = false;
        router[i]->parent = -1;
    }
    
    time = 0;
    SECOND_DFS = true;
    tmp_subnet_size = 1;

    for (i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false)
        {
            DFS(i, router);
            if (tmp_subnet_size > max_subnet_size)
                max_subnet_size = tmp_subnet_size;
            tmp_subnet_size = 1;
        }
    }
    
    int* subnet = (int*)malloc(sizeof(int)*num_subnetworks);
    listToArray(&subnetworksIds,subnet);
    mergeSort(subnet, 0, num_subnetworks - 1);
    
    for (i = 0; i < num_routers; i++)
    {
        clearList(router[i]->head);
        free(router[i]);
    }

    printf("%d\n", num_subnetworks);
    
    for (i = 0; i < num_subnetworks - 1; i++)
        printf("%d ", subnet[i]);
        
    printf("%d\n%d\n%d\n", subnet[i], articPointsCnt, max_subnet_size);
    
    free(subnet);
    free(router);
    
    return 0;
}
