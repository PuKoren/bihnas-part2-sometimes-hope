#include "Gameover.h"

Gameover::Gameover(){
    this->menuFont = TTF_OpenFont("visitor2.ttf", SCREEN_WIDTH/10);
    this->gameOverTexture = TTF_RenderUTF8_Solid(this->menuFont, "GAME OVER", WHITE);
}

Gameover::~Gameover(){
    if(this->gameOverTexture != NULL){
        SDL_FreeSurface(this->gameOverTexture);
    }
    TTF_CloseFont(this->menuFont);
}

void Gameover::Event(SDL_Event* ev, GAME_STATE* gs){
    if(ev->type == SDL_KEYDOWN){
        if(ev->key.keysym.sym == SDLK_ESCAPE){
            *gs = MENU;
        }else if(ev->key.keysym.sym == SDLK_RETURN){
            *gs = MENU;
        }
    }
}

#if PSP
void Gameover::Event(SceCtrlData* pad, SDL_Event* ev, GAME_STATE* gs){
    switch(pad->Buttons)
    {
        case PSP_CTRL_START:
            *gs = MENU;
            break;
        case PSP_CTRL_CROSS:
             *gs = MENU;
             break;
        default:
            break;
    }
}
#endif

void Gameover::Update(Uint32 gameTime){

}

void Gameover::Draw(SDL_Surface* viewport){
    SDL_Rect destR;
    destR.x = SCREEN_HEIGHT / 3;
    destR.y = SCREEN_HEIGHT / 2;
    destR.w = this->gameOverTexture->w;
    destR.h = this->gameOverTexture->h;
    SDL_BlitSurface(this->gameOverTexture, NULL, viewport, &destR);
}
