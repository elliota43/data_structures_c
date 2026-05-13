#include <stdio.h>
#include <stdlib.h>

typedef int item_t;

typedef struct {
  item_t *base;
  item_t *top;
  int size;
} MyStack;

MyStack *create_stack(int size) {
  MyStack *s = (MyStack *)malloc(sizeof(MyStack));
  if (s == NULL)
    return NULL;

  s->base = (item_t *)malloc(sizeof(item_t) * size);
  if (s->base == NULL) {
    free(s);
    return NULL;
  }

  s->top = s->base;
  s->size = size;

  return s;
}

void destroy_stack(MyStack *s) {
  if (s != NULL) {
    free(s->base);
    free(s);
  }
}

void push(MyStack *s, item_t item) {
  if (s->top == s->base + s->size) {
    fprintf(stderr, "stack overflow. Cannot push %d\n", item);
    return;
  }

  *(s->top) = item;
  s->top++;
}

int is_empty(MyStack *s) { return s->top == s->base; }

item_t pop(MyStack *s) {
  s->top--;
  return *(s->top);
}

item_t peek(MyStack *s) { return *(s->top - 1); }

int main(void) {
  printf("Creating a stack of size 3...\n");
  MyStack *my_stack = create_stack(3);

  printf("Pushing 10, 20, 30...\n");

  push(my_stack, 10);
  push(my_stack, 20);
  push(my_stack, 30);

  printf("Attempting to push 40 onto the stack...\n");
  push(my_stack, 40);

  printf("\nPopping items until empty:\n");

  while (!is_empty(my_stack)) {
    item_t item = pop(my_stack);
    printf("Popped: %d\n", item);
  }

  destroy_stack(my_stack);
  return 0;
}
