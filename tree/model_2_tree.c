#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
