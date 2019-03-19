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

bool SEGUNDA_VOLTA=false;
int time = 0, num_subnetworks = 0, subnetworkId = 0;

int articPointsCnt = 0, max_subnet_size = 0, tmp_subnet_size = 0;

node_t NEW(int number)
{
    node_t x = (node_t)malloc(sizeof(struct node));
    x->num = number;
    x->next = NULL;
    return x;
}

router_t NewRouter()
{
    router_t router = (router_t)malloc(sizeof(struct router));
    router->head = NULL;
    router->visited = false;
    router->parent = -1;
    router->articPoint = false;
    
    return router;
}

void addtoListSORTED(node_t *head, int number)
{
    node_t x = NEW(number);
    if (*head == NULL)
    {
        x->next = *head;
        *head = x;
    }
    else
    {
        if (number < (*head)->num)
        {
            x->next = *head;
            *head = x;
        }
        else
        {
            node_t tmp = *head;
            while (tmp->next != NULL && tmp->next->num < x->num)
                tmp = tmp->next;

            x->next = tmp->next;
            tmp->next = x;
        }
    }
}

void mergeSortAux(int vector[], int l, int mid, int r)
{
    int i, j, k;
    int aux[r];
    for(i = mid + 1; i > l; i--)
        aux[i - 1] = vector[i - 1];
    for(j = mid; j < r; j++)
        aux[r + mid - j] = vector[j + 1];
    for(k = l; k <= r; k++)
    {
        if(aux[j] < aux[i])
            vector[k] = aux[j--];
        else
            vector[k] = aux[i++];
    }
}

void mergeSort(int vector[], int l, int r)
{
    int mid = (r + l) /2;
    if(r <= l)
        return;
    mergeSort(vector, l, mid);
    mergeSort(vector, mid + 1, r);
    mergeSortAux(vector, l, mid, r);
    
}

void listToArray(node_t *head, int vector[])
{
    node_t tmp;
    int i = 0;
    for (tmp = *head; tmp != NULL; tmp = tmp->next)
    {
        vector[i] = tmp->num;
        i++;
    }
}

void addtoList(node_t *head, int number)
{
    node_t n = NEW(number);
    n->next = *head;
    *head = n;
}

void printList(node_t head)
{
    node_t t;
    for (t = head; t != NULL; t = t->next)
        printf("%d\n", t->num);
}

void DFS(int i, router_t router[])
{ 
    int numOfChilds = 0;
    router[i]->visited = true;
    if (router[i]->articPoint && SEGUNDA_VOLTA)return;
    if (i + 1 > subnetworkId)
        subnetworkId = i + 1;
    router[i]->discovery = router[i]->low = time++;

    node_t x;
    for (x = router[i]->head; x != NULL; x = x->next)
    {
        
        if (router[x->num - 1]->visited == false )
        {
            if(router[x->num-1]->articPoint && SEGUNDA_VOLTA) continue;
            
            tmp_subnet_size++;
            numOfChilds++;
            router[x->num - 1]->parent = i;
            
            DFS(x->num - 1, router);
            if (router[i]->low > router[x->num - 1]->low)
                router[i]->low = router[x->num - 1]->low;

            if (router[i]->parent == -1 && numOfChilds > 1 && !SEGUNDA_VOLTA)
            {

                if (!router[i]->articPoint)
                {
                    articPointsCnt++;
                    router[i]->articPoint = true;
                }
            }
            else if (router[i]->parent != -1 && router[x->num - 1]->low >= router[i]->discovery && !SEGUNDA_VOLTA)
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

int main()
{
    int num_routers, num_connections;

    int num1, num2;

    scanf("%d", &num_routers);
    scanf("%d", &num_connections);
    router_t router[num_routers];
    
    int i = 0;
    node_t subnetworksIds;
    
    for (i = 0; i < num_routers; i++)
        router[i] = NULL;

    for (i = 0; i < num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);
        
        if (router[num1 - 1] == NULL)
            router[num1 - 1] = NewRouter();
            
        if (router[num2 - 1] == NULL)
            router[num2 - 1] = NewRouter();

        addtoList(&(router[num1 - 1])->head, num2);
        addtoList(&(router[num2 - 1])->head, num1);
    }

    for (i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false)
        {
            subnetworkId = 0;
            num_subnetworks++;
            DFS(i, router);
            printf("OLA");
            addtoList(&subnetworksIds, subnetworkId);
            
        }
    }

    for (i = 0; i < num_routers; i++)
    {
        router[i]->visited = false;
        router[i]->parent = -1;
    }
    time = 0;
    SEGUNDA_VOLTA=true;
    tmp_subnet_size=1;
    
    for (i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false)
        {
            DFS(i, router);
            if (tmp_subnet_size > max_subnet_size)
            {
                
                max_subnet_size = tmp_subnet_size;
            }
            tmp_subnet_size = 1;
        }
    }
    
    int subnetworks[num_subnetworks];
    listToArray(&subnetworksIds, subnetworks);
    mergeSort(subnetworks, 0, num_subnetworks);
    
    printf("%d\n", num_subnetworks);
    
    for (i = 0; i < num_subnetworks - 1; i++)
    {
        printf("%d ", subnetworks[i]);
    }
    printf("%d", subnetworks[i]);
    printf("\n");
    printf("%d\n", articPointsCnt);
    printf("%d\n", max_subnet_size);

    return 0;
}