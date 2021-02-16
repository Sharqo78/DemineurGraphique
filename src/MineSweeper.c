/*
 ===============================================================================================
 Name        : deminGraph.c
 Author      : EL BAKRAOUI Salim
 Version     : 0.2prePrePreAlphaAlpha
 Copyright   : Aucun copyright, par contre ça veut pas dire qu'il faut plagier mon boulot
 Description : Portage de notre demineur en terminal vers un demineur graphique
 ===============================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> //J'ai pu remarquer que peu importe l'OS, l'include avec un / fonctionne (peut-être grâce à MinGW qui sait)
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "defines.h"
#include "demin.h"

struct s_app{
	Uint32			uStatus;
	SDL_Window		*pWindow;
	SDL_Renderer	*pRenderer;
	SDL_Event	 	sEvent;
	int				iQuit;
} app;

int main(int argc, char*argv[]) {

	app.iQuit = 1;
	int nStatus = 0;
	int *pScene;
	char buf[256];
	int* pCoord;
	int iDisCell;

	SDL_Init(SDL_INIT_VIDEO);

	app.pWindow=SDL_CreateWindow(
			"MineSweeper",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);

	if(app.pWindow==NULL){
		fprintf(stderr, "Unable to initialize SDL Window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	app.pRenderer=SDL_CreateRenderer(app.pWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	if(app.pRenderer==NULL){
			fprintf(stderr,"Unable to initialize hardware renderer : %s\n", SDL_GetError());
			app.pRenderer=SDL_CreateRenderer(app.pWindow, -1, SDL_RENDERER_SOFTWARE);
			if(app.pRenderer==NULL){
				fprintf(stderr,"Unable to initialize software renderer: %s\n",SDL_GetError());
				return EXIT_FAILURE;
			}
		}
	srand((unsigned int)time(NULL));

	pScene = (int*)malloc(SCENE_NB_ROW*SCENE_NB_COL*sizeof(int));
	DeminSceneInit(pScene, SCENE_NB_ROW, SCENE_NB_COL, SCENE_NB_PERCENT);

	DeminSceneDraw(app.pRenderer,pScene,SCENE_NB_ROW,SCENE_NB_COL, 1);
	SDL_Delay(15);

	SDL_RenderPresent(app.pRenderer);

	while(app.iQuit){
		while(SDL_PollEvent(&app.sEvent)){
			switch(app.sEvent.type){
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONUP:
				pCoord = OnClickCellCoordinates(&app.sEvent, pScene, SCENE_NB_ROW, SCENE_NB_COL);
				iDisCell = DiscoverCell(pScene, pCoord[0], pCoord[1], SCENE_NB_ROW, SCENE_NB_COL);
				if(iDisCell == GAME_OVER_VALUE){
					app.iQuit=0;
				}
				sprintf(buf,"Clicked cell coordinates - Ligne: %03d Colonne: %03d - Cases découvertes %d", pCoord[0], pCoord[1], iDisCell);
				SDL_SetWindowTitle(app.pWindow, buf);
				DeminSceneDraw(app.pRenderer,pScene,SCENE_NB_ROW,SCENE_NB_COL, 1);
				//AppDraw(pRenderer, &nStatus);
				break;
			case SDL_WINDOWEVENT:
				switch(app.sEvent.window.event){
				case SDL_WINDOWEVENT_SHOWN:
					break;
				case SDL_WINDOWEVENT_CLOSE:
					app.sEvent.type = SDL_QUIT;
					SDL_PushEvent(&app.sEvent);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				app.iQuit=0;
				break;
			case SDL_KEYUP:
				switch(app.sEvent.key.keysym.sym){
				case SDLK_ESCAPE:
					app.iQuit=0;
					break;
				case SDLK_q:
					app.iQuit=0;
					break;
				case SDLK_SPACE:
					//AppDraw(pRenderer, &nStatus);
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

		free(pCoord);
		pCoord=NULL;
		free(pScene);
		pScene=NULL;
		SDL_DestroyRenderer(app.pRenderer);
		SDL_DestroyWindow(app.pWindow);
		SDL_Quit();
		return EXIT_SUCCESS;
}


