#ifndef _MENU_H
#define _MENU_H
#if PSP
#include <pspctrl.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "config.h"

class Menu{
    private:
        int selectedMenuIndex;
        int selectedMenuPosition;
        int selectedMenuMaxPosition;
        int charSize;
        int animationDelay;
        int animationCurrent;
        bool movingRight;
        TTF_Font* menuFont;
		SDL_Surface* menuTitle;
        std::vector<std::string> menuItems;
        std::vector<SDL_Surface*> menuSurfaces;
        int previousAction;
    public:
        Menu();
        ~Menu();
        void Update(Uint32);
        #if PSP
        void Event(SceCtrlData*, SDL_Event*, GAME_STATE*);
        #endif
        void Event(SDL_Event*, GAME_STATE*);
        void Draw(SDL_Surface*);
};

#endif
