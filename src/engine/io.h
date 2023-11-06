#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct file {
  char *data;
  bool is_valid;
  size_t len;
} File;

File read_file(const char *Path);
