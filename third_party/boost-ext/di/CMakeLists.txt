add_library(pos_boost_di INTERFACE)
add_library(pos::boost_di ALIAS pos_boost_di)

if(POS_BOOST_DI_INCLUDE_DIR)
    set(POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR "${POS_BOOST_DI_INCLUDE_DIR}")
else()
    set(POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()

target_include_directories(pos_boost_di SYSTEM INTERFACE
    "${POS_BOOST_DI_EFFECTIVE_INCLUDE_DIR}")
target_compile_features(pos_boost_di INTERFACE cxx_std_17)
