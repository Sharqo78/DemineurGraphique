#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

int* OnClickCellCoordinates(SDL_Event*event, int* pS, int nRow, int nCol);
int	DiscoverCell(int* pS, int x, int y, int nRow, int nCol);
void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent);
void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode, TTF_Font *pFont);
int AppIsMouseOnCell(int xM, int yM, int nRow, int nCol);
void DeminDrawNumber(int number, SDL_Renderer *pRenderer, int x, int y, TTF_Font *pFont);
void DeminDrawMessage(SDL_Renderer *pRenderer, TTF_Font *pFont, char *text);