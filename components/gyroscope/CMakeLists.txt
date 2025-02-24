# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
# cmake_minimum_required(VERSION 3.16)
get_filename_component(dir ${CMAKE_CURRENT_LIST_FILE} PATH)
FILE(GLOB_RECURSE app_sources ${dir}/QMI8658/*.cpp)

idf_component_register(SRCS ${app_sources}      
                    INCLUDE_DIRS "."
                    REQUIRES arduino-esp32
)
