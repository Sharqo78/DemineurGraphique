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
	Uint32				uStatus;
	SDL_Window		*	pWindow;
	SDL_Renderer	*	pRenderer;
	SDL_Event	 		sEvent;
	int					iQuit;
	TTF_Font		*	pFont;
	SDL_Surface		*	pSurface;
	SDL_Rect			surfaceRect;
	SDL_Texture		*	pTexture;
	SDL_Color			colorText; 
} app;

int main(int argc, char*argv[]) {

	app.iQuit = 1;
	int nStatus = 0;
	int *pScene;
	char buf[256];
	int* pCoord;
	int iDisCell;
	int iTotalDisCell = 0;
	int nbMines = (SCENE_NB_COL*SCENE_NB_PERCENT*SCENE_NB_ROW)/100;
	if(SDL_Init(SDL_INIT_VIDEO)==-1){
		fprintf(stderr, "SDL2 Initialization failed: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}


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
	
	//Initialisation et affectation de la police d'écriture
	//La condition prévoit de fermer le programme en cas d'erreur.
	TTF_Init();
	app.pFont = TTF_OpenFont("/home/salim/git/DemineurGraphique/Debug/arial.ttf", SCENE_CELL_SIZE);

	if(app.pFont == NULL){
		printf("TTF_Openfont() : %s\n", TTF_GetError());
		TTF_Quit();
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	srand((unsigned int)time(NULL));

	//Initiation de la scène de jeu
	pScene = (int*)malloc((SCENE_NB_ROW*SCENE_NB_COL+1)*sizeof(int));
	DeminSceneInit(pScene, SCENE_NB_ROW, SCENE_NB_COL, SCENE_NB_PERCENT);

	//Traçage de la scène de jeu.
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
				iTotalDisCell += iDisCell;
				sprintf(buf,"Clicked cell coordinates - Ligne: %03d Colonne: %03d - Cases découvertes %d", pCoord[0], pCoord[1], iDisCell);
				SDL_SetWindowTitle(app.pWindow, buf);
				DeminSceneDraw(app.pRenderer,pScene,SCENE_NB_ROW,SCENE_NB_COL, 1);
				//Procédure en cas de "Game Over"
				if(iDisCell == GAME_OVER_VALUE){
					 DeminSceneDraw(app.pRenderer,pScene,SCENE_NB_ROW,SCENE_NB_COL, 0);
					 app.colorText.r = 255;
					 app.colorText.g = 255;
					 app.colorText.b = 255;
					 app.colorText.a = 255;
					 app.pSurface = TTF_RenderText_Blended(app.pFont, "GAME OVER", app.colorText);
					 app.pTexture = SDL_CreateTextureFromSurface(app.pRenderer, app.pSurface);

					 app.surfaceRect.w = WINDOW_WIDTH;
					 app.surfaceRect.x = 0;
					 app.surfaceRect.h = SCENE_CELL_SIZE;
					 app.surfaceRect.y = WINDOW_HEIGHT/2;

					 SDL_RenderCopy(app.pRenderer, app.pTexture, NULL, &app.surfaceRect);
					 SDL_DestroyTexture(app.pTexture);
					 SDL_FreeSurface(app.pSurface);

					 SDL_RenderPresent(app.pRenderer);

					 SDL_Delay(2500);
					 app.iQuit=0;
				}
				else if (iTotalDisCell==(SCENE_NB_COL*SCENE_NB_ROW)-nbMines){
					app.colorText.r = 255;
					app.colorText.g = 255;
					app.colorText.b = 255;
					app.colorText.a = 255;
					app.pSurface = TTF_RenderText_Blended(app.pFont, "You WIN", app.colorText);
					app.pTexture = SDL_CreateTextureFromSurface(app.pRenderer, app.pSurface);
					DeminSceneDraw(app.pRenderer, pScene, SCENE_NB_ROW, SCENE_NB_COL, 0);
					app.surfaceRect.w = WINDOW_WIDTH;
					app.surfaceRect.x = 0;
					app.surfaceRect.h = SCENE_CELL_SIZE;
					app.surfaceRect.y = WINDOW_HEIGHT/2;
					
					SDL_RenderCopy(app.pRenderer, app.pTexture, NULL, &app.surfaceRect);
					SDL_DestroyTexture(app.pTexture);
					SDL_FreeSurface(app.pSurface);

					SDL_RenderPresent(app.pRenderer);

					SDL_Delay(2500);
					app.iQuit=0;
				}
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

		TTF_CloseFont(app.pFont);
		free(pCoord);
		pCoord=NULL;
		free(pScene);
		pScene=NULL;
		SDL_DestroyRenderer(app.pRenderer);
		SDL_DestroyWindow(app.pWindow);
		SDL_Quit();
		return EXIT_SUCCESS;
}


