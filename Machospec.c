#include "Machospec.h"

#include <stdlib.h>

const char **machospec_copy_image_paths(struct dyld_all_image_infos* infos, uint32_t *count) {
  uint32_t cnt = infos->infoArrayCount;
  *count = cnt;
  const char **result = (const char**)malloc(sizeof(char*) * cnt);
  for (uint32_t i = 0; i < cnt; i++) {
    result[i] = infos->infoArray[i].imageFilePath;
  }
  return result;
}

