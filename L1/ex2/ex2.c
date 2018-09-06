/*************************************
* Lab 1 Exercise 2
* Name: Aadit Kamat 
* Matric No: A0164761B
* Lab Group:02
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);

void printList(node*);
int lengthList(node*);
void destroyList(node*);



int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copies, length;

    while (scanf("%i %i %i", &position, &input, &copies) == 3) {
        length = lengthList(myList);
        if (position > length) {
            position = length;
        }
        myList = insertAt(myList, position, copies, input);
        length = lengthList(myList);

    }

    printf("My List: \n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;

    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

int lengthList(node * head) {
    node* ptr = head;
    int ctr = 0;
    while (ptr != NULL) {
        ptr = ptr -> next;
        ctr++;
    }
    return ctr;
}
node* createNewNode(node* nextNode, int newValue) {
    node* ptr = malloc(sizeof(node));
    ptr -> data = newValue;
    ptr -> next = nextNode;
    return ptr;
}

node* insertBeforeHead(node* head, int newValue) {
    return createNewNode(head, newValue);
}

node* insertInBetween(node* before, node* after, int newValue) {
    before -> next = createNewNode(after, newValue);
    return before;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    if (head == NULL) {
        head = createNewNode(NULL, newValue);
        for(int i = 1; i < copies; i++) {
           head = insertBeforeHead(head, newValue);
        }
    }
    else if (position == 0) {
        for (int i = 0; i < copies; i++) {
            head = insertBeforeHead(head, newValue);
        }
    }
    else {
        node* temp = head;
        for (int i = 1; i < position; i++) {
            temp = temp -> next;
        }
        for (int j = 0; j < copies; j++) {
            temp = insertInBetween(temp, temp ->next, newValue);
        }
    }
    return head;
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;
    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
     while (head != NULL) {
        node* next = head -> next;
        free(head);
        head = next;
    }
}
