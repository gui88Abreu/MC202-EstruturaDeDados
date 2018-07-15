#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List{
    char card[4];
    char colorCard;
    char suitCard;
    char valueCard;
    char state;
    struct List *next;
}List;

List *beginList();
int checkEmptyList(List *);
void insertHead(List *, char *, char, char);
void insertTail(List *, char *, char, char);
void eraseItem(List *, char *);
void eraseList(List *);
void printList(List *);

List *beginList(){
    /*It begins a list putting a head*/
    List *head = NULL;
    head = (List*)malloc(sizeof(List));
    head->next = NULL;
    return head;
}

int checkEmptyList(List *head){
    /*It checks if the list given is empty. It returns 1 if true and 0 otherwise.*/
    return (head == NULL);
}

/*
List* seek(List *current, int value){
    List *p = NULL;

    while(current->next != NULL && !p){
        current = current->next;
        if(current->value == value)
            p = current;
    }

    return p;
}
*/

void insertHead(List *head, char *card, char valueCard, char state){
    /*It inserts an item in the beginning of the list given*/
    List *new;
    
    if (head!=NULL){
        new = (List*)malloc(sizeof(List));
        strcpy(new->card,card);
        new->valueCard = valueCard;
        new->state = state;
        new->next = head->next;
        head->next = new;
    }

    return;
}

void insertTail(List *head, char *card, char valueCard, char state){
    //It inserts an item in the end of the list given
    
    List *helper = head;
    List *new;

    if(helper != NULL){
        while(helper->next != NULL){
            helper = helper->next;
        }

        new = (List*)malloc(sizeof(List));
        strcpy(new->card,card);
        new->colorCard = getColor(card);
        new->suitCard = getSuit(card);
        new->valueCard = valueCard;
        new->state = state;
        helper->next = new;
        new->next = NULL;
    }

    return;
}

/*
void insertMidle(List *head, int newValue, int reference){
    //It inserts an item in any position of the list given
    
    List *node = seek(head, reference);
    List *new;
    
    if (node!=NULL){
        new = (List*)malloc(sizeof(List));
        new->value = newValue;
        new->next = node->next;
        node->next = new;
    }

    return;
}
*/

void eraseItem(List *current, char *card){
    /*it erases a specific item of the list given*/
    
    List *helper = NULL;
    char found = 0;

    while(current->next != NULL && !found){
        helper = current;
        current = current->next;
        if(strcmp(current->card,card)==0)
            found = 1;
    }

    if(found){
        helper->next = current->next;
        free(current);
    }


    /*If the item given don't exist in the list the function does nothing*/
    return;
}

void eraseList(List *head){
    /*It erases all items of the list given*/
    if (head != NULL){
        while(head->next != NULL){
            eraseItem(head,head->next->card);
        }
        
        free(head);
    }
    return;
}

void printList(List *head){
    /*It prints all items of the list given*/
    if(head!=NULL){
        while(head->next != NULL){
            printf("%3s %2d %d\n",head->next->card, head->next->valueCard, head->next->state);
            head = head->next;
        }
    }
    return;
}