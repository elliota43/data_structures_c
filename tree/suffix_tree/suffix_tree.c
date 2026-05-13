#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

typedef struct SuffixTreeNode {
  struct SuffixTreeNode *children[MAX_CHAR];
  int suffixIndex; // starts at 0; remains -1 if an internal node
} SuffixTreeNode;

SuffixTreeNode *create_node(void) {
  SuffixTreeNode *node = (SuffixTreeNode *)malloc(sizeof(SuffixTreeNode));
  if (!node) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  for (int i = 0; i < MAX_CHAR; i++) {
    node->children[i] = NULL;
  }
  node->suffixIndex = -1;
  return node;
}

void insert_suffix(SuffixTreeNode *root, const char *suffix, int index) {
  SuffixTreeNode *curr = root;

  for (int i = 0; suffix[i] != '\0'; i++) {
    unsigned char charIndex = (unsigned char)suffix[i];

    if (curr->children[charIndex] == NULL) {
      curr->children[charIndex] = create_node();
    }

    curr = curr->children[charIndex];
  }

  curr->suffixIndex = index;
}

SuffixTreeNode *build_suffix_tree(const char *text) {
  SuffixTreeNode *root = create_node();

  int len = strlen(text);

  for (int i = 0; i < len; i++) {
    insert_suffix(root, text + i, i);
  }
  return root;
}

void collect_leaf_indices(SuffixTreeNode *node) {
  if (node == NULL)
    return;

  if (node->suffixIndex != -1) {
    printf("- Found at index: %d\n", node->suffixIndex);
  }

  for (int i = 0; i < MAX_CHAR; i++) {
    if (node->children[i] != NULL) {
      collect_leaf_indices(node->children[i]);
    }
  }
}

void search_pattern(SuffixTreeNode *root, const char *pattern) {
  SuffixTreeNode *curr = root;

  for (int i = 0; pattern[i] != '\0'; i++) {
    unsigned char charIndex = (unsigned char)pattern[i];

    if (curr->children[charIndex] == NULL) {
      printf("Pattern '%s' not found.\n", pattern);
      return;
    }
    curr = curr->children[charIndex];
  }

  printf("Pattern '%s' found. Occurrences:\n", pattern);
  collect_leaf_indices(curr);
}

void free_tree(SuffixTreeNode *node) {
  if (node == NULL)
    return;

  for (int i = 0; i < MAX_CHAR; i++) {
    if (node->children[i] != NULL) {
      free_tree(node->children[i]);
    }
  }
  free(node);
}

int main(void) {
  const char *text = "banana$";

  printf("Building tree for text: %s\n", text);

  SuffixTreeNode *root = build_suffix_tree(text);

  search_pattern(root, "nan");
  search_pattern(root, "ana");
  search_pattern(root, "apple");

  free_tree(root);
  return 0;
}
