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
//Valeur
int AppIsMouseOnCell(int xM, int yM, int nRow, int nCol){
	int k,m;
	for(k=PADDING_TOP;k<(nRow*SCENE_CELL_BLOCK);k+=SCENE_CELL_BLOCK){
		for(m=PADDING_HRZ;m<(nCol*SCENE_CELL_BLOCK);m+=SCENE_CELL_BLOCK){
			if( ((xM<m) && (yM<k+SCENE_CELL_BLOCK)) || ((xM<m+SCENE_CELL_BLOCK) && (yM<k)) )return 0;
			else if( (xM<m+SCENE_CELL_SIZE) && (yM<(k+SCENE_CELL_SIZE)) )return 1;
		}
	}
}

//Cette fonction gère les coordonnées des cases
//Elle prend l'évènement pour interpréter les clics de souris et repère l'endroit du clic
//Elle retourne un tableau de pointeurs contenant les coordonnés X et Y des cases
//Si on sort des limites de la scène de jeu, elle retourne le nombre de cellules + 1 
//Valeur non utilisée par la génération du champ de jeu.
//J'ai du ressortir à cette combine car ça ne retournait rien quand j'étais pas dans les limites et ça faisait planter le jeu.
int* OnClickCellCoordinates(SDL_Event*event, int* pS, int nRow, int nCol){
	int *pCoord;
	int xM, yM;
	int k,m;
	int cellRow, cellCol;

	if (!AppIsMouseOnCell(event->button.x,event->button.y, nRow, nCol))goto stop;

	//On initialise le pointeur qui contiendra les coordonnées de la case cliquée.
	pCoord = (int*)malloc(2*sizeof(int));

	//On ignore le padding horizontal et le padding de la bordure supérieure
	xM = (event->button.x-PADDING_HRZ);
	yM = (event->button.y-PADDING_TOP);

	cellCol = xM/(SCENE_CELL_SIZE+SCENE_CELL_SPACING);
	cellRow = yM/(SCENE_CELL_SIZE+SCENE_CELL_SPACING);
	
	if( (xM>0) && (yM>0) && (yM<PLAYFIELD_HEIGHT) && (xM<PLAYFIELD_WIDTH) ){
		pCoord[0]=cellRow;
		pCoord[1]=cellCol;
		return pCoord;
		free(pCoord);
		pCoord=NULL;
	}
	//J'avais envie d'essayer l'instruction goto
	else goto stop;
	stop:
		pCoord[0]=SCENE_NB_CELLS+1;
		pCoord[1]=0;
		return pCoord;
		free(pCoord);
		pCoord=NULL;
}


//Cette fonction est un clair copié-collé du démineur terminal
int	DiscoverCell(int* pS, int x, int y, int nRow, int nCol){
	int k,m;
	int cnt=0;
	//Si la cellule a déjà été jouée, on retourne 0
	if( x==(SCENE_NB_CELLS+1) )return 0;
	if(mIsPlayed(pS[x+(y*nRow)]))return 0;
	mSetPlayed(pS[x+(y*nRow)]);
	//Si la celllule est une mine, on² retourne -1 (partie terminée)
	if((mCellValue(pS[x+(y*nRow)])==CELL_MINE))return GAME_OVER_VALUE;
	//Si la cellule n'est pas une mine mais qu'elle n'est pas nulle,
	//on retourne 1, car on aura découvert qu'une case
	if((mCellValue(pS[x+(y*nRow)])!=CELL_VOID))return 1;
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

//Cette fonction sert à initier le tableau de jeu en mémoire 
void DeminSceneInit(int *pS, int nRow, int nCol, int nPercent){
    int k,m;
    int x,y;
    int nbMines = (nRow*nCol*nPercent)/100;

    //Mise de toutes les valeurs à 0
    for(k=0;k<(nCol*nRow);k++)pS[k]=0;
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

//Cette fonction sert à tracer la scène de jeu.
//mode 1 = mode de jeu
//mode 0 = mode découvert
void DeminSceneDraw(SDL_Renderer *pRenderer,int *pS, int nRow, int nCol, int mode){
    SDL_Rect r = {};
    int k,m;
    r.w = SCENE_CELL_SIZE;
    r.h = SCENE_CELL_SIZE;
    r.x=PADDING_HRZ;
    r.y=PADDING_TOP;
	SDL_SetRenderDrawColor(pRenderer, 56, 60, 74, 255);
	SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    if(mode){
        for(k=0;k<nRow;k++){
			for(m=0;m<nCol;m++){
				if(mIsPlayed(pS[k+m*nRow])){
					if(mCellValue(pS[k+m*nRow])>=CELL_MINE){
						SDL_SetRenderDrawColor(pRenderer, 82, 148, 255, 255);
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
				}
				else{
					SDL_SetRenderDrawColor(pRenderer, 75, 81, 98, 255);
					SDL_RenderFillRect(pRenderer, &r);
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
					SDL_SetRenderDrawColor(pRenderer, 82, 148, 255, 255);
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
	SDL_RenderPresent(pRenderer);
}
