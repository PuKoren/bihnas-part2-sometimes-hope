#if PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h> 
#endif
#include <SDL.h>
#include "config.h"
#include "Menu.h"
#include "Gameover.h"
#include "Ingame.h"

bool running = true;

#if PSP
// De rappel de service de sortie
int exit_callback(int arg1, int arg2, void *common)
{
 running = false;
 sceKernelExitGame();
 return 0;
}


// Rappel de service
int CallbackThread(SceSize args, void *argp)
{
 int cbid;
 cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
 sceKernelRegisterExitCallback(cbid);
 
 sceKernelSleepThreadCB();
 
 return 0;
}


// Monte le Rappel de service et envoi son ID
int SetupCallbacks(void)
{
 int thid = 0;
 thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
 
 if (  thid >= 0 )
 {
  sceKernelStartThread(thid, 0, 0);
 }
 
 return thid;
}

SceCtrlData pad; 

extern "C" 
#endif
int main(int argc, char** argv){
	#if PSP
    pspDebugScreenInit();
    SetupCallbacks();
    #endif

    if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1 || TTF_Init() == -1){
        return 0;
    }
	
	#if PSP
	SDL_Surface* viewport = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF);
	#else
	SDL_Surface* viewport = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	#endif
    SDL_WM_SetCaption("Part 2 - Sometimes, Hope", NULL);


    GAME_STATE gs = MENU;
    Menu* menu = new Menu();
    Ingame* ingame = new Ingame();
    Gameover* gameover = new Gameover();
    SDL_Event ev;

    int previousTicks = SDL_GetTicks();
    while(running){
        int elapsedTime = SDL_GetTicks() - previousTicks;
        SDL_FillRect(viewport, NULL, 0x0);

        while(SDL_PollEvent(&ev)){
            switch(gs){
                case INGAME:
                    ingame->Event(&ev, &gs);
                    break;
                case GAMEOVER:
                    gameover->Event(&ev, &gs);
                    break;
                case PAUSE:
                    menu->Event(&ev, &gs);
                    break;
                default:
                    menu->Event(&ev, &gs);
                    break;
            };
        }
		#if PSP
        sceCtrlReadBufferPositive(&pad, 1); 
        switch(gs){
            case INGAME:
                ingame->Event(&pad, &ev, &gs);
                break;
            case GAMEOVER:
                gameover->Event(&pad, &ev, &gs);
                break;
            case PAUSE:
                menu->Event(&pad, &ev, &gs);
                break;
            default:
                menu->Event(&pad, &ev, &gs);
                break;
        };
        #endif


        if(ev.type == SDL_QUIT){
            running = false;
        }else if(gs == NEWGAME){
            gs = INGAME;
            ingame->Init();
        }

        switch(gs){
            case INGAME:
                ingame->Update(elapsedTime, &gs);
                ingame->Draw(viewport);
                break;
            case GAMEOVER:
                gameover->Update(elapsedTime);
                ingame->Draw(viewport);
                gameover->Draw(viewport);
                break;
            case PAUSE:
                ingame->Draw(viewport);
                menu->Draw(viewport);
            default:
                menu->Update(elapsedTime);
                menu->Draw(viewport);
                break;
        };

        SDL_Flip(viewport);
        previousTicks = previousTicks + elapsedTime;
    }

    SDL_FreeSurface(viewport);

    delete menu;
    delete ingame;
    delete gameover;
    
    TTF_Quit();
    SDL_Quit();
    #if PSP
    sceKernelExitGame();
    #endif
    return 0;
}
