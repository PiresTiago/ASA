#include <stdio.h>
#include <stdlib.h>

//VAR GLOBAIS

int Routers_Vector_SIZE;

////////////////////////////
////////////////////////////

//LIST//

typedef struct connection
{
    int nums[2];
    struct connection *next;
} * node;

node NEW()
{
    node x = (node)malloc(sizeof(struct connection));
    x->next = NULL;
    return x;
}

node insertBegin(node head, int num1, int num2)
{

    node x = NEW();
    x->next = head;
    x->nums[0] = num1;
    x->nums[1] = num2;
    return x;
}

node lookup(node head, int num)
{
    node t;
    for (t = head; t != NULL; t = t->next)
        if (t->nums[0] == num)
            return t;
    return NULL;
}

void printList(node head)
{
    node t;
    for (t = head; t != NULL; t = t->next)
        printf("%d %d\n", t->nums[0], t->nums[1]);
}

////////////////////////////
////////////////////////////

node readInput(node head, int *num_routers, int *num_connections)
{
    int num1, num2;
    scanf("%d %d", num_routers, num_connections);
    for (int i = 0; i < *num_connections; i++)
    {
        scanf("%d %d", &num1, &num2);

        if (head == NULL)
        {
            head = NEW();
            head->nums[0] = num1;
            head->nums[1] = num2;
        }
        else
            head = insertBegin(head, num1, num2);
    }
    return head;
}

void searchSubNetwork(int num_routers)
{
    
}

int main()
{
    int size = 50, *vector, *Routers_Vector;
    int num_routers, num_connections;
    node head = NULL;
    head=readInput(head, &num_routers, &num_connections);
    printList(head);
    Routers_Vector_SIZE = num_routers;
    Routers_Vector = malloc(sizeof(int) * num_routers);

    for (int i = 1; i <= num_routers; i++)
        Routers_Vector[i - 1] = i;

    int router_num = 1;
    while (router_num <= num_routers)

        return 0;
}

/*

void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readInput(node head, int *num_routers, int *num_connections)
{
    char space, newline;
    int cnt = 0, num1, num2;

    scanf("%d", num_routers);
    scanf("%d", num_connections);

    if (*num_routers < 2 || *num_connections < 1)
    {
        printf("Input error\n");
        exit(EXIT_FAILURE);
    }
    clearBuffer();
    for (int i = 0; i < *num_connections; i++)
    {

        num1 = getchar();
        space = getchar();
        num2 = getchar();
        newline = getchar();

        num1 -= 48;
        num2 -= 48;

        if (num1 < 0 || num1 > 9 || num1 > *num_routers ||
            num2 < 0 || num2 > 9 || num2 > *num_routers ||
            space != 32 || newline != 10)
        {
            printf("Input error, try again.\n");
            i--;
            continue;
        }
        if (head == NULL)
        {
            head = NEW();
            head->nums[0] = num1;
            head->nums[1] = num2;
        }
        else
            head = insertBegin(head, num1, num2);

        //printf("\n");
        //printList(head);
        //printf("\n");
    }
}*/