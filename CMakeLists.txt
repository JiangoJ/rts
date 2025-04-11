cmake_minimum_required(VERSION 3.11)

# [Dependencies]: raylib, nlohmann-json, llama.cpp, fmt

# Needed to add VERSION because some subdirectory libraries utilize CMAKE_PROJECT_VERSION
project(rts VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB RTS_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

find_package(raylib REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)

# Find all include directories for LLAMA
set(LLAMA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/llama.cpp)
file(GLOB_RECURSE LLAMA_HEADER_FILES LIST_DIRECTORIES true "${LLAMA_DIR}/*/include/*h")
set(LLAMA_INCLUDE_DIRS "")
foreach(header ${LLAMA_HEADER_FILES})
  get_filename_component(dir "${header}" DIRECTORY)
  list(APPEND LLAMA_INCLUDE_DIRS "${dir}")
endforeach()
list(REMOVE_DUPLICATES LLAMA_INCLUDE_DIRS)

# Link, include dirs
add_executable(rts main.cpp ${RTS_SRC})
target_include_directories(rts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rts PRIVATE ${LLAMA_DIR}/include ${LLAMA_INCLUDE_DIRS})
target_link_libraries(rts PRIVATE raylib nlohmann_json::nlohmann_json ${LLAMA_DIR}/build/common/libcommon.a ${LLAMA_DIR}/build/bin/libllama.dylib fmt::fmt curl)
