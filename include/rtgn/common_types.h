#ifndef RTGN_COMMON_TYPES_H
#define RTGN_COMMON_TYPES_H

#include <inttypes.h>

/* INPUT */

#define RTGN_INPUT_FLAG_LEFT   (1<<0)
#define RTGN_INPUT_FLAG_RIGHT  (1<<1)

typedef struct rtgn_Input rtgn_Input;
struct rtgn_Input
{
    uint32_t inputFlags;
};

/* GAME STATE */

typedef void rtgn_game_state_t;
typedef void (*rtgn_init_game_state_f)(rtgn_game_state_t* gameState, int playerCount);
typedef void (*rtgn_tick_game_state_f)(rtgn_game_state_t* gameState, rtgn_Input* playerInputs);

#endif
