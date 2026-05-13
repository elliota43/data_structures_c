#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  bool is_leaf;
  int key; // acts as a routing key (internal) OR the actual data key (leaf)

  union {
    struct {
      struct node *left;
      struct node *right;
    } internal;

    struct {
      int payload;
    } leaf;
  } data;
} node_t;

node_t *create_leaf(int key, int payload) {
  node_t *node = (node_t *)malloc(sizeof(node_t));

  if (!node)
    exit(EXIT_FAILURE);

  node->is_leaf = true;
  node->key = key;
  node->data.leaf.payload = payload;
  return node;
}

node_t *create_internal(int routing_key, node_t *left, node_t *right) {
  node_t *node = (node_t *)malloc(sizeof(node_t));
  if (!node)
    exit(EXIT_FAILURE);

  node->is_leaf = false;
  node->key = routing_key;
  node->data.internal.left = left;
  node->data.internal.right = right;
  return node;
}

node_t *search(node_t *root, int query_key) {
  if (root == NULL)
    return NULL;

  node_t *curr = root;

  while (!curr->is_leaf) {
    if (query_key < curr->key) {
      curr = curr->data.internal.left;
    } else {
      curr = curr->data.internal.right;
    }
  }

  if (curr->key == query_key) {
    return curr;
  }

  return NULL;
}

node_t *insert(node_t *root, int key, int payload) {
  if (root == NULL)
    return create_leaf(key, payload);

  // tree only has one node
  if (root->is_leaf) {
    if (root->key == key) {
      root->data.leaf.payload = payload; // update existing leaf
      return root;
    }

    node_t *new_leaf = create_leaf(key, payload);

    // new internal root uses the larger key as the routing key
    if (key < root->key) {
      return create_internal(root->key, new_leaf, root);
    } else {
      return create_internal(key, root, new_leaf);
    }
  }

  // standard traversal to find where the new leaf should be inserted
  node_t *parent = NULL;
  node_t *curr = root;
  bool want_left = false;

  while (!curr->is_leaf) {
    parent = curr;
    if (key < curr->key) {
      curr = curr->data.internal.left;
      want_left = true;
    } else {
      curr = curr->data.internal.right;
      want_left = false;
    }
  }

  // if the leaf is the same key, just update the payload
  if (curr->key == key) {
    curr->data.leaf.payload = payload;
    return root;
  }

  // split the current leaf by creating a new internal router node
  // to hold them both.
  node_t *new_leaf = create_leaf(key, payload);
  node_t *new_internal;

  if (key < curr->key) {
    new_internal = create_internal(curr->key, new_leaf, curr);
  } else {
    new_internal = create_internal(key, curr, new_leaf);
  }

  // attach the new internal router to the parent, replacing the old single leaf
  if (want_left) {
    parent->data.internal.left = new_internal;
  } else {
    parent->data.internal.right = new_internal;
  }

  return root;
}
