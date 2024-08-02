# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(tpp_physim_example)

# Custom command to generate QEMU binary
add_custom_command(OUTPUT SYMBOLIC qemu_flash_image.bin
                   COMMAND esptool.py "--chip" "esp32" "merge_bin" "--fill-flash-size" "4MB" "-o" "qemu_flash_image.bin" "@flash_args"
                   DEPENDS app)
add_custom_target(qemu_flash_image ALL DEPENDS qemu_flash_image.bin)