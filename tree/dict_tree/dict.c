#include "dict.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  const char *key;
  DictValue *value;
  struct Node *left;
  struct Node *right;
  int height;
} Node;

typedef struct Dict {
  Node *root;
  size_t size;
} Dict;

/* --- Helpers --- */

// Recursively (post-order) traverse the tree and free the nodes,
// as well as the dict values.
static void node_destroy_recursive(Node *node) {
  if (node == NULL)
    return;

  node_destroy_recursive(node->left);
  node_destroy_recursive(node->right);

  free((void *)node->key);

  dict_value_free(node->value);

  free(node);
}

/* --- DictValue --- */

DictValue *create_string_value(const char *input) {
  if (!input) {
    errno = EINVAL;
    return NULL;
  }

  DictValue *value = malloc(sizeof(DictValue));
  if (!value)
    return NULL;
  value->type = DICT_VAL_STRING;
  value->as.str = strdup(input);

  if (!value->as.str) {
    free(value);
    return NULL;
  }

  return value;
}

DictValue *create_buffer_value(void *raw_data, size_t length) {

  if (!raw_data || length == 0) {
    errno = EINVAL;
    return NULL;
  }

  DictValue *value = malloc(sizeof(DictValue));
  if (!value) {
    return NULL;
  }
  value->type = DICT_VAL_BUFFER;
  value->as.buffer.size = length;

  value->as.buffer.data = malloc(length);
  if (!value->as.buffer.data) {
    free(value);
    errno = ENOMEM;
    return NULL;
  }

  memcpy(value->as.buffer.data, raw_data, length);

  return value;
}

DictValue *create_ptr_value(void *input) {
  if (!input) {
    errno = EINVAL;
    return NULL;
  }

  DictValue *value = malloc(sizeof(DictValue));
  if (!value)
    return NULL;

  value->type = DICT_VAL_CUSTOM_PTR;
  value->as.ptr = input;
  return value;
}

DictValue *create_int_value(int64_t input) {
  DictValue *value = malloc(sizeof(DictValue));
  if (!value)
    return NULL;
  value->type = DICT_VAL_INT;
  value->as.i = input;
  return value;
}

DictValue *create_float_value(double input) {
  DictValue *value = malloc(sizeof(DictValue));
  if (!value)
    return NULL;
  value->type = DICT_VAL_FLOAT;
  value->as.f = input;
  return value;
}

void dict_value_free(DictValue *value) {
  if (!value) {
    errno = EINVAL;
    return;
  }

  if (value->type == DICT_VAL_STRING) {
    free(value->as.str);
  } else if (value->type == DICT_VAL_BUFFER) {
    free(value->as.buffer.data);
  }

  free(value);
}

/* --- Dict --- */

Dict *dict_new(void) {
  Dict *dict = malloc(sizeof(Dict));
  if (!dict)
    return NULL;

  dict->root = NULL;
  dict->size = 0;

  return dict;
}

void dict_free(Dict *dict) {
  if (dict == NULL) {
    return;
  }

  node_destroy_recursive(dict->root);

  free(dict);
}

void dict_clear(Dict *dict) {
  if (dict == NULL || dict->root == NULL)
    return;

  node_destroy_recursive(dict->root);

  dict->root = NULL;
  dict->size = 0;
}

/* --- Tree Helpers --- */

static int get_height(Node *n) { return n ? n->height : 0; }

static int max(int a, int b) { return (a > b) ? a : b; }

static void update_height(Node *n) {
  if (n) {
    n->height = 1 + max(get_height(n->left), get_height(n->right));
  }
}

static Node *node_find_min(Node *node) {
  if (node == NULL)
    return NULL;

  while (node->left != NULL) {
    node = node->left;
  }

  return node;
}
