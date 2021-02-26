#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#include "defines.h"
#include "demin.h"

//Cette fonction permet de savoir si on est sur une case ou non.
//Valeur retournée en cas de réussite -> 1 
//Valeur retournéee en cas d'échec -> 0

int AppIsMouseOnCell(int xM, int yM, int nRow, int nCol){
	int k,m;

	//On boucle sur le champ de jeu vertical
	for(k=PADDING_TOP;k<(nRow*SCENE_CELL_BLOCK);k+=SCENE_CELL_BLOCK){
		//Puis sur le champ de jeu horizontal
		for(m=PADDING_HRZ;m<(nCol*SCENE_CELL_BLOCK);m+=SCENE_CELL_BLOCK){
			//On vérifie si on est en dehors des cellules 
			if( ((xM<m) && (yM<k+SCENE_CELL_BLOCK)) || ((xM<m+SCENE_CELL_BLOCK) && (yM<k)) )return 0;
			else if( (xM<m+SCENE_CELL_SIZE) && (yM<(k+SCENE_CELL_SIZE)) )return 1;
		}
	}
}

/*Cette fonction gère les coordonnées des cases
Elle prend l'évènement pour interpréter les clics de souris et repère l'endroit du clic
Elle retourne un tableau de pointeurs contenant les coordonnés X et Y des cases
Si on sort des limites de la scène de jeu, elle retourne le nombre de cellules + 1 
Valeur non utilisée par la génération du champ de jeu.
J'ai du ressortir à cette combine car ça ne retournait rien quand j'étais pas dans les limites et ça faisait planter le jeu.*/

void OnClickCellCoordinates(SDL_Event*event, int* pS, int nRow, int nCol, int *pCoord){
	int xM, yM;
	int k,m;
	int cellRow, cellCol;

	//Si on est pas sur la case, on arrête.
	if (!AppIsMouseOnCell(event->button.x,event->button.y, nRow, nCol)){
		pCoord[0]=SCENE_NB_CELLS+1;
		pCoord[1]=0;
	}

	//On ignore le padding horizontal et le padding de la bordure supérieure
	xM = (event->button.x-PADDING_HRZ);
	yM = (event->button.y-PADDING_TOP);

	cellCol = xM/(SCENE_CELL_SIZE+SCENE_CELL_SPACING);
	cellRow = yM/(SCENE_CELL_SIZE+SCENE_CELL_SPACING);

	if( (xM>0) && (yM>0) && (yM<PLAYFIELD_HEIGHT) && (xM<PLAYFIELD_WIDTH) ){
		pCoord[0]=cellRow;
		pCoord[1]=cellCol;
	}

	else{
		pCoord[0]=SCENE_NB_CELLS+1;
		pCoord[1]=0;
	}
}


//Cette fonction est un clair copié-collé du démineur terminal
int	DiscoverCell(int* pS, int x, int y, int nRow, int nCol){
	int k,m;
	int cnt=0;
	//Si la cellule a déjà été jouée, qu'elle est la cellule "hors terrain" ou bien une cellule "flaguée", on retourne 0
	if((mIsPlayed(pS[x+(y*nRow)])) || (x==(SCENE_NB_CELLS+1)) || (mIsFlagged(pS[x+(y*nRow)])) )return 0;

    else mSetPlayed(pS[x+(y*nRow)]);
	//Si la celllule est une mine, on retourne -1 (partie terminée)
	if((mCellValue(pS[x+(y*nRow)])==CELL_MINE))return GAME_OVER_VALUE;
	//Si la cellule n'est pas une mine mais qu'elle n'est pas nulle,
	//on retourne 1, car on aura découvert qu'une case
	if (mCellValue(pS[x+(y*nRow)])!=CELL_VOID)return 1;
	if (mCellValue(pS[x+(y*nRow)])==CELL_VOID)cnt++;
	for(k=-1;k<=1;k++){
		for(m=-1;m<=1;m++){
			if(((k+x)>=0) && ((m+y)>=0) && ((k+x)<nRow) && ((m+y)<nCol)){
				cnt+=DiscoverCell(pS,x+k,y+m, nRow, nCol);
			}
		}
	}
	return cnt;
}


