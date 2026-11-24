pos_add_test(domain_tests
    domain/score_test.cpp
    domain/student_test.cpp
    domain/criterion_test.cpp
    domain/rubricator_test.cpp
    domain/evaluation_test.cpp
    domain/final_score_policy_test.cpp
    domain/final_score_calculator_test.cpp
)
target_link_libraries(domain_tests PRIVATE pos_domain)

pos_add_test(application_tests
    application/student_service_test.cpp
    application/evaluation_service_test.cpp
    application/final_score_service_test.cpp
    application/report_service_test.cpp
)
target_link_libraries(application_tests PRIVATE
    pos_application pos_infrastructure Boost::json)

pos_add_test(infrastructure_tests
    infrastructure/report_test.cpp
)
target_link_libraries(infrastructure_tests PRIVATE pos_infrastructure Boost::json)

pos_add_test(app_tests
    app/composition_root_test.cpp
    app/demo_scenario_test.cpp
)
target_link_libraries(app_tests PRIVATE pos_app pos_infrastructure Boost::json)
