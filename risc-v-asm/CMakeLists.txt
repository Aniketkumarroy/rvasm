add_library(risc-v-asm STATIC hex.c utilities.c rv32Encoding.c InstructionEncoding.c)
target_include_directories(risc-v-asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RISC_V_ASM_LIBS risc-v-asm CACHE INTERNAL "riscv assembler libraries")
set(RISC_V_ASM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "riscv assembler include directories")