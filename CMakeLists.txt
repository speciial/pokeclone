cmake_minimum_required(VERSION "3.26.0")

# Setting the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin)

# Removing libs that are not needed. Does not remove windows libs like user32.lib
set(CMAKE_OPTIMIZE_DEPENDENCIES 1)

# NOTE:
#   This will work for now but in the future I'll have to go through this and further
#   customize this setup. There is so much about this that's still annoying and the 
#   compile time isn't all that good either. 

# Project Setup
project("pokeclone")
add_executable(${PROJECT_NAME} "src/Application.c" "src/Renderer.c" "src/System.c")

# Resource Folder
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/data
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
    COMMENT "Copying resources into binary folder.")

# Libraries
add_subdirectory("third_party/glfw")

target_include_directories(${PROJECT_NAME}
    PUBLIC third_party/glfw/include
    PUBLIC third_party/glad
    PUBLIC third_party/stb
)

target_link_directories(${PROJECT_NAME} 
    PUBLIC third_party/glfw/src/Debug
)

target_link_libraries(${PROJECT_NAME} 
    opengl32.lib
    glfw
)