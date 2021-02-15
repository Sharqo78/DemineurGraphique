/*
 ================================================================================
 Name        : deminGraph.c
 Author      : EL BAKRAOUI Salim
 Version     : 0.2prePrePreAlphaAlpha
 Copyright   : Y'en a aucun sers-toi mon pote
 Description : Portage de notre demineur en terminal vers un demineur graphique
 ================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "defines.h"
#include "deminGraphLib.h"


int main(int argc, char*argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window*pWindow;
	SDL_Renderer*pRenderer;
	SDL_Event pEvent;

	int run = 1;
	int nStatus = 0;

	char buf[256];

	pWindow=SDL_CreateWindow(
			"\"deminGraph.c\"",
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
	SDL_Delay(100);
	SDL_RenderPresent(pRenderer);
	SDL_Delay(100);
	AppDraw(pRenderer,&nStatus);
	SDL_Delay(100);
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


