# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(docs_tutorial)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND HEADERS
  "${PROJECT_BINARY_DIR}/zephyr/include/generated"
  "${PROJECT_SOURCE_DIR}/include"
  )
include_directories(${HEADERS})

target_sources(app PRIVATE src/blink.c src/main.c)
