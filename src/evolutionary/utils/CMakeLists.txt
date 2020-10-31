set(UTILS_INCLUDE .)

if(TARGET EvolutionaryUtilsLib)

message("EvolutionaryUtilsLib is already defined")

else()

add_library(EvolutionaryUtilsLib EvolutionaryUtils.cpp)
target_include_directories(EvolutionaryUtilsLib PUBLIC ${UTILS_INCLUDE})

endif()