//Cette fonction sert à marquer une cellule :
//Vous avez le choix entre un point d'interrogation (case suspectée) et un drapeau (case sûre)
//mode 0 : drapeau
//mode 1 : question
void MarkCell(int *pS, int x, int y, int nRow, int nCol, int mode){
	if(!mIsPlayed(pS[x+y*nRow])){
		if(!mode){
			if(mIsQuestion(pS[x+y*nRow])){
				mClrQuestion(pS[x+y*nRow]);
				mTglFlagged(pS[x+y*nRow]);
			}
			else mTglFlagged(pS[x+y*nRow]);
		}
		else{
			if(mIsFlagged(pS[x+y*nRow])){
				mClrFlagged(pS[x+y*nRow]);
				mTglQuestion(pS[x+y*nRow]);
			}
			else mTglQuestion(pS[x+y*nRow]);
		}
	}
}


//Cette fonction sert à initier le tableau de jeu en mémoire 
void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent){
    int k,m;
    int x,y;
    int nbMines = (nRow*nCol*nPercent)/100;

    //Mise de toutes les valeurs à 0
    for(k=0;k<(nCol*nRow);k++)pS[k]=CELL_VOID;
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

struct colors{
	SDL_Color		Bg;
	SDL_Color		Hidden;
	SDL_Color		Mine; //Les cases adjacentes ont les mêmes couleurs que les mines, seulement, elles on un niveau alpha qui varie selon le chiffre.
	SDL_Color		Void;
}color;
//Cette fonction sert à tracer la scène de jeu.
//mode 1 = mode de jeu
//mode 0 = mode découvert
void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode, TTF_Font *pFont){
    SDL_Rect r = {};
    int k,m;
    r.w = SCENE_CELL_SIZE;
    r.h = SCENE_CELL_SIZE;
    r.x=PADDING_HRZ;
    r.y=PADDING_TOP;

	mToColor(color.Bg, SCENE_COLOR_BACKGROUND);
	mToColor(color.Mine, SCENE_COLOR_MINE);
	mToColor(color.Hidden, SCENE_COLOR_HIDDEN);
	mToColor(color.Void, SCENE_COLOR_VOID);

	SDL_SetRenderDrawColor(pRenderer, color.Bg.r, color.Bg.g, color.Bg.b, color.Bg.a);
	SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    if(mode){
        for(k=0;k<nRow;k++){
			for(m=0;m<nCol;m++){
				if(mIsPlayed(pS[k+m*nRow])){
					if(mCellValue(pS[k+m*nRow])==CELL_MINE){
						SDL_SetRenderDrawColor(pRenderer, color.Mine.r, color.Mine.g, color.Mine.b, color.Mine.a);
						SDL_RenderFillRect(pRenderer, &r);
						DeminDrawNumber(pS[k+m*nRow], pRenderer, r.x, r.y, pFont, mode);
					}
					else if(mCellValue(pS[k+m*nRow])==CELL_VOID){
						SDL_SetRenderDrawColor(pRenderer, color.Void.r, color.Void.g, color.Void.b, color.Void.a);
						SDL_RenderFillRect(pRenderer, &r);
					}
					else{
						SDL_SetRenderDrawColor(pRenderer, color.Mine.r, color.Mine.g, color.Mine.b, pS[k+m*nRow]*26);
						SDL_RenderFillRect(pRenderer, &r);
						DeminDrawNumber(pS[k+m*nRow], pRenderer, r.x, r.y, pFont, mode);
					}
				}
				else{
					SDL_SetRenderDrawColor(pRenderer, color.Hidden.r, color.Hidden.g, color.Hidden.b, color.Hidden.a);
					SDL_RenderFillRect(pRenderer, &r);
					DeminDrawNumber(pS[k+m*nRow], pRenderer, r.x, r.y, pFont, mode);
				}
				r.x+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;

			}
			r.x=PADDING_HRZ;
			r.y+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;

    	}
    }
    else{
		for(k=0;k<nRow;k++){
			for(m=0;m<nCol;m++){
				if(mCellValue(pS[k+m*nRow])>=CELL_MINE){
					SDL_SetRenderDrawColor(pRenderer, color.Mine.r, color.Mine.g, color.Mine.b, color.Mine.a);
					SDL_RenderFillRect(pRenderer, &r);
					DeminDrawNumber(pS[k+m*nRow], pRenderer, r.x, r.y, pFont, mode);
				}
				else if(mCellValue(pS[k+m*nRow])==CELL_VOID){
					SDL_SetRenderDrawColor(pRenderer, color.Void.r, color.Void.g, color.Void.b, color.Void.a);
					SDL_RenderFillRect(pRenderer, &r);
				}
				else{
					SDL_SetRenderDrawColor(pRenderer, color.Mine.r, color.Mine.g, color.Mine.b, pS[k+m*nRow]*26);
					SDL_RenderFillRect(pRenderer, &r);
					DeminDrawNumber(pS[k+m*nRow], pRenderer, r.x, r.y, pFont, mode);
				}
				r.x+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;
			}
			r.x=PADDING_HRZ;
			r.y+=SCENE_CELL_SPACING+SCENE_CELL_SIZE;
		}
	}
	SDL_RenderPresent(pRenderer);
}

