cmake_minimum_required(VERSION 3.19)
project(greesy_tests LANGUAGES C CXX ASM)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES *.cpp *.hpp ../src/synth.cpp ../src/midi.cpp ../src/peripherals/*.cpp)
set(TARGET_NAME ${PROJECT_NAME})
add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
  ../inc
  ../inc/peripherals
  )

target_link_directories(${TARGET_NAME} PRIVATE
  )

# Using this appends the library flag to the end of the command
target_link_libraries(${TARGET_NAME}
  )
