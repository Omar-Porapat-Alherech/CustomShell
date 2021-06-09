// Porapat ArchLinux Kernel 5.5.6-arch1-1 @ 03/22/2020
#include <stdio.h>
#include <stdlib.h>  //has functions malloc() and free()
#include <string.h>

struct Node {
  int data;
  struct Node* next;
  struct Node* prev;
};

void insert_head(struct Node** head, int data) {
  // New Node Memory Allocation + Assignment
  struct Node* new_node = malloc(sizeof(struct Node));
  new_node->data = data;
  // Attaching Nodes
  new_node->next = *head;
  new_node->prev = NULL;
  // If head is null (First Element)
  if ((*head) != NULL) {
    (*head)->prev = new_node;
  }
  // Assign new head;
  *head = new_node;
}
// This Code Assumes there will only be one instance of what has to be removed
void remove_value(struct Node** head, int data) {
  // Pointers
  struct Node* current = *head;
  // If List is Empty
  if (current == NULL) {
    printf("Bruh the List is Empty, What am I supposed to Delete?");
    return;
  }
  while (current != NULL) {
    //// Made it Here
    if (current->data == data) {
      printf("Found Current->data = %d, data = %d\n", current->data, data);
      // Found Case 1: When the node to be removed is the head
      if (current->prev == NULL) {
        printf("Current = *HEAD = TRUE\n");
        current->next->prev = NULL;
        *head = current->next;
        free(current);
        return;
        // Found Case 2: When node to be removed is Tail
      } else if (current->next == NULL) {
        printf("Current = *Tail = TRUE\n");
        current->prev->next = NULL;
        free(current);
        return;
        // When Node to be removed is in the middle
      } else {
        printf("Current = *TAIL FALSE\n");
        printf("Current = *HEAD FALSE\n");
        current->prev->next = current->next;
        current->next->prev = current->prev;
        free(current);
        return;
      }
    } else {
      current = current->next;
    }
  }
}

void insert_tail(struct Node** head, int data) {
  // Allocation
  struct Node* new_node = malloc(sizeof(struct Node));
  struct Node* end = *head;
  // Assignment
  new_node->data = data;
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
