add_subdirectory(utils)
add_subdirectory(solver)
add_subdirectory(selector)
add_subdirectory(reproducer)

add_executable(Solve Solve.cpp)
target_link_libraries(Solve SolverLib)