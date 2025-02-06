cmake_minimum_required(VERSION 3.28)
project(ProjectName LANGUAGES C)

add_subdirectory("external/raylib")


add_executable(ProjectName main.c)

target_include_directories(ProjectName PRIVATE
        "external/clay"
)


target_link_libraries(ProjectName PRIVATE
        raylib
)

add_custom_command(
        TARGET ProjectName POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)

if (EMSCRIPTEN)
    target_compile_options(ProjectName PRIVATE)
    set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -s ENVIRONMENT=web -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FORCE_FILESYSTEM=1 -s FETCH_DEBUG --preload-file assets")
endif ()



set_target_properties(ProjectName
        PROPERTIES
        C_STANDARD 23
)


