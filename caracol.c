#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int num;
    struct node *next;
} * node_t;

typedef struct list
{
    int discovery, low;
    node_t head, tail;
    bool router_visited, all_visited;
} * list_t;

//VAR GLOBAIS
bool subnet_found = true;
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
void addtoSubNetIDList(node_t *head, int number)
{

    if (*head == NULL)
    {
        *head = malloc(sizeof(struct node));
        (*head)->num = number;
        (*head)->next = NULL;
    }
    else
    {
        node_t x = NEW(number);

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

void addtoList(list_t *list, int number)
{
    node_t x = NEW(number);
    x->next = (*list)->head;
    (*list)->head = x;
    (*list)->tail = (*list)->head;
}

void printList(node_t head)
{
    node_t t;
    for (t = head; t != NULL; t = t->next)
        printf("%d\n", t->num);
}
////////////////////////////
////////////////////////////

int main()
{
    int subnetwork_id = 0, num_subnetworks = 0, articulationPoints = 0,
        num_routers, num_visited_routers = 0, num_connections,
        discovery_time = 1;

    //Read Input
    int num1, num2;

    scanf("%d", &num_routers);
    scanf("%d", &num_connections);

    list_t listOfAdj[num_routers];
    node_t subnetworks_ids = NULL;

    for (int i = 0; i < num_routers; i++)
    {
        listOfAdj[i] = malloc(sizeof(struct list));
        listOfAdj[i]->head = malloc(sizeof(struct node));
        listOfAdj[i]->tail = malloc(sizeof(struct node));
        listOfAdj[i]->head = NULL;
        listOfAdj[i]->tail = NULL;
        listOfAdj[i]->all_visited = false;
        listOfAdj[i]->router_visited = false;
        listOfAdj[i]->low = 0;
    }

    for (int i = 0; i < num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);
        addtoList(&listOfAdj[num1 - 1], num2);
        addtoList(&listOfAdj[num2 - 1], num1);
    }
    //-----

    // IMPRIME LISTA DE ADJACENCIAS
    /*for (int i = 0; i < num_routers; i++)
    {
        printf("%d", i + 1);
        for (node_t j = listOfAdj[i]->head; j != NULL; j = j->next)
        {
            printf("->%d", j->num);
        }
        printf("\n");
    }
    printf("\n");*/

    int i = 0;
    while (true)
    {
        // for (int i = 0; i < num_routers; i++)
        //   printf("%d-ALL VISIT %d \n", i + 1, listOfAdj[i]->all_visited);
        //printf("\n");

        if (num_visited_routers >= num_routers)
            break;

        if (listOfAdj[i]->all_visited == true)
        {
            i++;
            continue;
        }

        if (listOfAdj[i]->router_visited == false)
        {
            if (i + 1 > subnetwork_id)
                subnetwork_id = i + 1;
            listOfAdj[i]->router_visited = true;
            listOfAdj[i]->discovery = discovery_time++;

            //printf("%d\n",num_visited_routers);
        }
        for (node_t x = listOfAdj[i]->tail; x != NULL; x = x->next)
        { //começa no ultimo elemento visitado


            if (x->next == NULL && listOfAdj[x->num - 1]->router_visited == false)
            { //ultimo elemento da lista e nao foi visitado

                //printf("%d- VEM DE %d\n", l++, i + 1);
                listOfAdj[i]->all_visited = true;
                num_visited_routers++;
                i = x->num - 1;
                //printf("VAI PARA %d\n", i + 1);
                //printf("\n");
                break;
            }

            else if (x->next == NULL && listOfAdj[x->num - 1]->router_visited == true)
            { //ultimo elemento da lista e ja foi visitado
                listOfAdj[i]->all_visited = true;
                num_visited_routers++;

                if (listOfAdj[i]->low == 0)
                    listOfAdj[i]->low = listOfAdj[x->num - 1]->discovery;
                else if (listOfAdj[x->num - 1]->discovery < listOfAdj[i]->low)
                    listOfAdj[i]->low = listOfAdj[x->num - 1]->discovery;

                for (node_t y = listOfAdj[i]->head; y != NULL; y = y->next)
                    if (listOfAdj[y->num - 1]->all_visited == false)
                    {
                        //printf("ULTIMO:%d\n", num_visited_routers);
                        //printf("%d- VEM DE %d\n", l++, i + 1);
                        subnet_found = false;
                        i = y->num - 1;
                        // printf("VAI PARA %d\n", i + 1);
                        // printf("\n");
                        break;
                    }

                if (subnet_found)
                {
                    //printf("ENTROU\n");
                    //printf("%d- VEM DE %d\n", l++, i + 1);
                    num_subnetworks++;
                    addtoSubNetIDList(&subnetworks_ids, subnetwork_id);

                    subnetwork_id = 0;
                    //printf("VAI PARA %d\n", i + 1);
                    //printf("\n");
                    //printf("visited routers:%d\n", num_visited_routers);
                    if (num_visited_routers < num_routers)
                    {
                        for (i = 0; i < num_routers; i++)
                        {
                            if (listOfAdj[i]->router_visited == false)
                                break;
                        }
                    }
                }
                subnet_found = true;
                break;
            }

            else if (listOfAdj[x->num - 1]->router_visited == false)
            {
                //printf("%d\n", i + 1);
                listOfAdj[i]->tail = x;
                // printf("%d- VEM DE %d\n", l++, i + 1);
                i = x->num - 1;
                //printf("VAI PARA %d\n", i + 1);
                //printf("\n");
                break;
            }
            //router ja visitado
            if (listOfAdj[i]->low == 0)
                listOfAdj[i]->low = listOfAdj[x->num - 1]->discovery;
            else if (listOfAdj[x->num - 1]->discovery < listOfAdj[i]->low)
                listOfAdj[i]->low = listOfAdj[x->num - 1]->discovery;
        }
    }

    
    /*printf("Discovery\n");
    for (int i = 0; i < num_routers; i++)
    {
        printf("%d ", listOfAdj[i]->discovery);
    }
    printf("\n");
    printf("LOW\n");
    for (int i = 0; i < num_routers; i++)
    {
        printf("%d ", listOfAdj[i]->low);
    }
    printf("\n");*/
    printf("%d\n", num_subnetworks);
    for (node_t x = subnetworks_ids; x != NULL; x = x->next)
    {
        printf("%d ", x->num);
    }
    printf("\n");
    printf("%d\n", articulationPoints);
    return 0;
}