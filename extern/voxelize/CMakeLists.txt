cmake_minimum_required( VERSION 3.0 )
project(Voxelize CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

enable_testing()
add_subdirectory(
    ${PROJECT_SOURCE_DIR}/extern/googletest
)
add_subdirectory(test)

set(CMAKE_BUILD_TYPE Release)
