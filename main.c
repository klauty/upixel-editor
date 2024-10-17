
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

typedef struct PalletItem
{
    SDL_FRect rect;
    SDL_FRect border;
    SDL_Color color;
    SDL_bool active;
} PalletItem;

typedef struct Pixel
{
    SDL_FRect rect;
    SDL_Color color;
}Pixel;


/* We will use this renderer to draw into this window every frame. */
static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
SDL_FRect rect;
SDL_FRect rectToolBar;

SDL_FRect mouseCursor;
float mouseX = 0;
float mouseY = 0;
SDL_FPoint mousePoint;

struct Pixel canvas[32][32];

SDL_Color primaryCanvasColor = {
    .r = 128,
    .g = 128,
    .b = 128,
    .a = 255
};

SDL_Color secondaryCanvasColor = {
    .r = 64,
    .g = 64,
    .b = 64,
    .a = 255
};

SDL_Color primaryWhiteColor = {
    .r = 238,
    .g = 238,
    .b = 238,
    .a = 255
};

SDL_Color secondaryGrayColor = {
    .r = 104,
    .g = 109,
    .b = 118,
    .a = 255
};

SDL_Color darkGrayColor = {
    .r = 55,
    .g = 58,
    .b = 64,
    .a = 255
};

SDL_Color activeOrangeColor = {
    .r = 220,
    .g = 95,
    .b = 0,
    .a = 255
};

SDL_Color activePalletColor = {
    .r = 100,
    .g = 0,
    .b = 0,
    .a = 255
};

/* This function runs once at startup. */
int SDL_AppInit(void **appstate, int argc, char *argv[])
{
    rect.x = 200; 
    rect.y = 100;
    rect.w = 320;
    rect.h = 320;

    rectToolBar.x = 0;
    rectToolBar.y = 0;
    rectToolBar.h = 300;
    rectToolBar.w = 50;

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer("Upixel-Editor", 640, 480, 0, &window, &renderer) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }
    
    mouseCursor.w = mouseCursor.h = 20;
    int flag = 0;
    for(int y = 0; y < 32; y+=1 ){
        for(int x = 0; x < 32; x+=1 ){
            canvas[x][y].rect.x = (x * 10)+rect.x;
            canvas[x][y].rect.y = (y * 10)+rect.y;
            canvas[x][y].rect.h = 10;
            canvas[x][y].rect.w = 10;
            if(flag){
                canvas[x][y].color = primaryCanvasColor;
            }else{
                canvas[x][y].color = secondaryCanvasColor;
                canvas[x][y].color.a = 128;
            }
            flag = !flag;
        }
        flag = !flag;
    }


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
    	
	/* cor de fundo */
    SDL_SetRenderDrawColor(renderer, secondaryGrayColor.r, secondaryGrayColor.g, secondaryGrayColor.b, secondaryGrayColor.a);
    SDL_RenderClear(renderer);

    // desenha fundo barra de ferramentas
    SDL_SetRenderDrawColor(renderer, darkGrayColor.r, darkGrayColor.g, darkGrayColor.b, darkGrayColor.a);
    SDL_RenderFillRect(renderer,&rectToolBar);

    // desenha o canvas para o pixel-art, a area de desenho vai ter 32x32 para testes  
    SDL_SetRenderDrawColor(renderer, primaryWhiteColor.r, primaryWhiteColor.g, primaryWhiteColor.b, primaryWhiteColor.a);
    SDL_RenderFillRect(renderer, &rect);

    //desenha o cursor do mouse
    for(int x = 0; x < 32;x+=1){
        for(int y = 0; y < 32; y+=1){
            
            if(SDL_PointInRectFloat(&mousePoint,&canvas[x][y].rect)){
                SDL_SetRenderDrawColor(renderer, activeOrangeColor.r, activeOrangeColor.g, activeOrangeColor.b, activeOrangeColor.a);

            }else{
                SDL_SetRenderDrawColor(renderer, canvas[x][y].color.r, canvas[x][y].color.g, canvas[x][y].color.b, canvas[x][y].color.a);
            }

            SDL_RenderFillRect(renderer,&canvas[x][y].rect);
        }
    }


    SDL_RenderPresent(renderer);  /* put it all on the screen! */
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate)
{
    /* SDL will clean up the window/renderer for us. */
}

