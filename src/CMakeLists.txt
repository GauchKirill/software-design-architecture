set(POS_INCLUDE_ROOT "${PROJECT_SOURCE_DIR}/src")

add_library(pos_domain STATIC
    domain/model/rubricator.cpp
    domain/model/evaluation.cpp
    domain/services/evaluation_factory.cpp
    domain/services/weighted_average_policy.cpp
    domain/services/weighted_points_policy.cpp
    domain/services/rounded_final_score_policy.cpp
    domain/services/final_score_calculator.cpp
)
target_include_directories(pos_domain PUBLIC "${POS_INCLUDE_ROOT}")
target_link_libraries(pos_domain PUBLIC pos_warnings)
add_library(pos::domain ALIAS pos_domain)

add_library(pos_application STATIC
    application/services/student_service.cpp
    application/services/rubricator_service.cpp
    application/services/evaluation_service.cpp
    application/services/final_score_service.cpp
    application/services/report_service.cpp
    application/pos_facade.cpp
)
target_link_libraries(pos_application PUBLIC pos_domain)
add_library(pos::application ALIAS pos_application)

add_library(pos_infrastructure STATIC
    infrastructure/reports/csv_final_score_report.cpp
    infrastructure/reports/json_final_score_report.cpp
    infrastructure/reports/report_factory_impl.cpp
    infrastructure/reports/file_report_writer.cpp
)
target_link_libraries(pos_infrastructure
    PUBLIC pos_application
    PRIVATE Boost::json Boost::filesystem
)
add_library(pos::infrastructure ALIAS pos_infrastructure)

add_library(pos_presentation STATIC
    presentation/console/console_io.cpp
    presentation/console/console_view.cpp
    presentation/console/console_app.cpp
)
target_link_libraries(pos_presentation PUBLIC pos_application Boost::headers)
add_library(pos::presentation ALIAS pos_presentation)

add_library(pos_app STATIC
    app/app_config.cpp
    app/cli.cpp
    app/composition_root.cpp
    app/demo_scenario.cpp
)
target_link_libraries(pos_app
    PUBLIC pos_infrastructure pos_presentation
    PRIVATE Boost::program_options pos::boost_di
)
add_library(pos::app ALIAS pos_app)

add_executable(pos main.cpp)
target_link_libraries(pos PRIVATE pos_app)
set_target_properties(pos PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
