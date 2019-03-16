#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node
{
    int num;
    struct node *next;
} * node_t;

/*typedef struct list
{
    node_t head, tail;
} * list_t;*/

typedef struct connection
{
    int nums[2];
    bool visited;
} connection_t;

//VAR GLOBAIS

////////////////////////////

//LIST//

node_t NEW(int number)
{
    node_t x = (node_t) malloc(sizeof(struct node));
    x->num = number;
    x->next = NULL;
    return x;
}

/*node_t insertEnd(node_t tail, int num1)
{
    node_t x = NEW();
    tail->next = x;
    x->next = NULL;
    x->num = num1;
    return x;
}*/

/*node_t lookup(node_t head, int num)
{
    node_t t;
    for (t = head; t != NULL; t = t->next)
        if (t->num == num)
            return t;
    return NULL;
}*/

void printList(node_t head)
{
    node_t t;
    for (t = head; t != NULL; t = t->next)
        printf("%d\n", t->num);
}

////////////////////////////
////////////////////////////

void pushStack(node_t *stack,int number)
{
        node_t x = NEW(number);
        x->next=*stack;
        *stack = x;
}


void popStack(node_t *stack)
{
    if (*stack == NULL){
        printf("Error on pop, Stack empty, exiting...\n");
        exit(EXIT_FAILURE);
    }
    node_t x = *stack;
    *stack = (*stack)->next;
    free(x);
}

int main()
{
    int subnetwork_id = 0, num_subnetworks = 1, num_crossRouters = 0,
        num_routers, num_connections, max_router_subnet = 0, tmp_router_subnet = 0;

    node_t stack = NULL, subnetworks_ids = NULL;

    //Read Input
    scanf("%d", &num_routers);
    scanf("%d", &num_connections);

    int visit[num_routers], graph[num_routers];
    connection_t routerConnections[num_connections];

    for (int i = 0; i < num_connections; i++)
        scanf("%d %d", &(routerConnections[i]).nums[0], &(routerConnections[i]).nums[1]);
    //-----

    for (int i = 0; i < num_routers; i++) //preenche array VISIT
    {
        visit[i] = i + 1;
    }

    subnetworks_ids = malloc(sizeof(struct node));
    //pushStack(&stack,1);
    //popStack(&stack);
 
    return 0;

    for (int i = 0; graph[num_routers - 1] == 0; i++)
    {

        if (visit[i] != i + 1)
            continue;
        visit[i] = -1;
        graph[i] = i + 1;
        pushStack(&stack,i+1);
    }


    /*for (int i = 0; i < num_routers; i++)
    {
        
    }

    printList(stack->head);*/

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