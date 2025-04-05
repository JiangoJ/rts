cmake_minimum_required(VERSION 3.11)

# Needed to add VERSION because some subdirectory libraries utilize CMAKE_PROJECT_VERSION
project(rts VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB RTS_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# OpenGL
find_package(OpenGL REQUIRED)
# glfw installed via brew doesnt come with a cmake config file
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(rts main.cpp ${RTS_SRC})
target_include_directories(rts PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENGL_gl_LIBRARY} ${GLFW_INCLUDE_DIRS})
target_link_libraries(rts ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
