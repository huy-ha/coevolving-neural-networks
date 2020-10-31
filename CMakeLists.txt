cmake_minimum_required( VERSION 3.0 )

# project name
project( creature_evolution )

# files

file(GLOB SIMULATOR_SOURCE
    "src/engine/core/*.cpp"
    "src/engine/collision/*.cpp"
    "src/engine/force/*.cpp"
)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/extern/voxelize
)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/src/neural
)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/src/creature
)


add_subdirectory(
    ${PROJECT_SOURCE_DIR}/src/evolutionary
)

# include
include_directories(./src ./include ./extern/voxelize/include ./src/creature) 

# target
add_library(SimulatorLib ${SIMULATOR_SOURCE})
target_include_directories(SimulatorLib PUBLIC
    ${PROJECT_SOURCE_DIR}/src/engine/core
)
find_package (Threads)
target_link_libraries (SimulatorLib ${CMAKE_THREAD_LIBS_INIT} stdc++fs VoxelizeLib CreatureLib)

set_property(TARGET SimulatorLib PROPERTY CXX_STANDARD 17)

add_executable( demo  ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries (demo CreatureLib SimulatorLib)

#target
set(CMAKE_BUILD_TYPE Release)