//Cette fonction se charge de tracer les nombres à l'intérieur de la case.
void DeminDrawNumber(int number, SDL_Renderer *pRenderer, int x, int y, TTF_Font *pFont, int mode){
	SDL_Surface 	*pSurfaceNb;
	SDL_Texture 	*pTextureNb;

	SDL_Surface		*pSurfaceMine;
	SDL_Texture		*pTextureMine;

	SDL_Color		col = {255,255,255,255};

	SDL_Rect		rect;

	char buf[256] = "";

	if( (!mode) || (mIsPlayed(number)) ){
		if(mCellValue(number)==CELL_MINE){
			pSurfaceMine = SDL_LoadBMP("mine.bmp");
			pTextureMine = SDL_CreateTextureFromSurface(pRenderer, pSurfaceMine);

			rect.x = x;
			rect.y = y;
			rect.w = SCENE_CELL_SIZE;
			rect.h = SCENE_CELL_SIZE;

			SDL_RenderCopy(pRenderer, pTextureMine, NULL, &rect);
			SDL_DestroyTexture(pTextureMine);
			SDL_FreeSurface(pSurfaceMine);
		}
		
		else{
			if(mCellValue(number)==CELL_VOID)sprintf(buf, " ");
			else sprintf(buf,"%d",mCellValue(number));
		
			pSurfaceNb = TTF_RenderText_Blended(pFont, buf , col);
			pTextureNb = SDL_CreateTextureFromSurface(pRenderer, pSurfaceNb);

			rect.w = pSurfaceNb->w;
			rect.h = pSurfaceNb->h;
			rect.x = x+(SCENE_CELL_SIZE/2)-(rect.w/2);
			rect.y = y+(SCENE_CELL_SIZE/2)-(rect.h/2);

			SDL_RenderCopy(pRenderer, pTextureNb, NULL, &rect);
			SDL_DestroyTexture(pTextureNb);
			SDL_FreeSurface(pSurfaceNb);
		}
	}
	else{
		if(mIsFlagged(number)){
		    pSurfaceMine = SDL_LoadBMP("flag.bmp");
			pTextureMine = SDL_CreateTextureFromSurface(pRenderer, pSurfaceMine);

			rect.x = x;
			rect.y = y;
			rect.w = SCENE_CELL_SIZE;
			rect.h = SCENE_CELL_SIZE;

			SDL_RenderCopy(pRenderer, pTextureMine, NULL, &rect);
			SDL_DestroyTexture(pTextureMine);
			SDL_FreeSurface(pSurfaceMine);
		}
		if(!mIsFlagged(number)){
			if(mIsQuestion(number))sprintf(buf, "?");
			else sprintf(buf, " ");

			pSurfaceNb = TTF_RenderText_Blended(pFont, buf , col);
			pTextureNb = SDL_CreateTextureFromSurface(pRenderer, pSurfaceNb);

			rect.w = pSurfaceNb->w;
			rect.h = pSurfaceNb->h;
			rect.x = x+(SCENE_CELL_SIZE/2)-(rect.w/2);
			rect.y = y+(SCENE_CELL_SIZE/2)-(rect.h/2);

			SDL_RenderCopy(pRenderer, pTextureNb, NULL, &rect);
			SDL_DestroyTexture(pTextureNb);
			SDL_FreeSurface(pSurfaceNb);
		}
	}
}

//Se charge d'écrire les messages qui seront visible dans la partie "Padding Top"
void DeminDrawMessage(SDL_Renderer *pRenderer, TTF_Font *pFont, char *text){
	SDL_Rect 		r;
	SDL_Surface		*pSurfaceText;
	SDL_Texture		*pTextureText;
	SDL_Color 		col = {255,255,255,255};

	pSurfaceText = TTF_RenderText_Blended(pFont, text, col);
	pTextureText = SDL_CreateTextureFromSurface(pRenderer, pSurfaceText);

	r.w = pSurfaceText->w;
	r.h = pSurfaceText->h;

	r.x = (WINDOW_WIDTH/2)-((pSurfaceText->w)/2);
	r.y = 0;

	SDL_RenderCopy(pRenderer, pTextureText, NULL, &r);
	SDL_DestroyTexture(pTextureText);
	SDL_FreeSurface(pSurfaceText);
	SDL_RenderPresent(pRenderer);
}
