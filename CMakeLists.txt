cmake_minimum_required(VERSION 3.28.3)
project(verlet_integration VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(verlet_integration
  src/config.h
  src/config.cpp
  src/main.cpp
  src/factory.cpp
  src/render.cpp
  src/particle.cpp
  src/glad.c
  src/shader.cpp
)


target_compile_options(verlet_integration  PRIVATE -fsanitize=address)
target_link_options(verlet_integration  PRIVATE -fsanitize=address)

# Set the include directories
target_include_directories(verlet_integration 
    PRIVATE
    dependencies          
)

# Link with required libraries
target_link_libraries(verlet_integration 
    glfw
    OpenGL::GL
)
