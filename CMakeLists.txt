cmake_minimum_required(VERSION 3.16)

set(LIBRARY_NAME
    CUCA
)

set(GLOBAL_NAMESPACE
    xitren
)

project(${LIBRARY_NAME}
	DESCRIPTION "Complex Unit Control Abstraction library (CUCA)"
    VERSION 1.0.0
	LANGUAGES CXX
)

add_library(${LIBRARY_NAME} INTERFACE)
add_library(${GLOBAL_NAMESPACE}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_sources(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/base_unit.cpp)
target_compile_options(${LIBRARY_NAME} INTERFACE -std=c++20 -Wc++20-compat)

target_include_directories(
        ${LIBRARY_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

include(FetchContent)
FetchContent_Declare(
		patterns
		GIT_REPOSITORY https://github.com/xitren/patterns.git
		GIT_TAG        v1.0.3
)
FetchContent_MakeAvailable(patterns)

FetchContent_Declare(
		math
		GIT_REPOSITORY https://github.com/xitren/math.git
		GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(math)

target_link_libraries(${LIBRARY_NAME} INTERFACE ${GLOBAL_NAMESPACE}::CUCA ${GLOBAL_NAMESPACE}::patterns_lib ${GLOBAL_NAMESPACE}::math_lib)

message(STATUS "Adding library project \"${LIBRARY_NAME}\"")

enable_testing()
add_subdirectory(tests)