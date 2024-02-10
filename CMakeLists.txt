cmake_minimum_required(VERSION 3.0.0)
project(SFML_Game VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(SFML_Game src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
target_link_libraries(SFML_Game sfml-graphics sfml-audio)