/*
 ============================================================================
 Name        : SDLTest.c
 Author      : EL BAKRAOUI Salim
 Version     : 0.1preAlpha
 Copyright   : Y'en a aucun sers-toi mon pote
 Description : Test et expérimentations avec SDL
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define mBitsSet(f,m)	((f)|=(m))
#define mBitsClr(f,m)	((f)&=(~(m)))
#define mBitsTgl(f,m)	((f)^=(m))
#define mBitsMsk(f,m)	((f)&(m))
#define mIsBitSet(f,m)	(((f)&(m))==(m))
#define mIsBitClr(f,m)	(((~f)&(m)==(m))

#define RECT_A_TOGGLE_MASK	(0x00000001)
#define RECT_B_TOGGLE_MASK	(0x00000002)
#define RECT_C_TOGGLE_MASK	(0x00000004)
#define RECT_D_TOGGLE_MASK	(0x00000008)

#define mRectATgl(f)	mBitsTgl(f, RECT_A_TOGGLE_MASK)
#define mRectBTgl(f)	mBitsTgl(f, RECT_B_TOGGLE_MASK)
#define mRectCTgl(f)	mBitsTgl(f, RECT_C_TOGGLE_MASK)
#define mRectDTgl(f)	mBitsTgl(f, RECT_D_TOGGLE_MASK)

#define mIsRectAColOn(f)	mIsBitSet(f, RECT_A_TOGGLE_MASK)
#define mIsRectBColOn(f)	mIsBitSet(f, RECT_B_TOGGLE_MASK)
#define mIsRectCColOn(f)	mIsBitSet(f, RECT_C_TOGGLE_MASK)
#define mIsRectDColOn(f)	mIsBitSet(f, RECT_D_TOGGLE_MASK)

#define RECT_WIDTH		(300)
#define	RECT_HEIGHT		(300)

#define RECT_SPACING	(30)

#define	RECT_A_X	(RECT_SPACING)
#define	RECT_A_Y	(RECT_SPACING)

#define	RECT_B_X	(RECT_A_X+RECT_WIDTH+RECT_SPACING)
#define	RECT_B_Y	(RECT_A_Y)

#define	RECT_C_X	(RECT_A_X)
#define	RECT_C_Y	(RECT_A_Y+RECT_HEIGHT+RECT_SPACING)

#define	RECT_D_X	(RECT_A_Y+RECT_WIDTH+RECT_SPACING)
#define	RECT_D_Y	(RECT_A_Y+RECT_HEIGHT+RECT_SPACING)

#define WINDOW_WIDTH	(3*RECT_SPACING+2*RECT_WIDTH)
#define WINDOW_HEIGHT	(3*RECT_SPACING+2*RECT_HEIGHT)

void AppMouseButtonUp(SDL_Event*event, int*pStatus);
void AppDraw(SDL_Renderer*pRenderer, int*pStatus);

int main(int argc, char*argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window*pWindow;
	SDL_Renderer*pRenderer;
	SDL_Event pEvent;

	int run = 1;
	int nStatus = 0;

	char buf[256];

	pWindow=SDL_CreateWindow(
			"SDLTest.c",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0);
	if(pWindow==NULL){
		fprintf(stderr, "Unable to initialize SDL Window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	pRenderer=SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	if(pRenderer==NULL){
			fprintf(stderr,"Unable to initialize hardware renderer : %s\n", SDL_GetError());
			pRenderer=SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_SOFTWARE);
			if(pRenderer==NULL){
				fprintf(stderr,"Unable to initialize software renderer: %s\n",SDL_GetError());
				return EXIT_FAILURE;
			}
		}

	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(pRenderer);
	SDL_Delay(300);
	SDL_RenderPresent(pRenderer);

	while(run){
		while(SDL_PollEvent(&pEvent)){
			switch(pEvent.type){
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONUP:
				sprintf(buf,"Mouse cursor coordinates - X:%03d Y:%03d", pEvent.button.x, pEvent.button.y);
				SDL_SetWindowTitle(pWindow, buf);
				AppMouseButtonUp(&pEvent, &nStatus);
				AppDraw(pRenderer, &nStatus);
				break;
			case SDL_WINDOWEVENT:
				switch(pEvent.window.event){
				case SDL_WINDOWEVENT_SHOWN:
					break;
				case SDL_WINDOWEVENT_CLOSE:
					pEvent.type = SDL_QUIT;
					SDL_PushEvent(&pEvent);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				run=0;
				break;
			case SDL_KEYUP:
				switch(pEvent.key.keysym.sym){
				case SDLK_ESCAPE:
					run=0;
					break;
				case SDLK_q:
					run=0;
					break;
				case SDLK_SPACE:
					AppDraw(pRenderer, &nStatus);
				break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}


		SDL_DestroyRenderer(pRenderer);
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
		return EXIT_SUCCESS;
	}



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
	if(mIsRectAColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 155, 0, 255, 255);
	else SDL_SetRenderDrawColor(pRenderer, 0, 155, 255, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_B_X ;
	rect.y = RECT_B_Y;
	if(mIsRectBColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 155, 0, 255, 255);
	else SDL_SetRenderDrawColor(pRenderer, 0, 155, 255, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_C_X ;
	rect.y = RECT_C_Y;
	if(mIsRectCColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 155, 0, 255, 255);
	else SDL_SetRenderDrawColor(pRenderer, 0, 155, 255, 255);
	SDL_RenderFillRect(pRenderer, &rect);
	rect.x = RECT_D_X ;
	rect.y = RECT_D_Y;
	if(mIsRectDColOn(*pStatus))SDL_SetRenderDrawColor(pRenderer, 155, 0, 255, 255);
	else SDL_SetRenderDrawColor(pRenderer, 0, 155, 255, 255);
	SDL_RenderFillRect(pRenderer, &rect);

	SDL_RenderPresent(pRenderer);

}

