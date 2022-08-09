#include "./client_ctx.h"

#include <rtgn/network_address.h>
#include <string.h>

#include "./config.h"
#include "./game_state.h"

void initClientCtx(ClientCtx* ctx)
{
    const char* err;
    ctx->quit = 0;

    if(rtgn_networkAddress("127.0.0.1", SERVER_PORT, &ctx->srvAddr) < 0) {
        fprintf(stderr, "invalid server address string\n");
        exit(1);
    }
    rtgn_initClient(
        &ctx->networkClient,
        ctx->srvAddr,
        malloc(sizeof(GameState)),
        (rtgn_init_game_state_f)initGameState,
        (rtgn_tick_game_state_f)tickGameState);

    ctx->window = SDL_CreateWindow(
        "top down shooter",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        500, 500,
        SDL_WINDOW_SHOWN);
    if(ctx->window == NULL) {
        err = SDL_GetError();
        fprintf(stderr, "failed to create SDL2 window '%s'\n", err);
        exit(1);
    }
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
    if(ctx->renderer == NULL) {
        err = SDL_GetError();
        fprintf(stderr, "failed to create SDL2 renderer '%s'\n", err);
        exit(1);
    }
    bzero(&ctx->currentInput, sizeof(ctx->currentInput));
}

void destroyClientCtx(ClientCtx* ctx)
{
    rtgn_destroyClient(&ctx->networkClient);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}
