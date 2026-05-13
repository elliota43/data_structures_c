#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int item_t;

typedef struct {
  item_t *base;
  int size;
  int max_size;
  item_t *copy;
  int copy_size;
} my_stack_t;

my_stack_t *stack_create(int initial_size) {
  if (initial_size <= 0)
    return NULL;

  my_stack_t *st = (my_stack_t *)malloc(sizeof(my_stack_t));
  if (!st) {
    fprintf(stderr, "stack allocation failed\n");
    return NULL;
  }

  st->base = (item_t *)malloc(initial_size * sizeof(item_t));
  if (!st->base) {
    fprintf(stderr, "stack allocation failed: base array allocation\n");
    free(st);
    return NULL;
  }

  st->max_size = initial_size;
  st->size = 0;
  st->copy = NULL;
  st->copy_size = 0;

  return st;
}

bool stack_is_empty(my_stack_t *st) { return (st == NULL || st->size == 0); }

void stack_push(my_stack_t *st, item_t item) {
  if (!st)
    return;

  st->base[st->size] = item;
  st->size++;

  if (st->copy != NULL || st->size >= 0.75 * st->max_size) {
    int additional_copies = 4;

    if (st->copy == NULL) {
      st->copy = (item_t *)malloc(st->max_size * sizeof(item_t) * 2);
      if (!st->copy) {
        fprintf(stderr, "Error: shadow stack allocation failed\n");
        exit(EXIT_FAILURE);
      }
    }

    while (additional_copies > 0 && st->copy_size + st->copy_size < st->size) {
      st->copy[st->copy_size] = st->base[st->copy_size];
      st->copy_size++;
      additional_copies--;
    }

    if (st->copy_size == st->size) {
      free(st->base);
      st->base = st->copy;
      st->max_size *= 2;
      st->copy = NULL;
      st->copy_size = 0;
    }
  }
}

item_t stack_pop(my_stack_t *st) {
  if (stack_is_empty(st)) {
    fprintf(stderr, "Error: stack empty\n");
    exit(EXIT_FAILURE);
  }

  st->size--;
  item_t popped_item = st->base[st->size];

  // if we have a copy and popping brings the size below the copy size,
  // the copy is complete so we can just use it instead of the base array.
  if (st->copy != NULL && st->copy_size >= st->size) {
    free(st->base);
    st->base = st->copy;
    st->max_size *= 2;
    st->copy = NULL;
    st->copy_size = 0;
  }
  return popped_item;
}

item_t stack_top(my_stack_t *st) {
  if (stack_is_empty(st)) {
    fprintf(stderr, "error: stack empty\n");
    exit(EXIT_FAILURE);
  }

  return st->base[st->size - 1];
}

void stack_destroy(my_stack_t *st) {
  if (!st)
    return;

  if (st->base)
    free(st->base);

  if (st->copy)
    free(st->copy);

  free(st);
}
