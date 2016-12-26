Machospec
===
This repo contains code used to inspect mach-o files at runtime. To start there will be a function which takes a `struct dyld_all_image_infos*` and prints out all the loaded images with their current states. You can retrieve the `struct dyld_all_image_infos*` via `task_info` with the flavor of `TASK_DYLD_INFO`.

