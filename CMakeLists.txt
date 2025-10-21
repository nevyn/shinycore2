# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html

cmake_minimum_required(VERSION 3.5)
set(IDF_TARGET "esp32") 
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(shinycore2)
