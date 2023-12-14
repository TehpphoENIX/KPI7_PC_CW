cmake_minimum_required(VERSION 3.25.0)
project(CW VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(CPack)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CTest)
enable_testing()
set(TIMEOUT 100)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(CWServer "exec/main.cpp" ${SOURCES})

add_subdirectory(tests)
