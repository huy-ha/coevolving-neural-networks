file(GLOB SELECTOR_SOURCE
    "*.cpp"
)

add_library(SelectorLib ${SELECTOR_SOURCE})
target_include_directories(SelectorLib PUBLIC .)
target_link_libraries(SelectorLib CreatureLib EvolutionaryUtilsLib)

set_property(TARGET SelectorLib PROPERTY CXX_STANDARD 17)