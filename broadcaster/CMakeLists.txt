# CMakeList.txt : projet CMake pour broadcaster, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
project (broadcaster VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

find_package(glfw3 REQUIRED)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 340acc5cdad624b9398827eec2832fe2fd05f966) # The commit hash for 1.9.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

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

set(API
    ./src/api/api.cpp ./src/api/api.h
    ./src/api/json.hpp
)

add_executable(broadcaster "main.cpp" ${GRAPHICS} ${OBJECTS} ${CAPTURE} ${API})
target_include_directories(broadcaster PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)
link_directories(broadcaster PUBLIC ${CMAKE_SOURCE_DIR}/lib)

if(CMAKE_SYSTEM MATCHES Windows)
   target_link_libraries(broadcaster PUBLIC glfw3.lib ${OpenCV_LIBS} ${CMAKE_DL_LIBS} cpr::cpr)
endif()

if(CMAKE_HOST_SYSTEM MATCHES Linux)
   target_link_libraries(broadcaster PUBLIC glfw ${OpenCV_LIBS} ${CMAKE_DL_LIBS} cpr::cpr)
endif()

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")
