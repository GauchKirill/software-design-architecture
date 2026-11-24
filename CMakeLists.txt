cmake_minimum_required(VERSION 3.24)

project(pos
    VERSION 1.0.0
    DESCRIPTION "POS - teacher evaluation of students (DDD/SOLID/GoF demo)"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(POS_BUILD_TESTS "Build unit and integration tests" ON)
option(POS_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(POS_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(POS_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
set(POS_BOOST_DI_INCLUDE_DIR "" CACHE PATH
    "Optional directory that contains boost/di.hpp (defaults to the vendored copy)")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PosHelpers)
include(PosBoost)
include(PosTesting)

if(POS_ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()
if(POS_ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=undefined)
endif()

add_subdirectory(third_party/boost-ext/di)
add_subdirectory(src)

if(POS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(POS_BOOST_DI_INCLUDE_DIR)
    set(POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR "${POS_BOOST_DI_INCLUDE_DIR}")
else()
    set(POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-ext/di/include")
endif()

message(STATUS "POS: C++ standard ${CMAKE_CXX_STANDARD}")
message(STATUS "POS: Boost headers ${POS_BOOST_HEADER_VERSION_FULL}, libraries ${Boost_VERSION_STRING}")
message(STATUS "POS: Boost.DI include: ${POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR}")
