#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { 
  struct list_node *node = malloc(sizeof(struct list_node));
    if (node == NULL) {
      return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
    if (node == NULL) {
      return;
    }
    node->next = list->head;
    list->head = node;
    if (list->tail == NULL) {
      list->tail = node;
    }
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
    if (node == NULL) {
      return;
    }
    if (list->tail == NULL) {
      list->head = node;
      list->tail = node;
    } else {
      list->tail->next = node;
      list->tail = node;
    }
}

size_t remove_from_head(struct linked_list *list) { 
  if (list->head == NULL) {
      return 0; // or some error value
    }
    struct list_node *node = list->head;
    size_t value = node->value;
    list->head = node->next;
    if (list->head == NULL) {
      list->tail = NULL;
    }
    free(node);
    return value;
}

size_t remove_from_tail(struct linked_list *list) { 
  if (list->tail == NULL) {
    return 0; // or some error value
  }
    
  // If there's only one node
  if (list->head == list->tail) {
    size_t value = list->head->value;
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    return value;
  }
    
  // Find the node before tail
  struct list_node *prev = list->head;
  while (prev->next != list->tail) {
    prev = prev->next;
  }
    
  size_t value = list->tail->value;
  free(list->tail);
  list->tail = prev;
  list->tail->next = NULL;
  return value;
}

void free_list(struct linked_list list) {
  struct list_node *current = list.head;
    while (current != NULL) {
      struct list_node *next = current->next;
      free(current);
      current = next;
    }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
