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
    bool visited;
    node_t head, tail; //List of adjencies
} * router_t;

//VAR GLOBAIS
int time = 0;
node_t articPoints = NULL, subnetworksIds = NULL;
int articPointsCnt = 0;
////////////////////////////

//LIST//

node_t NEW(int number)
{
    node_t x = (node_t)malloc(sizeof(struct node));
    x->num = number;
    x->next = NULL;
    return x;
}

//adds element to list in ascending order
void addtoListSORTED(node_t *head, int number)
{
    node_t x = NEW(number);

    if (*head == NULL)
        *head = x;
    else
    {
        if (number < (*head)->num)
        {
            x->next = *head;
            *head = x;
        }

        node_t tmp, prev;
        tmp = *head;
        while (tmp != NULL && tmp->num <= number)
        {
            prev = tmp;
            tmp = tmp->next;
        }
        x->next = tmp;
        prev->next = x;
    }
}

void addtoList(node_t *head, node_t *tail, int number)
{
    node_t x = NEW(number);
    x->next = *head;
    *head = x;
    *tail = *head;
}

void printList(node_t head)
{
    node_t t;
    for (t = head; t != NULL; t = t->next)
        printf("%d\n", t->num);
}
////////////////////////////
////////////////////////////
void DFS(int i, router_t router[])
{

    router[i]->visited = true;
    router[i]->discovery = router[i]->low = time++;
    //printf("HERE IT GOES\nDISC:%d  LOW:%d\n\n",router[i]->discovery,router[i]->low);

    int child = 0;
    for (node_t x = router[i]->tail; x != NULL; x = x->next)
    {

        if (router[x->num - 1]->visited == false)
        {
            child++;
            router[x->num - 1]->parent = i;
            DFS(x->num - 1, router);


            if (router[i]->parent == 0 && child > 1)
            {
                articPointsCnt++;
                addtoListSORTED(&articPoints, i+1);
            }
            //printf("LOWCHILD:%d\nDISCPARENT:%d\n",router[x->num - 1]->low,router[i]->discovery);
            if (router[i]->parent != 0 && router[x->num - 1]->low >= router[i]->discovery)
            {
                articPointsCnt++;
                addtoListSORTED(&articPoints, i+1);
            }
        }
        else if (x->num != router[i]->parent)
            if (router[i]->low > router[x->num - 1]->discovery)
                router[i]->low = router[x->num - 1]->discovery;
    }
}

int main()
{
    int num_routers, num_connections;

    //Read Input
    int num1, num2;

    scanf("%d", &num_routers);
    scanf("%d", &num_connections);

    router_t router[num_routers];

    for (int i = 0; i < num_routers; i++)
    {
        router[i] = malloc(sizeof(struct router));
        router[i]->head = malloc(sizeof(struct node));
        router[i]->tail = malloc(sizeof(struct node));
        router[i]->head = NULL;
        router[i]->tail = NULL;
    }

    for (int i = 0; i < num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);
        addtoList(&(router[num1 - 1])->head,&(router[num1 - 1])->tail, num2);
        addtoList(&(router[num2 - 1])->head,&(router[num2 - 1])->tail, num1);
    }
    //----

    // IMPRIME LISTA DE ADJACENCIAS
    for (int i = 0; i < num_routers; i++)
    {
        printf("%d", i + 1);
        for (node_t j = router[i]->head; j != NULL; j = j->next)
        {
            printf("->%d", j->num);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false){
            DFS(i, router);
        }
            
    }

    for (node_t x = articPoints; x != NULL; x = x->next)
    {
        printf("%d ", x->num);
    }
    printf("\n");
    printf("%d\n", articPointsCnt);
    return 0;

    return 0;
}
