
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(led_clock)

set(COMPONENTS "alexa_gadgets_toolkit" "FastLED-idf")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
