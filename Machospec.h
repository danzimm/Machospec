
#ifndef Machospec_h
#define Machospec_h

#include <mach-o/dyld_images.h>

const char **machospec_copy_image_paths(struct dyld_all_image_infos* infos, uint32_t *count);

#endif // Machospec_h

