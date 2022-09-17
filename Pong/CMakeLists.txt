cmake_minimum_required(VERSION 3.10)
project(Pong)


include(FetchContent)
FetchContent_Declare(SFML
   GIT_REPOSITORY https://github.com/SFML/SFML.git
   GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(SOURCES
	main.cpp
	ball.h
	ball.cpp
	paddle.h
	paddle.cpp
	vectorMath.h
	vectorMath.cpp
)
add_executable(Pong ${SOURCES})
target_include_directories(Pong PUBLIC textures/)
target_include_directories(Pong PUBLIC fonts/)

# add the SFML Libraries
target_link_libraries(Pong PRIVATE sfml-graphics)
target_compile_features(Pong PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Pong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Pong> $<TARGET_FILE_DIR:Pong> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS Pong)