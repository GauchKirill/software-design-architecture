find_package(GTest 1.14 CONFIG QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)

function(pos_add_test target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE GTest::gtest_main pos_warnings)
    target_include_directories(${target} PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/tests"
    )
    gtest_discover_tests(${target} DISCOVERY_TIMEOUT 60)
endfunction()
