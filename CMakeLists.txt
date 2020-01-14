project(assimp-learn)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# add_library(STB_IMAGE "src/tools/stb_image.cpp")
# set(LIBS ${LIBS} STB_IMAGE)

# add_library(GLAD "src/tools/glad.c")
# set(LIBS ${LIBS} GLAD)

# add_library(SHADER_LOADER "src/tools/my_shader_loader.cpp")
# set(LIBS ${LIBS} SHADER_LOADER)

# add_library(TEXTURE_LOADER "src/tools/texture_2d.cpp")
# set(LIBS ${LIBS} TEXTURE_LOADER)

include_directories(/System/Library/Frameworks)
include_directories(${CMAKE_SOURCE_DIR}/includes)

find_library(COCOA_LIBRARY Cocoa)
find_library(OpenGL_LIBRARY OpenGL)
find_library(IOKit_LIBRARY IOKit)
find_library(CoreVideo_LIBRARY CoreVideo)
mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
set(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY})
set(LIBS ${LIBS} ${APPLE_LIBS} ${ASSIMP_LIBRARY})

file(GLOB SHADERS
  "src/shader/*.vs"
  "src/shader/*.fs"
  "src/shader/*.gs"
)
foreach(SHADER ${SHADERS})
  file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
endforeach(SHADER)

set(NAME "main")
# file(GLOB SRC
#     "src/*.h"
#     "src/*.cpp"
#     "src/*.vs"
#     "src/*.fs"
#     "src/*.gs"
# )
aux_source_directory("src/" SRC)
aux_source_directory("src/glad/" SRC)
aux_source_directory("src/stdimage/" SRC)
aux_source_directory("src/tools/" SRC)
aux_source_directory("src/model/" SRC)
aux_source_directory("src/gui/" SRC)
add_executable(${NAME} ${SRC})
target_link_libraries(${NAME} ${LIBS})

set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/")
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin/")
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin/")
