/*
 * Copyright (C) <2016>  <chenp>
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 * 
 */

//Version 1.0
#include<stdlib.h>
#include<stdio.h>
#include<SDL/SDL.h>
#include<math.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include "DrawSdl1.h"
#include "Xgraphics.h"



#define taillex 1200
#define tailley 800
/**
Fonctions de bases permettant le dessin
sans passer par sdl_GFX
+fonction qui permet le gras(recalcul)
*/

SDL_Surface * screen;

Uint32 white, black, couleur[100]; 

//pour ecrire des mots...
typedef struct word{
  char * mot ;
  int coordX ;
  int coordY ;
}word ;


/*
  int main(){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_WM_SetCaption("Renderer OpenStreetMap", NULL);
  putenv("SDL_VIDEO_WINDOW_POS=center"); 
  screen=SDL_SetVideoMode(800,600,32, SDL_HWSURFACE|SDL_DOUBLEBUF); 
  
  couleur[0]=SDL_MapRGB(screen->format,255,255,255); //white 
  couleur[1]=SDL_MapRGB(screen->format,255,0,0); // red
  couleur[2]=SDL_MapRGB(screen->format,0,250,0); // vert 
  couleur[3]=SDL_MapRGB(screen->format,0,0,0); // black
  couleur[4]=SDL_MapRGB(screen->format ,0,0,250);//BLUE
  SDL_FillRect(screen,0,couleur[0]); 
  SDL_Color couleurNoire = {0, 0, 0};
  
  
  char* names[5]= {"rue homer","rue donut","rue saint","rue krakra","rue henri"};
  int exemple[2][5]={{100,300,400,450,100},{200,500,100,300,550}};
  
  SDL_Surface *texte[5]  ;
  
  
  SDL_Rect position;
  TTF_Font *police = NULL;//declaration de la police
  if(TTF_Init() == -1){
  fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
  exit(EXIT_FAILURE);
  }
  
  police = TTF_OpenFont("angelina.TTF", 35);//taille a changer..
  
  
  line(100,100,200,100, couleur[3]);
  line(200,100,200,400, couleur[3]);
  line(200,400,100,400, couleur[3]);
  line(100,400,100,200, couleur[3]);
  line(100,200,50,200, couleur[3]);
  line(50,200,100,100, couleur[3]);
  
 
  floodfill( 101,101, couleur[1], couleur[3]);
  
  linewithwidth(500,400,700,200,2,couleur[3]);
  
  //test d affichage de mots dans la fenetre
  int ii ;
  for(ii=0 ; ii< 5 ;ii++){
  texte[ii] = TTF_RenderText_Blended(police, names[ii], couleurNoire);  
  position.x = exemple[0][ii];
  position.y = exemple[1][ii];
  SDL_BlitSurface(texte[ii], NULL, screen, &position); 
  }
  
  SDL_Flip(screen); 
  pause();
  
  return 0;  
  }
*/




/**************************************
Fonctions de dessins
-Tracer des lignes en gras ou non (chemins ,rues ..)
-polygones (batiments ect)
-la couleur des objets invoques mettre le point a l interieur de la figure
***************************************/

void pause2()
{
  SDL_Event event;
  // do SDL_WaitEvent(&evenement);
  //while(evenement.type != SDL_QUIT && evenement.type != SDL_KEYDOWN); 
  //SDL_SaveBMP(screen,"sortie.bmp");
  
  int continuer = 1;
  while(continuer)
    {
      switch(event.type)
        {   
        case SDL_QUIT:
	  continuer = 0;
	  break;
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
            case SDLK_ESCAPE :
	      continuer = 0;
	      break;
	    default:; 
	    }
	  break; 
	default: ;
        }
      SDL_PollEvent(&event);
    }

}


void putpixel(int xe, int ye, Uint32 c)
{ 
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; *numerocase=c; 
}

