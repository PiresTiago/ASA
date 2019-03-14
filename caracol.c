#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//VAR GLOBAIS

int Routers_Vector_SIZE;
int stack_index = 0, subnet_index = 0, grafo_index = 0;
int cnt_finish = 0;
bool firstTime = true;
bool FLAG_CRUZAMENTO = false;
////////////////////////////
////////////////////////////

//LIST//
typedef struct node
{
    int num;
    struct node *next;
} * node;

typedef struct list
{
    node head, tail;
} * ListPos;

typedef struct connection
{
    int nums[2];
    bool visited;
    struct connection *next;
} * connection;


typedef struct listconnection
{
    connection head, tail;
} *ConnectionPos;

node NEW()
{

    node x = (node)malloc(sizeof(struct node));
    x->next = NULL;
    return x;
}

connection NewConnection(ConnectionPos pos)
{
    connection c = (connection)malloc(sizeof(struct connection));
    c->next = NULL;
    c->nums[0] = 0;
    c->nums[1] = 0;
    
    if (pos->head == NULL)
    {
        pos->head = c;
        pos->tail = c;
    }
    
    return c;
}

node insertEnd(node tail, int num1)
{
    node x = NEW();
    tail->next = x;
    x->next = NULL;
    x->num = num1;
    return x;
}

node lookup(node head, int num)
{
    node t;
    for (t = head; t != NULL; t = t->next)
        if (t->num == num)
            return t;
    return NULL;
}

void printList(node head)
{
    node t;
    for (t = head; t != NULL; t = t->next)
        printf("%d\n", t->num);
}

////////////////////////////
////////////////////////////

void readInput(connection routerConnections, ConnectionPos pos, int *num_routers, int *num_connections)
{
    scanf("%d", num_routers);
    scanf("%d", num_connections);
    routerConnections = (connection)malloc(sizeof(struct connection) * (*num_connections));
    
    int num1, num2;

    for (int i = 0; i < *num_connections; i++)
    {
        fscanf(stdin, "%d %d", &num1, &num2);
        connection c = NewConnection(pos);
        c->nums[0] = num1;
        c->nums[1] = num2;
        pos->tail->next = c;
        pos->tail = c;
    }
        
}

void searchSubNetwork(int num_routers)
{
}

int main()
{
    int *graph, subnetwork_id = 0;

    connection routerConnections;

    int num_subnetworks = 1, num_crossRouters = 0;
    int num_routers, num_connections, max_router_subnet = 0, tmp_router_subnet = 0;;
    node stack = NULL, subnetworks_ids = NULL;
    ConnectionPos connectionsPos = (ConnectionPos)malloc(sizeof(struct list));
    ListPos stackPos = (ListPos)malloc(sizeof(struct list));
    ListPos subnetworks_idsPos = (ListPos)malloc(sizeof(struct list));
    
    readInput(routerConnections, connectionsPos, &num_routers, &num_connections);
    int visit[num_routers];
    

    stack = NEW();
    stackPos->head = stack;
    stackPos->tail = stack;

    subnetworks_ids = NEW();
    subnetworks_idsPos->head = subnetworks_ids;
    subnetworks_idsPos->tail = subnetworks_ids;

    for (int i = 0; i < num_routers; i++)
    {
        node teste = NEW();
        stackPos->tail->next = teste;
        stackPos->tail = teste;
        teste->num = i;
    }

    printList(stackPos->head);

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