cmake_minimum_required(VERSION 3.20)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

project(rvasm VERSION 1.0)

add_subdirectory(risc-v-asm)

add_executable(rvasm main.c)
target_link_libraries(rvasm PRIVATE ${RISC_V_ASM_LIBS} m)
target_include_directories(rvasm PRIVATE ${RISC_V_ASM_INCLUDE_DIRS})