#include <stdio.h>
#include <stdlib.h>

// Data Structure to store a linked list node
struct Node
{
	int data;
	struct Node* next;
};

// Helper function to print given linked list
void printList(struct Node* head)
{
	struct Node* ptr = head;
	while (ptr)
	{
		printf("%d -> ", ptr->data);
		ptr = ptr->next;
	}

	printf("null");
}

// Helper function to insert new Node in the beginning of the linked list
void push(struct Node** head, int data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = *head;
	*head = newNode;
}

// Helper function to return a new node of the linked list
struct Node* newNode(int data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

// Function to insert the given node into the correct sorted position in
// the given list sorted in increasing order
void SortedInsert(struct Node** head, struct Node* newNode)
{
	// Special case for the head end
	if (*head == NULL || (*head)->data >= newNode->data)
	{
		newNode->next = *head;
		*head = newNode;
		return;
	}

	// Locate the node before the point of insertion
	struct Node* current = *head;
	while(current->next != NULL && current->next->data < newNode->data)
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;
}

// main method
int main(void)
{
	// input keys
	int keys[] = {2, 4, 6, 8};
	int n = sizeof(keys)/sizeof(keys[0]);

	// points to the head node of the linked list
	struct Node* head = NULL;

	// construct linked list

	SortedInsert(&head, newNode(5));
	SortedInsert(&head, newNode(9));
	SortedInsert(&head, newNode(1));

	// print linked list
	printList(head);
    printf("\n");

	return 0;
}