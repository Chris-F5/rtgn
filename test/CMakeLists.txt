cmake_minimum_required(VERSION 3.10)

add_executable(tests
    runner.c
    tests/test_temp.c
    )

target_link_libraries(tests PRIVATE rtgn)

add_custom_target(test COMMAND tests DEPENDS tests)
