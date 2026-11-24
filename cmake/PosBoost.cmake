# Определяем версию Boost по заголовкам, которые реально увидит компилятор,
# и требуем ровно такие же библиотеки. Это защищает от классической ошибки
# "заголовки 1.90 + .so 1.83" (несовместимый ABI внутри boost::function).
execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -std=c++23 -x c++ -E -dM
            -include boost/version.hpp /dev/null
    OUTPUT_VARIABLE _pos_boost_defines
    ERROR_QUIET
    RESULT_VARIABLE _pos_boost_result
)

if(NOT _pos_boost_result EQUAL 0)
    message(FATAL_ERROR
        "Не удалось определить версию Boost по заголовку boost/version.hpp. "
        "Убедитесь, что Boost установлен и доступен компилятору.")
endif()

string(REGEX MATCH "BOOST_VERSION ([0-9]+)" _pos_boost_match "${_pos_boost_defines}")
if(NOT _pos_boost_match)
    message(FATAL_ERROR "В boost/version.hpp не найден макрос BOOST_VERSION.")
endif()
set(POS_BOOST_VERSION_MACRO "${CMAKE_MATCH_1}")

math(EXPR POS_BOOST_MAJOR "${POS_BOOST_VERSION_MACRO} / 100000")
math(EXPR POS_BOOST_MINOR "(${POS_BOOST_VERSION_MACRO} / 100) % 1000")
math(EXPR POS_BOOST_PATCH "${POS_BOOST_VERSION_MACRO} % 100")
set(POS_BOOST_HEADER_VERSION "${POS_BOOST_MAJOR}.${POS_BOOST_MINOR}")
set(POS_BOOST_HEADER_VERSION_FULL
    "${POS_BOOST_MAJOR}.${POS_BOOST_MINOR}.${POS_BOOST_PATCH}")

# EXACT критичен: в системе могут лежать CMake-конфиги другой версии Boost
# (например, отключённые через *.disabled). Без EXACT CMake может выбрать 1.90
# под заголовки 1.83 и получить ABI-конфликт.
find_package(Boost ${POS_BOOST_HEADER_VERSION_FULL} EXACT CONFIG REQUIRED
    COMPONENTS program_options json filesystem)

if(DEFINED Boost_VERSION_MACRO AND NOT "${Boost_VERSION_MACRO}" STREQUAL "${POS_BOOST_VERSION_MACRO}")
    message(FATAL_ERROR
        "BOOST VERSION MISMATCH: заголовки Boost ${POS_BOOST_HEADER_VERSION_FULL} "
        "(BOOST_VERSION=${POS_BOOST_VERSION_MACRO}), а найденные библиотеки "
        "${Boost_VERSION_STRING} (Boost_VERSION_MACRO=${Boost_VERSION_MACRO}).\n"
        "Нельзя смешивать разные версии Boost. Уберите лишнюю установку из "
        "include/library путей или укажите Boost_DIR/Boost_ROOT явно.")
endif()

if(NOT TARGET Boost::program_options OR NOT TARGET Boost::json OR NOT TARGET Boost::filesystem)
    message(FATAL_ERROR
        "CMake-конфиг Boost ${Boost_VERSION_STRING} не предоставил ожидаемые цели "
        "Boost::program_options, Boost::json, Boost::filesystem.")
endif()
