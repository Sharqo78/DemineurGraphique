#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

void AppMouseButtonUp(SDL_Event*event, int*pStatus);
void AppDraw(SDL_Renderer*pRenderer, int*pStatus);
void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent);
void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode);