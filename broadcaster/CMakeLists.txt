# CMakeList.txt : projet CMake pour broadcaster, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
project (broadcaster VERSION 0.1.0)

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

set(GRAPHICS
    ./src/graphics/glad.c
    ./src/graphics/stb.cpp

    ./src/graphics/shaderClass.cpp ./src/graphics/shaderClass.h

    ./src/graphics/VAO.cpp ./src/graphics/VAO.h
    ./src/graphics/VBO.cpp ./src/graphics/VBO.h
    ./src/graphics/EBO.cpp ./src/graphics/EBO.h

    ./src/graphics/texture.cpp ./src/graphics/texture.h
    ./src/graphics/camera.h ./src/graphics/camera.cpp

    ./src/graphics/renderer.cpp ./src/graphics/renderer.h
)

set(OBJECTS
    ./src/objects/object.cpp ./src/objects/object.h
    ./src/objects/cube.h ./src/objects/cube.cpp
)

set(CAPTURE
    ./src/capture/capturer.cpp ./src/capture/capturer.h
)

add_executable(broadcaster "main.cpp" ${GRAPHICS} ${OBJECTS} ${CAPTURE})
target_include_directories(broadcaster PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
target_link_directories(broadcaster PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(broadcaster PRIVATE opengl32.lib glfw3.lib ${OpenCV_LIBS})
target_compile_features(broadcaster PRIVATE cxx_std_17)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")
