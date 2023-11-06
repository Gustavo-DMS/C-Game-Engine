#include "../io.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"
#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_CHUNK_SIZE 2097152

// File read_file(const char *path) {
//   File file = {.is_valid = false};
//
//   FILE *fp = fopen(path, "rb");
//
//   if (!fp || ferror(fp)) {
//     fprintf(stderr, IO_READ_ERROR_GENERAL, path, errno);
//     exit(1);
//   }
//
//   char *data = NULL;
//   char *tmp;
//   size_t used = 0;
//   size_t size = 0;
//   size_t n;
//
//   while (true) {
//     size = used + IO_READ_CHUNK_SIZE + 1;
//
//     if (used + IO_READ_CHUNK_SIZE + 1 > size) {
//       if (size <= used) {
//         free(data);
//         fprintf(stderr, "Input file too large: %s\n", data);
//         exit(1);
//       }
//
//       tmp = realloc(data, size);
//
//       if (!tmp) {
//         free(data);
//         fprintf(stderr, IO_READ_ERROR_MEMORY, path);
//         exit(1);
//       }
//
//       data = tmp;
//     }
//     n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
//     if (n == 0) {
//       break;
//     }
//     used += n;
//   }
//
//   if (ferror(fp)) {
//     free(data);
//     fprintf(stderr, IO_READ_ERROR_GENERAL, path, errno);
//     exit(1);
//   }
//
//   tmp = realloc(data, used + 1);
//   if (!tmp) {
//     free(data);
//     fprintf(stderr, IO_READ_ERROR_MEMORY, path);
//     exit(1);
//   }
//   data = tmp;
//   data[used] = 0;
//
//   file.data = data;
//   file.len = used;
//   file.is_valid = true;
//
//   return file;
// }

File read_file(const char *path) {
  File file = {.is_valid = false};

  FILE *fp = fopen(path, "rb");
  if (!fp || ferror(fp)) {
    fprintf(stderr, IO_READ_ERROR_GENERAL, path, errno);
    return file;
  }

  char *data = NULL;
  char *tmp;
  size_t used = 0;
  size_t size = 0;
  size_t n;

  while (true) {
    if (used + IO_READ_CHUNK_SIZE + 1 > size) {
      size = used + IO_READ_CHUNK_SIZE + 1;

      if (size <= used) {
        free(data);
        fprintf(stderr, "Input file too large: %s\n", path);
        return file;
      }

      tmp = realloc(data, size);
      if (!tmp) {
        free(data);
        fprintf(stderr, IO_READ_ERROR_MEMORY, path);
        return file;
      }
      data = tmp;
    }

    n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
    if (n == 0)
      break;

    used += n;
  }

  if (ferror(fp)) {
    free(data);
    fprintf(stderr, IO_READ_ERROR_GENERAL, path, errno);
    return file;
  }

  tmp = realloc(data, used + 1);
  if (!tmp) {
    free(data);
    fprintf(stderr, IO_READ_ERROR_MEMORY, path);
    return file;
  }
  data = tmp;
  data[used] = 0;

  file.data = data;
  file.len = used;
  file.is_valid = true;

  return file;
}
