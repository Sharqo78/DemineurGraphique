#include "defines.h"
#include "deminGraphLib.h"
#include <SDL2/SDL.h>

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

