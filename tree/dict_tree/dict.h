#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --- types --- */

typedef enum {
  DICT_VAL_INT,
  DICT_VAL_FLOAT,
  DICT_VAL_STRING,
  DICT_VAL_BUFFER,
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

// Dictionary handle
typedef struct Dict Dict;

typedef void (*DictIterFunc)(const char *key, DictValue *value,
                             void *user_data);

typedef int (*KeyComparator)(const char *k1, const char *k2);

/* --- Lifecycle --- */

Dict *dict_new(void);
Dict *dict_new_case_insensitive(void);
void dict_free(Dict *dict);
void dict_clear(Dict *dict);

/* --- Value Creation Helpers --- */

DictValue *create_string_value(const char *input);
DictValue *create_buffer_value(void *raw_data, size_t length);
DictValue *create_int_value(int64_t input);
DictValue *create_float_value(double input);
DictValue *create_ptr_value(void *input);
void dict_value_free(DictValue *value);

/* --- API Operations --- */

bool dict_insert(Dict *dict, const char *key, DictValue *value);
bool dict_remove(Dict *dict, const char *key);

DictValue *dict_get(Dict *dict, const char *key);

/* --- Introspection & Iteration --- */

size_t dict_size(const Dict *dict);
bool dict_is_empty(const Dict *dict);
void dict_foreach(const Dict *dict, DictIterFunc callback, void *user_data);

#endif // DICT_H
