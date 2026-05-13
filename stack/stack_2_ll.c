#include <stdio.h>
#include <stdlib.h>

typedef int item_t;

typedef struct Node {
  item_t item;
  struct Node *next;
} Node;

typedef struct {
  Node *top;
  int size;
} MyStack;

MyStack *create_stack() {
  MyStack *s = (MyStack *)malloc(sizeof(MyStack));
  if (s == NULL) {
    printf("Could not allocate memory for stack\n");
    return NULL;
  }

  s->top = NULL;
  s->size = 0;

  return s;
}

void destroy_stack(MyStack *s) {
  if (s != NULL) {
    Node *curr = s->top;
    while (curr != NULL) {
      Node *next = curr->next;
      free(curr);
      curr = next;
    }
  }

  free(s);
}

void push(MyStack *s, item_t item) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (new_node == NULL) {
    printf("Could not allocate memory for new node\n");
    return;
  }

  new_node->item = item;
  new_node->next = s->top;

  s->top = new_node;
  s->size++;
}

int stack_empty(MyStack *s) { return s->top == NULL; }

item_t top(MyStack *s) { return s->top->item; }

item_t pop(MyStack *s) {
  Node *temp = s->top;
  item_t popped_value = temp->item;
  s->top = s->top->next;
  s->size--;

  free(temp);

  return popped_value;
}
