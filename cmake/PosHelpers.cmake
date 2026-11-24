add_library(pos_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pos_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )
    if(POS_WARNINGS_AS_ERRORS)
        target_compile_options(pos_warnings INTERFACE -Werror)
    endif()
elseif(MSVC)
    target_compile_options(pos_warnings INTERFACE /W4)
    if(POS_WARNINGS_AS_ERRORS)
        target_compile_options(pos_warnings INTERFACE /WX)
    endif()
endif()