Uint32 getpixel(int xe, int ye)
{ 
  Uint32 * numerocase;
  numerocase= (Uint32 *)(screen->pixels)+xe+ye*screen->w; return (*numerocase); 
}
/**
Trace une simple Ligne droite
*/

void line(int x0,int y0, int x1,int y1, Uint32 c)
{
  int dx,dy,x,y,residu,absdx,absdy,pasx,pasy,i;
  dx=x1-x0; dy=y1-y0; residu=0; x=x0;y=y0; if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
  if (dx>0) pasx=1;else pasx=-1; if (dy>0) pasy=1; else pasy=-1;
  absdx=abs(dx);absdy=abs(dy);
  if (dx==0) for(i=0;i<absdy;i++) { y+=pasy;
      if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c); }
  else if(dy==0) for(i=0;i<absdx;i++){ x+=pasx;
      if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c); }
  else if (absdx==absdy)
    for(i=0;i<absdx;i++) {x+=pasx; if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
      y+=pasy;
      if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
    }
  else if (absdx>absdy)
    for(i=0;i<absdx;i++)
      { x+=pasx; if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
	residu+=absdy;
	if(residu >= absdx) {residu -=absdx; y+=pasy;
	  if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
	}
      }
  else for(i=0;i<absdy;i++)
	 {y+=pasy; if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c);
	   residu +=absdx;
	   if (residu>=absdy) {residu -= absdy;x +=pasx;
	     if (x>=0 && x<taillex && y>=0 && y<tailley) putpixel(x,y,c); 
	   }
	 }
}
/**
Methode permettant le gras (utilise maintenant des fonctions 
de GX qui permet des traits droits
*/

void linewithwidth(int x1, int y1, int x2, int y2, int width,Uint32 c)
{
  int dx,dy;
  float k,xf1,yf1,xf2,yf2,d,dx1,dy1,ndx1,ndy1,ndx2,ndy2,angle=M_PI/2.;
  aalineColor(screen ,x1,y1,x2,y2,c);
  dx=x2-x1; dy=y2-y1; d=sqrt(dx*dx+dy*dy);
  if (d!=0.) /* si le vecteur n’est step nul */
    { dx1=(float)width*(float)dx/d; dy1=(float)width*(float)dy/d;
      ndx1=dx1*cos(angle)-dy1*sin(angle);
      ndy1=dx1*sin(angle)+dy1*cos(angle);
      ndx2=dx1*cos(-angle)-dy1*sin(-angle);
      ndy2=dx1*sin(-angle)+dy1*cos(-angle);
      for(k=0;k<=1.;k+=0.1/d)
	{
	  xf1=(1.-k)*x1+k*x2; yf1=(1.-k)*y1+k*y2;
	  xf2=xf1-ndx1; yf2=yf1-ndy1;  aalineColor(screen,xf1,yf1,xf2,yf2,c);//line(xf1,yf1,xf2,yf2,c);
	  xf2=xf1-ndx2; yf2=yf1-ndy2;  aalineColor(screen,xf1,yf1,xf2,yf2,c); //line(xf1,yf1,xf2,yf2,c);
	}
    }
}

/**
Floodfill methode de coloriage 
*/
void floodfill( int x,int y, Uint32 cr,Uint32 cb)
{ 
  int xg,xd,xx;
  if (getpixel(x,y) !=cb && getpixel(x,y) !=cr)
    { putpixel(x,y,cr);
      xg=x-1;
      while(xg>0 && getpixel(xg,y)!=cb) {putpixel(xg,y,cr); xg--;}
      xd=x+1;
      while(xd<800 && getpixel(xd,y)!=cb) {putpixel(xd,y,cr); xd++ ;}
      for(xx=xg; xx<xd;xx++)
	{ if (y>1 ) {floodfill(xx,y-1,cr,cb);}
	  if (y<599 ) {floodfill(xx,y+1,cr,cb);} 
	}
    }
}


 
