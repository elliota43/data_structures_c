#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  DICT_VAL_INT,
  DICT_VAL_FLOAT,
  DICT_VAL_BOOL,
  DICT_VAL_STRING, // null terminated char array
  DICT_VAL_BUFFER, // raw binary data (needs a size track)
  DICT_VAL_CUSTOM_PTR,
} DictValueType;

typedef struct {
  DictValueType type;

  union {
    int64_t i;
    double f;
    char *str;

    struct {
      uint8_t *data;
      size_t size;
    } buffer;

    void *ptr;
  } as;
} DictValue;

typedef struct Node {
  const char *key;
  DictValue value;
  struct Node *left;
  struct Node *right;
} Node;

DictValue *create_string_value(const char *input) {
  DictValue *value = malloc(sizeof(DictValue));
  value->type = DICT_VAL_STRING;
  value->as.str = strdup(input);
  return value;
}

DictValue *create_buffer_value(void *raw_data, size_t length) {
  DictValue *value = malloc(sizeof(DictValue));
  value->type = DICT_VAL_BUFFER;
  value->as.buffer.size = length;
  value->as.buffer.data = malloc(length);
  memcpy(value->as.buffer.data, raw_data, length);
  return value;
}
