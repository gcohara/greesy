cmake_minimum_required(VERSION 3.19)
set(CMAKE_TOOLCHAIN_FILE "cmake/arm-none-eabi.cmake")
project(greesyn LANGUAGES C CXX ASM)

# MCU specific
set(LINKER_SCRIPT "../ld/stm32f3discovery.ld")
set(MCU_OPTIONS -mcpu=cortex-m4 -mthumb)

file(GLOB SOURCES "src/*.cpp" "src/*.S")
add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_include_directories(${PROJECT_NAME}.elf PRIVATE
  inc
  )

target_compile_options(${PROJECT_NAME}.elf PRIVATE
  ${MCU_OPTIONS}
  -fno-exceptions
  --specs=nosys.specs
  -Wall
  -Wextra
  -g
  )

target_link_options(${PROJECT_NAME}.elf PRIVATE
  ${MCU_OPTIONS}
  -T${LINKER_SCRIPT}
  -Wall
  --specs=nosys.specs
  -nostdlib
  -lgcc
  )

# So that a binary file is generated from the ELF file
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf)
add_custom_command(
  TARGET ${PROJECT_NAME}.bin
  COMMAND ${CMAKE_OBJCOPY} ARGS -S -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
  )


# So that we can flash by using 'make flash'
add_custom_target(flash DEPENDS ${PROJECT_NAME}.bin)
add_custom_command(
  TARGET flash
  USES_TERMINAL
  COMMAND st-flash write ${PROJECT_NAME}.bin 0x08000000
  )
