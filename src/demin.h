#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

int* OnClickCellCoordinates(SDL_Event*event, int* pS, int nRow, int nCol);
int	DiscoverCell(int* pS, int x, int y, int nRow, int nCol);
void AppDraw(SDL_Renderer*pRenderer, int*pStatus);
void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent);
void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode);
