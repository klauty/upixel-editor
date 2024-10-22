
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
SDL_MouseButtonFlags mouseButtonsState;

struct Pixel canvas[32][32];

struct Pixel primaryActivePalletColor = {
    .color.r = 100,
    .color.g = 0,
    .color.b = 0,
    .color.a = 255
};

struct Pixel secondaryActivePalletColor = {
    .color.r = 0,
    .color.g = 0,
    .color.b = 100,
    .color.a = 255
}; 

struct PalletItem colorPallet[2][8];    

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

void fillCanvasAlternate(){
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
}

void fillCanvasColor(SDL_Color color){
    int flag = 0;
    for(int y = 0; y < 32; y+=1 ){
        for(int x = 0; x < 32; x+=1 ){
            canvas[x][y].rect.x = (x * 10)+rect.x;
            canvas[x][y].rect.y = (y * 10)+rect.y;
            canvas[x][y].rect.h = 10;
            canvas[x][y].rect.w = 10;
            canvas[x][y].color = color;
        }
    }
}

/*TODO - iniciar as cores da paleta baseada em parâmetro*/
void initPallet(SDL_FRect *toolBar){
    for(int y = 0; y < 8; y+=1 ){
        for(int x = 0; x <= 1; x+=1 ){
                colorPallet[x][y].rect.x =(x*17)+toolBar->x+10;
                colorPallet[x][y].rect.y =(y*17)+toolBar->y+80;
                colorPallet[x][y].rect.w = 15;
                colorPallet[x][y].rect.h = 15;
                colorPallet[x][y].color.r = 100;
                colorPallet[x][y].color.g = 0;
                colorPallet[x][y].color.b = 0;
                colorPallet[x][y].color.a = 255;
                SDL_Log("x%f , y%f",colorPallet[x][y].rect.x,colorPallet[x][y].rect.y);
        }
    }
}

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
    mouseCursor.w = mouseCursor.h = 20;

    //posiciona os indicadores de cor primaria e secundaria
    primaryActivePalletColor.rect.x = rectToolBar.x + 10;
    primaryActivePalletColor.rect.y = rectToolBar.y + 10;
    primaryActivePalletColor.rect.h = 40;
    primaryActivePalletColor.rect.w = 40;

    secondaryActivePalletColor.rect.x = primaryActivePalletColor.rect.x + 20;
    secondaryActivePalletColor.rect.y = primaryActivePalletColor.rect.y + 20;
    secondaryActivePalletColor.rect.h = 40;
    secondaryActivePalletColor.rect.w = 40;

    initPallet(&rectToolBar);

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer("Upixel-Editor", 640, 480, 0, &window, &renderer) == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }
    
    
    fillCanvasColor(primaryWhiteColor);
    
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
int SDL_AppEvent(void *appstate, const SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    
    mouseButtonsState = SDL_GetMouseState(&mousePoint.x,&mousePoint.y);
    if(event->type == SDL_EVENT_MOUSE_MOTION){

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
    
    // desenha indicador das cores primaria e secundaria selecionadas
    SDL_SetRenderDrawColor(renderer, secondaryActivePalletColor.color.r,
                                     secondaryActivePalletColor.color.g,
                                     secondaryActivePalletColor.color.b,
                                     secondaryActivePalletColor.color.a);
    SDL_RenderFillRect(renderer,&secondaryActivePalletColor.rect);

    SDL_SetRenderDrawColor(renderer, primaryActivePalletColor.color.r,
                                     primaryActivePalletColor.color.g,
                                     primaryActivePalletColor.color.b,
                                     primaryActivePalletColor.color.a);
    SDL_RenderFillRect(renderer,&primaryActivePalletColor.rect);
    
    //desenha a paleta de cores
    for(int y = 0; y <  8; y+=1 ){
        for(int x = 0; x <= 1; x+=1 ){
            SDL_SetRenderDrawColor(renderer,colorPallet[x][y].color.r,
                                            colorPallet[x][y].color.g,
                                            colorPallet[x][y].color.b,
                                            colorPallet[x][y].color.a);
            
            SDL_RenderFillRect(renderer, &colorPallet[x][y].rect);

        }
    }
    
    // desenha o canvas para o pixel-art, a area de desenho vai ter 32x32 para testes  
    SDL_SetRenderDrawColor(renderer, primaryWhiteColor.r, primaryWhiteColor.g, primaryWhiteColor.b, primaryWhiteColor.a);
    SDL_RenderFillRect(renderer, &rect);

    //desenha o cursor do mouse
    for(int x = 0; x < 32;x+=1){
        for(int y = 0; y < 32; y+=1){
            
            if(SDL_PointInRectFloat(&mousePoint,&canvas[x][y].rect)){
                SDL_SetRenderDrawColor(renderer, activeOrangeColor.r, activeOrangeColor.g, activeOrangeColor.b, activeOrangeColor.a);
                if (mouseButtonsState == SDL_BUTTON_LEFT ){
                    canvas[x][y].color = primaryActivePalletColor.color;
                }

                if (mouseButtonsState-1 == SDL_BUTTON_RIGHT ){
                    canvas[x][y].color = secondaryActivePalletColor.color;
                }

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

