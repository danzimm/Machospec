
#ifndef Machospec_h
#define Machospec_h

#include <mach-o/loader.h>
#include <mach-o/dyld_images.h>

const char **machospec_copy_image_paths(struct dyld_all_image_infos* infos, uint32_t *count);
void **machospec_copy_local_symbols(struct mach_header_64* header,
                                    const char ***symbolNames,
                                    uint32_t *count);

#endif // Machospec_h

