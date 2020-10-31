file(GLOB SOLVER_SOURCE
    "*.cpp"
)

add_library(SolverLib ${SOLVER_SOURCE})
target_include_directories(SolverLib PUBLIC .)
target_link_libraries(SolverLib CreatureLib SimulatorLib SelectorLib ReproducerLib EvolutionaryUtilsLib)

set_property(TARGET SolverLib PROPERTY CXX_STANDARD 17)