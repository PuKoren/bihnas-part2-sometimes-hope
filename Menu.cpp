#include "Menu.h"

Menu::Menu(){
    this->movingRight = true;
    this->charSize = SCREEN_WIDTH/15;
    this->animationDelay = 20;
    this->animationCurrent = 0;
    this->selectedMenuIndex = 0;
    this->selectedMenuPosition = 0;
    this->previousAction = -1;
    this->selectedMenuMaxPosition = SCREEN_WIDTH/25;
    this->menuItems.push_back("Continue");
    this->menuItems.push_back("Start");
    this->menuItems.push_back("Exit");
    this->menuFont = TTF_OpenFont("visitor2.ttf", this->charSize);
    for(unsigned int i = 0; i < this->menuItems.size(); i++){
        this->menuSurfaces.push_back(TTF_RenderUTF8_Solid(this->menuFont, this->menuItems[i].c_str(), WHITE));
    }
	this->menuTitle = TTF_RenderUTF8_Solid(this->menuFont, "Part 2 - Sometimes, Hope", WHITE);
}

Menu::~Menu(){
    for(unsigned int i = 0; i < this->menuItems.size(); i++){
        SDL_FreeSurface(this->menuSurfaces[i]);
    }
	SDL_FreeSurface(this->menuTitle);
    if(this->menuFont != NULL){
        TTF_CloseFont(this->menuFont);
        this->menuFont = NULL;
    }
}

void Menu::Event(SDL_Event* ev, GAME_STATE* gs){
    if(ev->type == SDL_KEYDOWN){
        if(ev->key.keysym.sym == SDLK_DOWN){
            this->selectedMenuIndex = ((unsigned int)this->selectedMenuIndex+1 > this->menuItems.size()-1)?0:this->selectedMenuIndex+1;
        }else if(ev->key.keysym.sym == SDLK_UP){
            this->selectedMenuIndex = (this->selectedMenuIndex-1 < 0)?this->menuItems.size()-1:this->selectedMenuIndex-1;
        }else if(ev->key.keysym.sym == SDLK_RETURN){
            if(this->selectedMenuIndex == 0){
                *gs = (*gs == PAUSE)?INGAME:NEWGAME;
            }else if(this->selectedMenuIndex == 1){
                *gs = NEWGAME;
            }else{
                ev->type = SDL_QUIT;
            }
            this->selectedMenuIndex = 0;
        }else if(ev->key.keysym.sym == SDLK_ESCAPE){
            ev->type = SDL_QUIT;
        }
        this->selectedMenuPosition = 0;
    }
}

#if PSP
void Menu::Event(SceCtrlData* pad, SDL_Event* ev, GAME_STATE* gs){
    if(pad->Buttons != 0)
    {
        if(this->previousAction < 0 && (pad->Buttons & PSP_CTRL_START || pad->Buttons & PSP_CTRL_CROSS)){
            if(this->selectedMenuIndex == 0){
                *gs = (*gs == PAUSE)?INGAME:NEWGAME;
            }else if(this->selectedMenuIndex == 1){
                *gs = NEWGAME;
                this->selectedMenuIndex = 0;
            }else{
                ev->type = SDL_QUIT;
            }
            this->previousAction = 2;
        }else if(pad->Buttons & PSP_CTRL_UP && this->previousAction != 0){
            this->selectedMenuIndex = (this->selectedMenuIndex-1 < 0)?this->menuItems.size()-1:this->selectedMenuIndex-1;
            this->previousAction = 0;
        }else if(pad->Buttons & PSP_CTRL_DOWN && this->previousAction != 1){
            this->selectedMenuIndex = ((unsigned int)this->selectedMenuIndex+1 > this->menuItems.size()-1)?0:this->selectedMenuIndex+1;
            this->previousAction = 1;
        }else if(pad->Buttons & PSP_CTRL_SELECT){
            ev->type = SDL_QUIT;
        }
    }else{
        this->previousAction = -1;
    }
}
#endif

void Menu::Update(Uint32 gameTime){
    this->animationCurrent += gameTime;
    if(this->animationCurrent >= this->animationDelay){
        if(this->movingRight){
            this->selectedMenuPosition += 1;
            if(this->selectedMenuPosition >= this->selectedMenuMaxPosition){
                this->movingRight = false;
            }
        }else{
            this->selectedMenuPosition -= 1;
            if(this->selectedMenuPosition <= 0){
                this->movingRight = true;
            }
        }
        this->animationCurrent -= this->animationDelay;
    }
}

void Menu::Draw(SDL_Surface* viewport){
    int y_spacing = this->charSize/2;
    int y_align = SCREEN_HEIGHT/2;
    int x_align = SCREEN_WIDTH/3;
    for(unsigned int i = 0; i < this->menuSurfaces.size(); i ++){
        SDL_Rect destR;
        if((unsigned)this->selectedMenuIndex == i){
            destR.x = x_align + this->selectedMenuPosition;
        }else{
            destR.x = x_align;
        }
        destR.y = y_align + y_spacing*i;
        SDL_BlitSurface(this->menuSurfaces[i], NULL, viewport, &destR);
    }
	SDL_Rect destR;
	destR.x = 10;
	destR.y = SCREEN_HEIGHT/4;
	destR.w = this->menuTitle->w;
	destR.h = this->menuTitle->h;
	SDL_BlitSurface(this->menuTitle, NULL, viewport, &destR);
}
