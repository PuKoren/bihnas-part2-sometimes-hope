#ifndef _GAMEOVER_H
#define _GAMEOVER_H
#if PSP
#include <pspctrl.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include "config.h"

class Gameover{
    private:
        TTF_Font* menuFont;
        SDL_Surface* gameOverTexture;

    public:
        Gameover();
        ~Gameover();
        void Update(Uint32);
        void Event(SDL_Event*, GAME_STATE*);
        #if PSP
        void Event(SceCtrlData*, SDL_Event*, GAME_STATE*);
        #endif
        void Draw(SDL_Surface*);
};

#endif
