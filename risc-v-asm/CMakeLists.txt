add_library(risc-v-asm STATIC hex.c utilities.c rv32Encoding.c)
target_include_directories(risc-v-asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)