cmake_minimum_required(VERSION 3.0)
project(SDLBitmapFontSample)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SDLBITMAPFONT_HDRS "${CMAKE_CURRENT_LIST_DIR}/*.h")
file(GLOB SDLBITMAPFONT_SRCS "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

file(GLOB SDLBITMAPFONT_SAMPLE_HDRS "${CMAKE_CURRENT_LIST_DIR}/sample/*.h")
file(GLOB SDLBITMAPFONT_SAMPLE_SRCS "${CMAKE_CURRENT_LIST_DIR}/sample/*.cpp")

if (CMAKE_HOST_APPLE)
    add_executable(SDLBitmapFontSample
        ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SAMPLE_HDRS}
        ${SDLBITMAPFONT_SRCS} ${SDLBITMAPFONT_SAMPLE_SRCS}
    )

    execute_process(
        COMMAND brew --prefix sdl2
        OUTPUT_VARIABLE sdl2_prefix
    	OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    include("${sdl2_prefix}/lib/cmake/SDL2/sdl2-config.cmake")
    # find_package(SDL2)

	target_compile_options(SDLBitmapFontSample PRIVATE -Wall -Wextra -pedantic)

    target_include_directories(SDLBitmapFontSample PUBLIC ${SDL2_INCLUDE_DIRS})
    target_include_directories(SDLBitmapFontSample PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    target_link_directories(SDLBitmapFontSample PUBLIC ${SDL2_LIBDIR})
    target_link_libraries(SDLBitmapFontSample SDL2)
else (CMAKE_HOST_WIN32)
    add_executable(SDLBitmapFontSample
        ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SAMPLE_HDRS}
        ${SDLBITMAPFONT_SRCS} ${SDLBITMAPFONT_SAMPLE_SRCS}
    )

    # TODO
	# SDL2_INCLUDE_DIRS SDL2_LIBDIR, copy SDL2.dll
	#set(SDL2_INCLUDE_DIRS, "${CMAKE_CURRENT_LIST_DIR}/third_party/SDL2/include")
	#set(SDL2_LIBDIR, "${CMAKE_CURRENT_LIST_DIR}/third_party/SDL2/lib/x64")

    target_include_directories(SDLBitmapFontSample PUBLIC "${CMAKE_CURRENT_LIST_DIR}/third_party/SDL2/include")
    target_include_directories(SDLBitmapFontSample PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    target_link_directories(SDLBitmapFontSample PUBLIC "${CMAKE_CURRENT_LIST_DIR}/third_party/SDL2/lib/x64")
    target_link_libraries(SDLBitmapFontSample SDL2 SDL2main)

    add_custom_command(TARGET SDLBitmapFontSample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/third_party/SDL2/lib/x64/SDL2.dll $<TARGET_FILE_DIR:SDLBitmapFontSample>
	)   

endif()

source_group("Sample" FILES ${SDLBITMAPFONT_SAMPLE_HDRS} ${SDLBITMAPFONT_SAMPLE_SRCS})
source_group("Core" FILES ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SRCS})

