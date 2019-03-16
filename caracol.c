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

void addtoSubNetList(node_t *head, int number)
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
        x->next = *head;
        *head = x;
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
    int subnetwork_id = 0, num_subnetworks = 0, num_crossRouters = 0,
        num_routers, num_visited_routers = 0, num_connections,
        max_router_subnet = 0, tmp_router_subnet = 0;

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
        listOfAdj[i]->head->next = NULL;
        listOfAdj[i]->tail->next = NULL;
        listOfAdj[i]->all_visited = false;
        listOfAdj[i]->router_visited = false;
    }

    for (int i = 0; i < num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);
        addtoList(&listOfAdj[num1 - 1], num2);
        addtoList(&listOfAdj[num2 - 1], num1);
    }
    //-----

    // IMPRIME LISTA DE ADJACENCIAS
    for (int i = 0; i < num_routers; i++) 
    {
        printf("%d", i + 1);
        for (node_t j = listOfAdj[i]->head; j->next != NULL; j = j->next)
        {
            printf("->%d", j->num);
        }
        printf("\n");
    }
    printf("\n");

    int i = 0;
    while (true)
    {

        if (num_visited_routers == num_routers)
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
            num_visited_routers++;
            //printf("%d\n",num_visited_routers);
        }
        for (node_t x = listOfAdj[i]->head; x->next != NULL; x = x->next)
        { //começa no ultimo elemento visitado
            //printf("%d\n", i + 1);
            
            if (x->next == NULL && listOfAdj[x->num - 1]->router_visited == false)
            { //ultimo elemento da lista e nao foi visitado
                listOfAdj[i]->all_visited = true;
                i = x->num - 1;
                break;
            }

            else if (x->next == NULL && listOfAdj[x->num - 1]->router_visited == true)
            { //ultimo elemento da lista e ja foi visitado
                listOfAdj[i]->all_visited = true;

                for (node_t y = listOfAdj[i]->head; y->next != NULL; y = y->next)
                    if (listOfAdj[y->num - 1]->all_visited == false)
                    {
                        subnet_found = false;
                        i = y->num - 1;
                        break;
                    }
                if (subnet_found)
                {

                    num_subnetworks++;
                    addtoSubNetList(&subnetworks_ids, subnetwork_id);
                    subnetwork_id = 0;
                }
                subnet_found = true;
                break;
            }

            else if (listOfAdj[x->num - 1]->router_visited == false)
            {
                listOfAdj[i]->tail = x;
                i = x->num - 1;
                break;
            }
        }
    }
    addtoSubNetList(&subnetworks_ids, subnetwork_id);
    printf("%d\n", num_subnetworks);
    for (node_t x = subnetworks_ids; x->next != NULL; x = x->next)
    {
        printf("OLA\n");
        printf("%d", subnetworks_ids->num);
    }
    return 0;
}

/*   stack = malloc(sizeof(int) * num_routers);
    visit = malloc(sizeof(int) * num_routers);
    grafo = malloc(sizeof(int) * num_routers);
    subnetworks_ids = malloc(sizeof(int) * num_routers);

    stack[stack_index] = head->nums[0]; //STACK INIT

    for (int i = 0; i < num_routers; i++)
    {
        visit[i] = i + 1;
        subnetworks_ids[i] = 0;
        grafo[i] = 0;
    }

    while (grafo[grafo_index - 1] < num_routers)
    {
        if (cnt_finish == num_connections)
            break; //Compara nº de visited com nº de conexoes

        for (t = head; t != NULL; t = t->next)
        {

            //printf("STACK: %d \n",stack[stack_index]);
            //printf("VISIT: %d \n",visit[t->nums[0]-1]);
            //printf("GRAFO %d\n",grafo[grafo_index-1]);
            if (t->visited == false &&
                (stack[stack_index] == t->nums[0] || stack[stack_index] == t->nums[1]))
            {
                //printf("ENTROU\n");
                t->visited == true;
                if (visit[t->nums[0] - 1] == t->nums[0])
                {
                    if (FLAG_CRUZAMENTO)
                    {
                        if (max_router_subnet < tmp_router_subnet)
                        {
                            max_router_subnet = tmp_router_subnet-1;
                            tmp_router_subnet = 0;
                        }
                        FLAG_CRUZAMENTO = false;
                        num_crossRouters++;
                    }                           //VERIFICA SE VERTICE É BRANCO
                    visit[t->nums[0] - 1] = -1; //MARCA VERTICE COMO DESCOBERTO
                    grafo[grafo_index++] = t->nums[0];
                    tmp_router_subnet++;
                    if (firstTime)
                    {
                        stack[stack_index] = t->nums[0];
                        firstTime = false;
                    }
                    else
                        stack[++stack_index] = t->nums[0];
                }
                if (visit[t->nums[1] - 1] == t->nums[1])
                {
                    if (FLAG_CRUZAMENTO)
                    {
                        if (max_router_subnet < tmp_router_subnet)
                        {
                            max_router_subnet = tmp_router_subnet-1;
                            tmp_router_subnet = 0;
                        }
                        FLAG_CRUZAMENTO = false;
                        num_crossRouters++;
                    }
                    visit[t->nums[1] - 1] = -1;
                    grafo[grafo_index++] = t->nums[1];
                    tmp_router_subnet++;
                    stack[++stack_index] = t->nums[1];
                }

                if (grafo[grafo_index - 1] > subnetwork_id)
                {
                    subnetwork_id = grafo[grafo_index - 1];

                    //printf("GRAFO %d\n",subnetwork_id);
                }
            }
        }

        if (stack_index == 0)
        {
            num_subnetworks++;
            subnetworks_ids[subnet_index++] = subnetwork_id;
            subnetwork_id = 0;
            FLAG_CRUZAMENTO = true;
            continue;
        }
        else if (grafo[grafo_index - 1] == num_routers)
        {
            subnetworks_ids[subnet_index++] = subnetwork_id;
            //printf("SUB %d\n",subnetworks_ids[0]);
            break;
        }
        stack_index -= 1;
        FLAG_CRUZAMENTO = true;
    }

    printf("%d\n", num_subnetworks);
    for (int x = 0; subnetworks_ids[x] != 0; x++)
    {

        printf("%d", subnetworks_ids[x]);
    }
    printf("\n");
    printf("%d\n", num_crossRouters);
    printf("%d\n", max_router_subnet);
    return 0;
}*/