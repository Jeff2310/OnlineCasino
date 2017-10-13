cmake_minimum_required(VERSION 3.8)
project(OnlineCasino)

set(CMAKE_CXX_STANDARD 11)
set(INC_DIR C:/MinGW/include)
set(LINK_DIR C:/MinGW/lib)

set(SOURCE_FILES casino.cpp ncurses.cpp gui.h)

add_executable(OnlineCasino ${SOURCE_FILES})
target_link_libraries(OnlineCasino ncurses)