cmake_minimum_required(VERSION 3.11)

# Needed to add VERSION because some subdirectory libraries utilize CMAKE_PROJECT_VERSION
project(rts VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB RTS_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

find_package(raylib REQUIRED)

add_executable(rts main.cpp ${RTS_SRC})
target_include_directories(rts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_link_libraries(rts raylib)

