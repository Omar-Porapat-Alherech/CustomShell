// Porapat ArchLinux Kernel 5.5.6-arch1-1 @ 03/22/2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char* data;
  struct Node* next;
  struct Node* prev;
};
void insert_head(struct Node** head, char* data) {
  //// New Node Memory Allocation + Assignment
  // Data field wasn't initialized, so I need to allocate the memory adding 1
  // for the null terminator '/0'
  int num_ele = (strlen(data)+1);
  //Dynamic Memory allocation
  struct Node* new_node = malloc(sizeof(*new_node) + num_ele * sizeof(*new_node->data));
  // Literally took me a whole day and half a textbook to realize this
  new_node->data = malloc(num_ele);
  strcpy(new_node->data, data);
  //// Attaching Nodes
  new_node->next = *head;
  new_node->prev = NULL;
  //// If head is null (First Element)
  if ((*head) != NULL) {
    (*head)->prev = new_node;
  }
  //// Assign new head;
  *head = new_node;
}
//// This Code Assumes there will only be one instance of what has to be removed
void remove_value(struct Node** head, char* data) {
  //// Pointers
  struct Node* current = *head;
  //// If List is Empty
  if (current == NULL) {
    printf("Bruh the List is Empty, What am I supposed to Delete?");
    return;
  }
  while (current != NULL) {
    if (strcmp(current->data, data) == 0) {
      // printf("Found Current->data = %d, data = %d\n", current->data, data);
      //// Found Case 1: When the node to be removed is the head
      if (current->prev == NULL) {
        //printf("Current = *HEAD = TRUE\n");
        current->next->prev = NULL;
        *head = current->next;
        free(current);
        return;
        //// Found Case 2: When node to be removed is Tail
      } else if (current->next == NULL) {
        //printf("Current = *Tail = TRUE\n");
        current->prev->next = NULL;
        free(current);
        return;
        // When Node to be removed is in the middle
      } else {
        //printf("Current = *TAIL FALSE\n");
        //printf("Current = *HEAD FALSE\n");
        current->prev->next = current->next;
        current->next->prev = current->prev;
        free(current);
        free(current->data);
        return;
      }
    } else {
      current = current->next;
    }
  }
}

void insert_tail(struct Node** head, char* data) {
  // Allocation
    // Plus 1 for Null Terminator
  int num_ele = (strlen(data)+1);
  struct Node* new_node = malloc(sizeof(*new_node) + num_ele * sizeof(*new_node->data));
  struct Node* end = *head;
  // Assignment
  new_node->data = malloc(num_ele);
  strcpy(new_node->data, data);
  // End of Linked List next is null
  new_node->next = NULL;
  // The case where the LinkedList is Empty
  if (*head == NULL) {
    new_node->prev = NULL;
    *head = new_node;
    // Make subsequent commands work not requiring at if/else
    return;
  }
  while (end->next != NULL) {
    end = end->next;
  }
  end->next = new_node;
  new_node->prev = end;
  return;
}




//void execute(char **commands, struct Node *head){
//    pid_t curr_process = fork();
//    int status;
//    int i;
//    struct Node * params;
//    char * base[64];
//    fflush(stdout);
//    int stat;
//    if (curr_process != 0){
//        /* Parent Code */
//        /* Wait for Child to Exit */
////        params = print_output(commands, head);
//        waitpid(curr_process, &status, 0);
//    }
//    else {
////        execvp(*commands++, &params->data);
//        params = print_output(commands, head);
//        while(params != NULL){
//            i=0;
//            base[i] = strtok(params->data, " ");
//            while (base[i] != NULL){
//                base[++i] = strtok(NULL," ");
//            }
//            execvp(*commands++, base);
//            params = params->next;
//        }
//    }
//}