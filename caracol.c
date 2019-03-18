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
    int discovery, low, parent, numOfChilds;
    bool visited;
    node_t head, tail; //List of adjencies
} * router_t;

//VAR GLOBAIS
int time = 0, num_subnetworks = 0, subnetworkId = 0;
node_t subnetworksIds = NULL;
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
void DFS(int i, router_t router[], bool articPointsVerifier[])
{

    router[i]->visited = true;
    if (i + 1 > subnetworkId)
        subnetworkId = i + 1;
    router[i]->discovery = router[i]->low = time++;

    for (node_t x = router[i]->tail; x != NULL; x = x->next)
    {
        //printf("%d\n",x->num-1);
        if (router[x->num - 1]->visited == false)
        {

            router[i]->numOfChilds++;
            router[x->num - 1]->parent = i;
            DFS(x->num - 1, router, articPointsVerifier);

            //if (router[i]->low > router[x->num - 1]->low)
            //router[i]->low = router[x->num - 1]->low;

            if (router[i]->parent == -1 && router[i]->numOfChilds > 1)
            {
                articPointsCnt++;
                articPointsVerifier[i] = -1;
            }
            //printf("ROUTER %d HAS %d CHILDS\n",i+1,router[i]->numOfChilds);
            //printf("2-ROUTER %d\nChild Low:%d\nParent Disc:%d\n\n",i+1,router[x->num - 1]->low,router[i]->discovery);
            else if (router[i]->parent != -1 && router[x->num - 1]->low >= router[i]->discovery)
            {
                //printf("%d tIME:%d\n", i + 1, time);
                articPointsCnt++;
                articPointsVerifier[i] = -1;
            }
        }
        else if (x->num - 1 != router[i]->parent)
            if (router[i]->low > router[x->num - 1]->discovery)
            {
                //printf("HELOOOO\n");
                router[i]->low = router[x->num - 1]->discovery;
            }
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
    bool articPointsVerifier[num_routers];
    int teste[num_routers][num_routers];

    for (int i = 0; i < num_routers; i++)
    {
        router[i] = (router_t)malloc(sizeof(struct router));
        router[i]->head = NULL;
        router[i]->tail = NULL;
        router[i]->numOfChilds = 0;
        router[i]->visited = false;
        router[i]->parent = -1;
        articPointsVerifier[i] = false;
        //teste[99] = (node_t)malloc(sizeof(struct node));
    }

    for (int i = 0; i < num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);
        addtoList(&(router[num1 - 1])->head, &(router[num1 - 1])->tail, num2);
        addtoList(&(router[num2 - 1])->head, &(router[num2 - 1])->tail, num1);
    }
    //----

    // IMPRIME LISTA DE ADJACENCIAS
    /*for (int i = 0; i < num_routers; i++)
    {
        printf("%d", i + 1);
        for (node_t j = router[i]->head; j != NULL; j = j->next)
        {
            printf("->%d", j->num);
        }
        printf("\n");
    }
    printf("\n");*/

    for (int i = 0; i < num_routers; i++)
    {
        if (router[i]->visited == false)
        {
            subnetworkId = 0;
            num_subnetworks++;
            DFS(i, router, articPointsVerifier);
            addtoListSORTED(&subnetworksIds, subnetworkId);
        }
    }


    //for (int i = 0; i < num_routers; i++)
        //printf("1-ROUTER %d\nDISC:%d  LOW:%d PARENT:%d\n\n",i+1,router[i]->discovery,router[i]->low,router[i]->parent);
       // printf("1-ROUTER %d Childs:%d\n",i+1,router[i]->numOfChilds);
    /*for (node_t x = articPoints; x != NULL; x = x->next)
    {
        printf("%d ", x->num);
    }*/
    printf("%d\n", num_subnetworks);
    for (node_t x = subnetworksIds; x != NULL; x = x->next)
    {
        printf("%d ", x->num);
    }
    printf("\n");
    printf("%d\n", articPointsCnt);
    return 0;

    return 0;
}