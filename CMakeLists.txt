cmake_minimum_required(VERSION "3.26.0")

# Setting the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin)

# Removing libs that are not needed
set(CMAKE_OPTIMIZE_DEPENDENCIES 1)

# NOTE:
#   This will work for now but in the future I'll have to go through this and further
#   customize this setup. There is so much about this that's still annoying and the 
#   compile time isn't all that good either. 

# TODO:
#   - [x] where do the windows libs come from (ex. gdi32.lib)?
#           -> Short answer is: you can't really configure that. 
#           set(CMAKE_OPTIMIZE_DEPENDENCIES 1) apparently helps though. 
#   - [ ] how do I configure different builds (debug, release, etc.)?
#   - [ ] will this just work on linux, or do I have to modify things? 
#   - [x] how do I specify output directories (because it's imposible to find anything here)?
#   - [x] how do I deal with a data folder? 
#   - [ ] how do I create a release build? 
#   - [ ] what do PUBLIC and PRIVATE do?
#   - [ ] set warning level and warning exceptions (/W4 /WX /wd4244)

# Project Setup
project("pokeclone")
add_executable(${PROJECT_NAME} "src/application.c")

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