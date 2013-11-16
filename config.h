#ifndef _CONFIG_H
#define _CONFIG_H

enum GAME_STATE{
    MENU,
    INGAME,
    GAMEOVER,
    PAUSE,
    NEWGAME
};

#if PSP
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;
#else
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
#endif

const SDL_Color WHITE = {255,255,255};
const SDL_Color RED = {255,0,0};
const SDL_Color BLUE = {0,0,255};
const SDL_Color GREEN = {0,255,0};
const SDL_Color YELLOW = {255,255,51};

#endif