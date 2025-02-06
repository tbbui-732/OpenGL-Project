# CMake v3.31
cmake_minimum_required(VERSION 3.31)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.1")
project(OpenGL VERSION 1.0)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Executables
set(CHAPTER "3.model_loading") # NOTE: update this depending on the chapter being built!
set(SUBCHAPTER "model")
add_executable(${PROJECT_NAME} src/${CHAPTER}/${SUBCHAPTER}/main.cpp src/glad.c src/stb_image.c)

# Libraries
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

find_library(GLFW glfw PATHS ${LIB_DIR})
find_library(ASSIMP assimp PATHS ${LIB_DIR})

target_link_libraries(${PROJECT_NAME} ${GLFW} ${ASSIMP})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
endif()

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
