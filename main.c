
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
SDL_FRect mouseCursor;
float mouseX = 0;
float mouseY = 0;
SDL_FPoint mousePoint;



/* This function runs once at startup. */
int SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer("Upixel-Editor", 640, 480, 0, &window, &renderer) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }
    
    mouseCursor.w = mouseCursor.h = 20;
    

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
int SDL_AppEvent(void *appstate, const SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if(event->type == SDL_EVENT_MOUSE_MOTION){
        SDL_GetMouseState(&mousePoint.x,&mousePoint.y);

        //SDL_Log("x %f,y %f ",mouseX, mouseY);
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
int SDL_AppIterate(void *appstate)
{
	SDL_FRect rect;
    SDL_FRect rectToolBar;

    rectToolBar.x = 0;
    rectToolBar.y = 0;
    rectToolBar.h = 300;
    rectToolBar.w = 50;
	
	/* cor de fundo */
    SDL_SetRenderDrawColor(renderer, 104, 109, 118, 255);
    SDL_RenderClear(renderer);

    // desenha fundo barra de ferramentas
    SDL_SetRenderDrawColor(renderer, 55, 58, 64, 255);
    SDL_RenderFillRect(renderer,&rectToolBar);

    // desenha o canvas para o pixel-art, a area de desenho vai ter 32x32 para testes  
    SDL_SetRenderDrawColor(renderer, 238, 238, 238, 255);
    rect.x = 200; 
    rect.y = 100;
    rect.w = 280;
    rect.h = 280;
    SDL_RenderFillRect(renderer, &rect);

    //desenha o cursor do mouse
    mouseCursor.x = mousePoint.x;
    mouseCursor.y = mousePoint.y;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer,&mouseCursor);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate)
{
    /* SDL will clean up the window/renderer for us. */
}

