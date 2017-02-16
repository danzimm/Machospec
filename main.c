#include <assert.h>
#include <dlfcn.h>
#include <mach/mach_error.h>
#include <mach/task_info.h>
#include <stdio.h>
#include <stdlib.h>

#include "Machospec.h"

int main() {
  struct task_dyld_info info;
  unsigned count = TASK_DYLD_INFO_COUNT;
  kern_return_t kr = task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&info, &count);
  if (kr != KERN_SUCCESS) {
    mach_error("task_info", kr);
    return 1;
  }
  struct dyld_all_image_infos* infos = (struct dyld_all_image_infos*)info.all_image_info_addr;
  uint32_t imageCount = 0;
  const char** paths = machospec_copy_image_paths(infos, &imageCount);
  printf("%u images loaded:\n", imageCount);
  for (uint32_t i = 0; i < imageCount; i++) {
    printf("  %s\n", paths[i]);
  }
  free(paths);
  struct mach_header_64* dyldHeader = (struct mach_header_64*)infos->dyldImageLoadAddress;
  assert(dyldHeader->magic == MH_MAGIC_64);
  const char **symbolNames = NULL;
  uint32_t symbolCount = 0;
  
  Dl_info dlinfo;
  assert(dladdr(main, &dlinfo) != 0);
  struct mach_header_64* hdr = (struct mach_header_64*)dlinfo.dli_fbase;
  assert(hdr->magic == MH_MAGIC_64);

  void **symbols = machospec_copy_local_symbols(dyldHeader, &symbolNames, &symbolCount);
  assert(symbols);
  printf("%u symbols found:\n", symbolCount);
  for (uint32_t i = 0; i < symbolCount; i++) {
    printf("  %s: %p\n", symbolNames[i], symbols[i]);
    if (strcmp(symbolNames[i], "_main") == 0) {
      assert(symbols[i] == main);
    }
  }
  free(symbols);
  free(symbolNames);
  return 0;
}

