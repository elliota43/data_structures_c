#include <stdio.h>
#include <stdlib.h>

typedef int item_t;

typedef struct {
  item_t *base;
  int front;
  int rear;
  int size;
} queue_t;

queue_t *create_queue(int size) {
  queue_t *qu;
  qu = (queue_t *)malloc(sizeof(queue_t));
  if (qu == NULL) {
    printf("Could not allocate memory for queue\n");
    return NULL;
  }

  qu->base = (item_t *)malloc(sizeof(item_t) * size);
  if (qu->base == NULL) {
    printf("Could not allocate memory for queue\n");
    free(qu);
    return NULL;
  }

  qu->front = qu->rear = -1;
  qu->size = size;

  return qu;
}

int queue_empty(queue_t *qu) { return qu->rear == -1; }

int enqueue(queue_t *qu, item_t x) {
  int next_rear = (qu->rear + 1) % qu->size;
  if (next_rear == qu->front) {
    printf("Queue is full\n");
    return -1;
  }

  if (qu->front == -1)
    qu->front = 0;

  qu->rear = next_rear;
  qu->base[qu->rear] = x;
  return 0;
}

item_t dequeue(queue_t *qu) {
  if (qu->front == -1) {
    printf("Queue is empty\n");
    return -1;
  }

  item_t x = qu->base[qu->front];

  if (qu->front == qu->rear) {
    qu->front = -1;
    qu->rear = -1;
  } else {
    qu->front = (qu->front + 1) % qu->size;
  }

  return x;
}

void destroy_queue(queue_t *qu) {
  if (qu != NULL) {
    free(qu->base);
    free(qu);
  }
}
