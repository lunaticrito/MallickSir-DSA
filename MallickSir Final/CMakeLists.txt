cmake_minimum_required(VERSION 3.16)
project(MathEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(DSA_SOURCES
    array_ops.cpp
    stack_queue.cpp
    expression_engine.cpp
    singly_linked.cpp
    doubly_linked.cpp
    sorting.cpp
    bst.cpp
    bit.cpp
    heap.cpp
    graph.cpp
    symbolic.cpp
    math_engine.cpp
)

#Terminal build 
add_executable(MathEngine
    ${DSA_SOURCES}
    terminal_ui.cpp
)

# Qt build (Phase 2) — uncomment when ready 
# find_package(Qt6 REQUIRED COMPONENTS Widgets)
# add_executable(MathEngineQt
#     ${DSA_SOURCES}
#     qt_ui.cpp
# )
# target_link_libraries(MathEngineQt Qt6::Widgets)
