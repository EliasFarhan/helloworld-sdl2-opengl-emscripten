cmake_minimum_required(VERSION 2.10)

project(HelloWorldSDLOpenGLWeb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets/)
#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/imgui/misc/fonts)
file(COPY ${CMAKE_SOURCE_DIR}/imgui/misc/fonts/ DESTINATION ${CMAKE_BINARY_DIR}/imgui/misc/fonts/)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    include(cmake/Emscripten.cmake)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s DISABLE_EXCEPTION_CATCHING=1 -s USE_WEBGL2=1 -s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=\"[\"png\", \"jpg\"]\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  --emrun --use-preload-plugins --preload-file assets/ --preload-file imgui/")
    add_executable(main main.cpp glwrappers.cpp window.cpp atlas.cpp
            font.cpp render-sprites.cpp render-surface.cpp render-imgui.cpp
            imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
            imgui/imgui_demo.cpp)
else ()
    include(FindPkgConfig)
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    add_executable(main main.cpp glwrappers.cpp window.cpp atlas.cpp
            font.cpp render-sprites.cpp render-surface.cpp render-imgui.cpp
            imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
            imgui/imgui_demo.cpp)
    message("${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}")
    target_link_libraries(main ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

endif ()




