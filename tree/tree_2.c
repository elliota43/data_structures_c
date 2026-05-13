#include <malloc/_malloc_type.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char value[64];
} object_t;

typedef struct tree_node {
  int key;
  struct tree_node *left;
  struct tree_node *right;
} tree_node_t;

tree_node_t *create_tree(void) {
  tree_node_t *tmp_node = (tree_node_t *)malloc(sizeof(tree_node_t));
  if (!tmp_node)
    exit(EXIT_FAILURE);

  tmp_node->left = NULL;
  tmp_node->right = NULL;
  return tmp_node;
}

object_t *search(tree_node_t *root, int query_key) {
  if (root->left == NULL)
    return NULL;

  tree_node_t *curr = root;

  while (curr->right != NULL) {
    if (query_key < curr->key) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }

  if (curr->key == query_key) {
    // we are storing the object in the leaf node's left pointer
    return (object_t *)curr->left;
  }

  return NULL;
}

void insert(tree_node_t *root, int key, object_t *payload) {
  // empty tree
  // dummy root transforms into the first leaf
  if (root->left == NULL) {
    root->key = key;
    root->left = (tree_node_t *)payload;
    root->right = NULL;
    return;
  }

  // traverse to find where to insert
  tree_node_t *curr = root;
  while (curr->right != NULL) {
    if (key < curr->key) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }

  // key already exists, just update payload
  if (curr->key == key) {
    curr->left = (tree_node_t *)payload;
    return;
  }

  // split the leaf

  // create new leaf node for the old data
  tree_node_t *old_leaf = (tree_node_t *)malloc(sizeof(tree_node_t));
  old_leaf->key = curr->key;
  old_leaf->left = curr->left; // transfer the old object pointer
  old_leaf->right = NULL;

  // create a new leaf node for the incoming data
  tree_node_t *new_leaf = (tree_node_t *)malloc(sizeof(tree_node_t));
  new_leaf->key = key;
  new_leaf->left = (tree_node_t *)payload; // store new object pointer
  new_leaf->right = NULL;

  // MUTATE THE CURRENT NODE IN-PLACE
  // 'curr' was a leaf. Now it becomes an internal router node pointing
  // to the two leaves we just created.
  if (key < curr->key) {
    curr->key = curr->key; // routing key becomes the larger of the two
    curr->left = new_leaf;
    curr->right = old_leaf;
  } else {
    curr->key = key; // routing key becomes the larger of the two
    curr->left = old_leaf;
    curr->right = new_leaf;
  }
}

void rotate_left(tree_node_t *n) {
  // we can only rotate left if n is an internal node and its right child is
  // also an internal node.
  // In this model, a node is a leaf it right == NULL
  if (n == NULL || n->right == NULL || n->right->right == NULL)
    return;

  tree_node_t *R = n->right; // the right internal node
  tree_node_t *A = n->left;  // the first interval (left of n)
  tree_node_t *B = R->left;  // the second interval (left of R)
  tree_node_t *C = R->right; // the third interval (right of R)

  int k1 = n->key;
  int k2 = R->key;

  // Repurpose the old Right node (R) to become the new Left node.
  // we just overwrite its routing key and give it the first two intervals.
  tree_node_t *L = R;
  L->key = k1;
  L->left = A;
  L->right = B;

  // mutate the rotation center (*n) in-place
  // it gets the new larger routing key, points to the repurposed left node,
  // and points directly to the third interval.
  n->key = k2;
  n->left = L;
  n->right = C;
}

void rotate_right(tree_node_t *n) {
  // We can only perform a right rotation if n is an internal node and its
  // left child is also an internal node.
  // again, a node is a leaf if right == NULL
  // Thus, if n->left->right is NULL, n->left is a leaf, and we cannot rotate.
  if (n == NULL || n->left == NULL || n->left->right == NULL)
    return;

  tree_node_t *L = n->left;
  tree_node_t *A = L->left;
  tree_node_t *B = L->right;
  tree_node_t *C = n->right;

  int k1 = n->key; // the larger routing key currently at the top
  int k2 = L->key; // the smaller routing key currently on the left

  // Repurpose the old Left node (L) to become the new Right node.
  // We overwrite its routing key with the larger routing key, and hand it
  // the two right-most intervals.
  tree_node_t *R = L;
  R->key = k1;
  R->left = B;
  R->right = C;

  // Mutate the rotation center (*n) in-place
  // it gets the smaller routing key, points directly to the first interval,
  // and points its right child to the repurposed right node.
  n->key = k2;
  n->left = A;
  n->right = R;
}

void print_tree_helper(tree_node_t *n, int depth) {
  if (n == NULL)
    return;

  bool is_leaf = (n->right == NULL);

  // Traverse right (prints at top of the console)
  if (!is_leaf) {
    print_tree_helper(n->right, depth + 1);
  }

  for (int i = 0; i < depth; i++) {
    printf("           ");
  }

  if (is_leaf) {
    object_t *obj = (object_t *)n->left;
    printf("[Leaf: %2d] -> \"%s\"\n", n->key, obj->value);
  } else {
    printf("[Int:  %2d]\n", n->key);
  }

  // traverse left (prints at bottom of the console)
  if (!is_leaf) {
    print_tree_helper(n->left, depth + 1);
  }
}

void print_tree(tree_node_t *root) {
  printf("\n=== Current Tree Structure ===\n");

  if (root->left == NULL) {
    printf("(Empty Tree)\n");
  } else {
    print_tree_helper(root, 0);
  }

  printf("================================\n");
}

int main(void) {
  tree_node_t *tree = create_tree();

  object_t *obj1 = malloc(sizeof(object_t));
  strcpy(obj1->value, "Data for 10");
  object_t *obj2 = malloc(sizeof(object_t));
  strcpy(obj2->value, "Data for 20");
  object_t *obj3 = malloc(sizeof(object_t));
  strcpy(obj3->value, "Data for 5");
  object_t *obj4 = malloc(sizeof(object_t));
  strcpy(obj4->value, "Data for 30");

  insert(tree, 10, obj1);
  insert(tree, 20, obj2);
  insert(tree, 5, obj3);
  insert(tree, 30, obj4);

  printf("\nBefore Rotation:");
  print_tree(tree);

  rotate_left(tree);

  printf("\nAfter Left Rotation:");
  print_tree(tree);

  rotate_right(tree);

  printf("\nAfter Right Rotation:");
  print_tree(tree);

  return 0;
}
