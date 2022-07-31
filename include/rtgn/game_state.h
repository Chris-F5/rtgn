#ifndef RTGN_GAME_STATE_H
#define RTGN_GAME_STATE_H

typedef void rtgn_game_state_t;
typedef void (*rtgn_tick_game_state_f)(rtgn_game_state_t* gameState);
typedef void (*rtgn_init_game_state_f)(rtgn_game_state_t* gameState);

#endif
