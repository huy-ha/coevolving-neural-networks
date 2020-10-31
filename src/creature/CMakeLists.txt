add_library(CreatureLib Creature.cpp )
target_link_libraries (CreatureLib NeuralLib VoxelizeLib SimulatorLib)
target_include_directories(CreatureLib PRIVATE ${PROJECT_SOURCE_DIR}/extern/voxelize/include ${PROJECT_SOURCE_DIR}/include )
target_include_directories(CreatureLib PUBLIC ./ )

add_executable( MutateDemo mutate_demo.cpp)
set_property(TARGET MutateDemo PROPERTY CXX_STANDARD 17)
target_link_libraries (MutateDemo CreatureLib)


add_executable( CreatureVisualizer creature_visualizer.cpp)
set_property(TARGET CreatureVisualizer PROPERTY CXX_STANDARD 17)
target_link_libraries (CreatureVisualizer CreatureLib)
target_include_directories(CreatureVisualizer PRIVATE ${PROJECT_SOURCE_DIR}/include/nlohmann )