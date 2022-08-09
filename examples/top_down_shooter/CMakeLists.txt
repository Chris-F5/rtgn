cmake_minimum_required(VERSION 3.10)

add_executable(top_down_shooter_client
    client.c
    client_ctx.c
    game_state.c
    window_event_handler.c
    )

add_executable(top_down_shooter_server
    server.c
    )

add_custom_target(assets
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
    )
add_dependencies(top_down_shooter_client assets)

target_link_libraries(top_down_shooter_client PRIVATE rtgn SDL2)
target_link_libraries(top_down_shooter_server PRIVATE rtgn)
