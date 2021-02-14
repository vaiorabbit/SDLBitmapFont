cmake_minimum_required(VERSION 3.0)
project(SDLBitmapFontSample)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_HOST_APPLE)
    execute_process(
        COMMAND brew --prefix sdl2
        OUTPUT_VARIABLE sdl2_prefix
    	OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    include("${sdl2_prefix}/lib/cmake/SDL2/sdl2-config.cmake")
    # find_package(SDL2)
else (CMAKE_HOST_WIN32)
    # TODO
endif()

file(GLOB SDLBITMAPFONT_HDRS "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB SDLBITMAPFONT_SRCS "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

file(GLOB SDLBITMAPFONT_SAMPLE_HDRS "${CMAKE_CURRENT_LIST_DIR}/sample/*.h")
file(GLOB SDLBITMAPFONT_SAMPLE_SRCS "${CMAKE_CURRENT_LIST_DIR}/sample/*.cpp")

add_executable(SDLBitmapFontSample
    ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SAMPLE_HDRS}
    ${SDLBITMAPFONT_SRCS} ${SDLBITMAPFONT_SAMPLE_SRCS}
)

source_group("Sample" FILES ${SDLBITMAPFONT_SAMPLE_HDRS} ${SDLBITMAPFONT_SAMPLE_SRCS})
source_group("Core" FILES ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SRCS})

target_include_directories(SDLBitmapFontSample PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(SDLBitmapFontSample PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_directories(SDLBitmapFontSample PUBLIC ${SDL2_LIBDIR})
target_link_libraries(SDLBitmapFontSample SDL2)
