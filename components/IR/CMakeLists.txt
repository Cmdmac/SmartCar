# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
# cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS "IR.cpp" "Ir.cpp" "ir_nec_encoder.c"
                       INCLUDE_DIRS "."
                       REQUIRES arduino-esp32 ir_learn)
