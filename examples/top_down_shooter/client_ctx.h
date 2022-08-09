#ifndef CLIENT_CTX_H
#define CLIENT_CTX_H

#include <SDL2/SDL.h>
#include <rtgn/common_types.h>
#include <rtgn/client.h>

typedef struct ClientCtx ClientCtx;
struct ClientCtx
{
    int quit;
    rtgn_networkAddress_t srvAddr;
    rtgn_Client networkClient;
    SDL_Window* window;
    SDL_Renderer* renderer;
    rtgn_Input currentInput;
};

void initClientCtx(ClientCtx* ctx);
void destroyClientCtx(ClientCtx* ctx);

#endif
