cmake_minimum_required(VERSION 3.10)

add_executable(top_down_shooter
    client.c
    )

add_custom_target(assets
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
    )
add_dependencies(top_down_shooter assets)

target_link_libraries(top_down_shooter PRIVATE rtgn)
