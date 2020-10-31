file(GLOB REPRODUCER_SOURCE
    "*.cpp"
)

add_library(ReproducerLib ${REPRODUCER_SOURCE})
target_include_directories(ReproducerLib PUBLIC .)
target_link_libraries(ReproducerLib CreatureLib EvolutionaryUtilsLib NeuralLib)

set_property(TARGET ReproducerLib PROPERTY CXX_STANDARD 17)