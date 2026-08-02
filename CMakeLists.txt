cmake_minimum_required(VERSION 3.31.6)

project(alpha)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs 
    core 
    support 
    irreader
    codegen
    asmprinter
    target
    native
    TargetParser
)


file(GLOB_RECURSE ALL_SRC_CPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER ALL_SRC_CPP EXCLUDE REGEX ".*main\\.cpp$")

file(GLOB_RECURSE ALL_SRC_HPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(alpha_lib STATIC ${ALL_SRC_CPP} ${ALL_SRC_HPP})

target_include_directories(alpha_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(alpha_lib PUBLIC ${llvm_libs})

# Main compiler

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE alpha_lib)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# Tests
find_package(Catch2 3 REQUIRED)

enable_testing()
file(GLOB_RECURSE ALL_TEST_CPP CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")


add_executable(tests 
    ${ALL_TEST_CPP}
)

target_link_libraries(tests PRIVATE 
    alpha_lib
    Catch2::Catch2WithMain
)

get_target_property(Catch2_INCLUDE_DIR Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(tests PRIVATE ${Catch2_INCLUDE_DIR})

include(Catch)
catch_discover_tests(tests)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)