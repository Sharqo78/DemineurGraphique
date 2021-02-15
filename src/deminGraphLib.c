#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "defines.h"
#include "deminGraphLib.h"


void AppMouseButtonUp(SDL_Event*event, int*pStatus){
	int xM, yM;
	xM = event->button.x;
	yM = event->button.y;
	//Rect A----------
	if((xM>=RECT_A_X)&&
	   (xM<RECT_A_X+RECT_WIDTH)&&
	   (yM>=RECT_A_Y)&&
	   (yM<RECT_A_Y+RECT_HEIGHT)){
		mRectATgl(*pStatus);
		return;
	}
	//Rect B----------
	if((xM>=RECT_B_X)&&
	   (xM<RECT_B_X+RECT_WIDTH)&&
	   (yM>=RECT_B_Y)&&
	   (yM<RECT_B_Y+RECT_HEIGHT)){
		mRectBTgl(*pStatus);
		return;
	}
	//Rect C----------
	if((xM>=RECT_C_X)&&
	   (xM<RECT_C_X+RECT_WIDTH)&&
	   (yM>=RECT_C_Y)&&
	   (yM<RECT_C_Y+RECT_HEIGHT)){
		mRectCTgl(*pStatus);
		return;
	}
	//Rect D----------
	if((xM>=RECT_D_X)&&
	   (xM<RECT_D_X+RECT_WIDTH)&&
	   (yM>=RECT_D_Y)&&
	   (yM<RECT_D_Y+RECT_HEIGHT)){
		mRectDTgl(*pStatus);
		return;
	}
}
void AppDraw(SDL_Renderer*pRenderer, int*pStatus){
	SDL_Rect rect={0,0,RECT_WIDTH,RECT_HEIGHT};
	rect.x = RECT_A_X ;
	rect.y = RECT_A_Y;
	if(mIsRectAColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 0, 255, 155, 255);
	else SDL_SetRenderDrawColor(pRenderer, 96, 116, 189, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_B_X ;
	rect.y = RECT_B_Y;
	if(mIsRectBColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer,0 , 255, 155, 255);
	else SDL_SetRenderDrawColor(pRenderer, 96, 116, 189, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_C_X ;
	rect.y = RECT_C_Y;
	if(mIsRectCColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 0, 255, 155, 255);
	else SDL_SetRenderDrawColor(pRenderer, 96, 116, 189, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_D_X ;
	rect.y = RECT_D_Y;
	if(mIsRectDColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 0, 255, 155, 255);
	else SDL_SetRenderDrawColor(pRenderer, 96, 116, 189, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	SDL_RenderPresent(pRenderer);
}

void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent){
    int k,m;
    int x,y;
    int nbMines = (nRow*nCol*nPercent)/100;

    //Mise de toutes les valeurs à 0
    for(k=0;k<(nCol*nRow);k++)pS[k]=0;
    
    //pS[0]=CELL_MINE;
    //pS[nCol-1]=CELL_MINE;
    //pS[nCol*(nRow-1)]=CELL_MINE;
    //pS[nCol*nRow-1]=CELL_MINE;

    //Insertion des mines ainsi que des valeurs adjacentes permettant de détecter les mines
    do{
        k=rand()%nRow;
        m=rand()%nCol;
        pS[k+m*nRow]=CELL_MINE;
        for(x=-1;x<=1;x++){
            for(y=-1;y<=1;y++){
                if(
                (k+x>=0) && 
                (k+x<nRow) && 
                (m+y>=0) &&
                (m+y<nCol) && 
                (pS[k+x+((m+y)*nRow)]!=CELL_MINE)) pS[k+x+((m+y)*nRow)]++;
                }
            }
    }while(--nbMines);

}

void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode){
    SDL_Rect r = {};
    int k,m;
    r.w = SCENE_CELL_SIZE;
    r.h = SCENE_CELL_SIZE;
    r.x=PADDING_HRZ;
    r.y=PADDING_TOP;
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    if(mode){
        for(k=0;k<nRow;k++){
			for(m=0;m<nCol;m++){
				if(mCellValue(pS[k+m*nRow])>=CELL_MINE){
					SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
					SDL_RenderFillRect(pRenderer, &r);
				}
				else if(mCellValue(pS[k+m*nRow])==CELL_VOID){
					SDL_SetRenderDrawColor(pRenderer, 64, 69, 82, 255);
					SDL_RenderFillRect(pRenderer, &r);
				}
				else{
					SDL_SetRenderDrawColor(pRenderer, 82, 148, 255, pS[k+m*nRow]*26);
					SDL_RenderFillRect(pRenderer, &r);
				}
				r.x+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;
			}
			r.x=PADDING_HRZ;
			r.y+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;

    	}
    }
    else{

    }
    SDL_RenderPresent(pRenderer);
}