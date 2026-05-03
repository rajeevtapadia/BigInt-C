cmake_minimum_required(VERSION 3.16)

project(BigInt C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- BigInt library ----
# add_library(bigint STATIC temp.c)
add_library(bigint INTERFACE)
target_include_directories(bigint INTERFACE ${CMAKE_SOURCE_DIR})
# target_include_directories(bigint PUBLIC ${CMAKE_SOURCE_DIR})

# ---- main binary ----
add_executable(main main.c)
target_link_libraries(main PRIVATE bigint)

# ---- enable testing ----
enable_testing()

# ---- subdirs ----
add_subdirectory(examples)
add_subdirectory(tests)
