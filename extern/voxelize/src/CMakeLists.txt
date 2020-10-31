add_library(VoxelizeLib ${PROJECT_SOURCE_DIR}/src/voxelize/Voxelize.cpp)
target_include_directories(VoxelizeLib PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(VoxelizeDemo ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(VoxelizeDemo VoxelizeLib)