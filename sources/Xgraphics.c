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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <SDL_gfxPrimitives.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_rotozoom.h>
#include <curl/curl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <glib.h>


#include "Xgraphics.h" 
#include "fgifsdl.h"
#include "DrawSdl1.h"
#include "cercle.h"

/*
  #define taillex 1000
  #define tailley 700 
*/




SDL_Surface * screen;
SDL_Surface * screen2;
int colar =70;
TTF_Font *police=NULL ;
int option =0;  
SDL_Color couleurNoire = {0, 0, 0};
colour *style ;
int taillex =1000;
int tailley=700;
cercle *list ;
int listsize ;
int mer=0 ;
int drawing = 0 ;

void res(int n){
  list = malloc((n*3)*sizeof(cercle));
  listsize =0 ;
}

void res2(int n){
  mer = n ;
}

/**
   Draw only if the camera can see it ,it seem to be usefull.
*/
int dessinePas(int a ,int b){
  if(a<0)
    return 0 ;
  if(b<0)
    return 0 ;
  if(a> taillex+20)
    return 0 ;
  if(b > tailley +20)
    return 0 ;
  return 1 ;        
}



//function used for the --past mode

void rendu (ways *chemins,int size ,double minla ,double maxla ,double minlo ,double maxlo  ){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_WM_SetCaption("Renderer OpenStreetMap", NULL);
  putenv("SDL_VIDEO_WINDOW_POS=center"); 
  screen=SDL_SetVideoMode(taillex,tailley,32, SDL_HWSURFACE|SDL_DOUBLEBUF); 
  couleur[0]=SDL_MapRGB(screen->format,245,214,23); //jaune 
  couleur[1]=SDL_MapRGB(screen->format,255,0,0); // red
  couleur[2]=SDL_MapRGB(screen->format,0,250,0); // vert 
  couleur[3]=SDL_MapRGB(screen->format,0,0,0); // black
  couleur[4]=SDL_MapRGB(screen->format ,0,0,250);//BLUE 
  couleur[5]=SDL_MapRGB(screen->format ,69,139,116);//other        
  SDL_FillRect(screen,0,couleur[0]); 
  int wi = taillex ;
  int he = tailley ;
  //Y = tailley - coordonne
  //calculons d abord la difference
  int differenceX=0 ;//vars pour cr
  int differenceY=0 ;
  differenceX = (maxla - minla)*10000000;//bas haut
  differenceY = (maxlo - minlo)*10000000;//gauche droite
  
  
  differenceX = differenceX/20 ;
  differenceY = differenceY/20;
  //Une unite corresponds a differenceX/nbcoupage nbcoupage=20 ,tout sera arrondi au int pret car nos fonctions sont en int    
  //on utilise un tableau de proportionalite pour trouver la position (environ d un  noeud
  //taillex/20 <- difference/20  
  //   ?       <- node.value (convertie)
  
  int inc  ;
  int jin  ;
  //short moins de place qu un int ...
  short deb =0 ;
  short deb2=0 ;
  short fin =0 ;
  short fin2=0 ;
  
  for(inc=0 ; inc < size ; inc++){      
    for(jin=0;jin<(chemins[inc].sizeN)-1 ; jin ++){
      deb=(((wi/50)*((chemins[inc].noeuds[jin].lon-minlo)*10000000))/differenceX);
      deb2= ( ((he/10)*((chemins[inc].noeuds[jin].lat-minla)*10000000))/differenceY);
      fin=(((wi/50)*((chemins[inc].noeuds[jin+1].lon-minlo)*10000000)))/differenceX;
      fin2=( ((he/10)*((chemins[inc].noeuds[jin+1].lat - minla)*10000000))/differenceY) ;
      aalineRGBA(screen,deb ,(he-deb2) ,fin,(he-fin2),80,100,100,200);
    } 
  }
  
  SDL_Flip(screen); 
  pause2();
  return;
}

int KeyHit()//temporaire
{
  SDL_Event e;
  if (SDL_PollEvent(&e))
    if (e.type == SDL_KEYDOWN)
      return 1;
  return 0;
}

//function used for zoom under the bmp format
void rendu3(){
  /*old version of zoom and translation*/
  SDL_Surface *image = NULL, *rotation = NULL;
  SDL_Rect rect;
  SDL_Event event;
  /*for later*/
  double angle = 0;
  // int sens = 1;
  /*for later*/
  double zoom = 2;
  int continuer = 1;
  int px=0;
  int py=0;
  rect.x =4;
  rect.y= 4;
  image = SDL_LoadBMP("sortie.bmp");
  
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
            case SDLK_a:
              if(zoom<4)
		zoom +=0.2;
	          
	      break;
	    case SDLK_z:
	      if(zoom>1.5)
		zoom -=0.1; 
	      break;
	    case SDLK_UP:
	      if(py<=700)
		py=py+100;     
	   
	      break;
            case SDLK_DOWN :
	      if(py>=50)
		py=py-100; 
             
	      break; 
	    case SDLK_LEFT: 
	      if(px <=1100)
	        px=px+100;
              
	      break;
	    case SDLK_RIGHT:
	      if(px >=50)	     
		px=px-100;
            
	      break;    
	    case SDLK_UNKNOWN:
	      printf("unknow key");	
	      break; 
	    default:;
	    }
	  break;
	default :;
        }
      //  printf("%f  %d  %d \n",zoom,px,py);
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
      rotation = rotozoomSurface(image, angle, zoom, 0);
      SDL_BlitSurface(rotation , NULL, screen, &rect);  
      SDL_PollEvent(&event);
      
      rect.x =  (px - rotation->w / 2);
      rect.y =  (py - rotation->h / 2);
      SDL_FreeSurface(rotation);
      SDL_Flip(screen);
    }
  SDL_FreeSurface(screen);
  SDL_FreeSurface(image);
  SDL_Quit();
}

//printf circle to see where we are
void zonage(){
  int kin = 0 ;
  for (kin=0 ; kin < listsize ; kin++){
    filledCircleRGBA(screen,list[kin].px,list[kin].py,list[kin].rad,200,0,00,60);  
  }
  listsize =0 ;
}


//Main function for draw lines and points
void dessin(ways *chemins,int size ,double minla ,double maxla ,double minlo ,double maxlo,int mode,int zo,int zo2,int zo3,int zo4 ,int angular,int ang,int theta){
  SDL_Rect position;
 
  // int wi = taillex ;
  // int he = tailley ;
  int inc =0;
  int jin =0 ;
  int tagI =0;
  short deb =0 ;
  short deb2=0 ;
  short fin =0 ; 
  short fin2=0 ;
  double* tmp =NULL;
  couleur[10]=SDL_MapRGB(screen->format,style[28].g,style[28].b,style[28].a);
  couleur[11]=SDL_MapRGB(screen->format,style[29].g,style[29].b,style[29].a);
  couleur[12]=SDL_MapRGB(screen->format,style[30].g,style[30].b,style[30].a);
  couleur[13]=SDL_MapRGB(screen->format,style[31].g,style[31].b,style[31].a);
  couleur[14]=SDL_MapRGB(screen->format,style[32].g,style[32].b,style[32].a);
  couleur[15]=SDL_MapRGB(screen->format,style[33].g,style[33].b,style[33].a);
  couleur[16]=SDL_MapRGB(screen->format,style[34].g,style[34].b,style[34].a);
  couleur[17]=SDL_MapRGB(screen->format,style[35].g,style[35].b,style[35].a);
  couleur[18]=SDL_MapRGB(screen->format,style[36].g,style[36].b,style[36].a);
  
  
  couleur[4]=SDL_MapRGB(screen->format,22,189,0); 
  for(inc=0 ; inc < size ; inc++){      

    
    if(chemins[inc].noeuds[0].lat == chemins[inc].noeuds[(chemins[inc].sizeN -1)].lat && chemins[inc].noeuds[0].lon == chemins[inc].noeuds[(chemins[inc].sizeN -1)].lon ){
      if(mode == 45){
	if(chemins[inc].where == 1){
	  double *tp1 = convert(chemins[inc].noeuds[0].lat , chemins[inc].noeuds[0].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	  double *tp2 =convert(chemins[inc].noeuds[(chemins[inc].sizeN/2)].lat , chemins[inc].noeuds[(chemins[inc].sizeN/2)].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	  if(listsize <5){
	    list[listsize].px= (tp1[0]+tp2[0])/2 ;
	    list[listsize].py=(tp1[1]+tp2[1])/2 ;
	    list[listsize].rad=100;
	    listsize ++ ;
	  }        
        }
      }
      for(tagI=0; tagI < chemins[inc].sizeT ; tagI++){
      
	if(!strcmp(chemins[inc].tagues[tagI].k ,"landuse")) 
	  {
	    short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	    short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  
	    int k;
	    
	    for(k=0;k<chemins[inc].sizeN ; k++){
	      tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	      s[k]=tmp[0];
	      s2[k]=tmp[1];
	      if(dessinePas(s[k],s2[k]) == 1){
	        drawing =1;
	      }
	    } 
	  
	  
	    if(drawing == 1){
	      if(!strcmp(chemins[inc].tagues[tagI].v ,"allotments")){
	   
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[0].r, style[0].g, style[0].b ,style[0].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 100, 100, 100, 255); 
	      }else if(!strcmp(chemins[inc].tagues[tagI].v ,"basin")){
	     
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[1].r, style[1].g, style[1].b ,style[1].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	      }else if(!strcmp(chemins[inc].tagues[tagI].v ,"brownfield")){
	    
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[2].r, style[2].g, style[2].b ,style[2].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	      }else if(!strcmp(chemins[inc].tagues[tagI].v ,"cemetery")){
	     
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[3].r, style[3].g, style[3].b ,style[3].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	      }else if(!strcmp(chemins[inc].tagues[tagI].v ,"commercial")){
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[4].r, style[4].g, style[4].b ,style[4].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	      }else if(!strcmp(chemins[inc].tagues[tagI].v ,"construction")){
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[5].r, style[5].g, style[5].b ,style[5].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	      }else{
	     
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[5].r, style[5].g, style[5].b ,style[5].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255);
	      
	      }
	     
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[6].r, style[6].g, style[6].b ,style[6].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255); 
	    
	      int name = 0;
	      if(option==1){
		for (name=0;name < chemins[inc].sizeT ; name++){ 
		  if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		    {
		      chemins[inc].name = chemins[inc].tagues[name].v ;
		      char stre[100];
		      for (name=0 ;name <100;name++){
			stre[name]=chemins[inc].name[name];
		      }
		      TTF_Init();
             
		      chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		      position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
		      position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
		      SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		    } 
		}
	      }
	      break;
	    }
	    
	    drawing =0 ;
	  }
      
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"leisure") ){
	  short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  int k;
	  if(zo >=300){
	    for(k=0;k<chemins[inc].sizeN ; k++){
	      tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	      s[k]=tmp[0];
	      s2[k]=tmp[1];
	      if(dessinePas(s[k],s2[k]) == 1){
		drawing =1;
	      }
	    }  
	    if(drawing == 1){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[7].r, style[7].g, style[7].b ,style[7].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 0, 0, 0, 255);  
	      int name = 0;
	      if(option==1){
		for (name=0;name < chemins[inc].sizeT ; name++){
		  if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		    {
		      chemins[inc].name = chemins[inc].tagues[name].v ;
		      char stre[100];
		      for (name=0 ;name <100;name++){
			stre[name]=chemins[inc].name[name];
		      }
		      chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		      position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
		      position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
		      SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		    } 
		}
	      }
	      break;
	    }
	  }
	  drawing =0 ; 
        }
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"building")){
	  short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  if(zo >=-120){
	    int k;
	    for(k=0;k<chemins[inc].sizeN ; k++){
	      tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode ,zo, zo2, zo3, zo4,angular,ang,theta);
	      s[k]=tmp[0];
	      s2[k]=tmp[1];
	      if(dessinePas(s[k],s2[k]) == 1){
	        drawing =1;
	      }
	    }
	    if(mode ==8){
	      int distance = sqrt((s[0]-s[chemins[inc].sizeN/2])*(s[0]-s[chemins[inc].sizeN/2]) + (s2[0]-s2[chemins[inc].sizeN/2])*(s2[0]-s2[chemins[inc].sizeN/2]));
	      if(distance >= (taillex*tailley)/60000){
	        drawing = 1 ;
	      }else drawing =0 ;
	    }
	    if(drawing == 1){
	      if(!strcmp(chemins[inc].tagues[tagI].v ,"yes")){
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[8].r, style[8].g, style[8].b ,style[8].a);
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);    
	      }else
		if(!strcmp(chemins[inc].tagues[tagI].v ,"school")){
		  filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[9].r, style[9].g, style[9].b ,style[9].a);
		  aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 10, 130, 120, 255);    
		} else 
		  if(!strcmp(chemins[inc].tagues[tagI].v ,"hotel")){
	       
		    filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[10].r, style[10].g, style[10].b ,style[10].a);
		    aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 220, 20, 170, 255);    
		  } else
		    if(!strcmp(chemins[inc].tagues[tagI].v ,"church")){
		      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[11].r, style[11].g, style[11].b ,style[11].a);
		      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 10, 70, 70, 255);    
		    }else{
		
		      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[12].r, style[12].g, style[12].b ,style[12].a);
		      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 10, 70, 70, 255); 
		    }
		
	      int name = 0;
	      if(option==1){
		if(zo>500){
		  for (name=0;name < chemins[inc].sizeT ; name++){
		    if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		      {
			chemins[inc].name = chemins[inc].tagues[name].v ;
			char stre[100];
			for (name=0 ;name <100;name++){
			  stre[name]=chemins[inc].name[name];
			}
			// printf("%s",stre);
			chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
			position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
			position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
			SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
			break;    
		      } 
		  }
		}
	      }
	      break;
	    }
	  }
	  drawing =0 ;
        } 
        else if(!strcmp(chemins[inc].tagues[tagI].k ,"natural")){
	  short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4 ,angular,ang,theta);
	    s[k]=tmp[0];
	    s2[k]=tmp[1];
	    if(dessinePas(s[k],s2[k]) == 1){
	      drawing =1;
	    }
	  }  
	  if(drawing ==1){
	    if (!strcmp(chemins[inc].tagues[tagI].v ,"water")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[13].r, style[13].g, style[13].b ,style[13].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"wood")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,  style[14].r, style[14].g, style[14].b ,style[14].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"heath")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[15].r, style[15].g, style[15].b ,style[15].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"fell")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[16].r, style[16].g, style[16].b ,style[16].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"bare_rock")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[17].r, style[17].g, style[17].b ,style[17].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }  else  if (!strcmp(chemins[inc].tagues[tagI].v ,"scree")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[18].r, style[18].g, style[18].b ,style[18].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }  else  if (!strcmp(chemins[inc].tagues[tagI].v ,"shingle")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[19].r, style[19].g, style[19].b ,style[19].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"sand")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[20].r, style[20].g, style[20].b ,style[20].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"beach")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[21].r, style[21].g, style[21].b ,style[21].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"cliff")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[22].r, style[22].g, style[22].b ,style[22].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"land")){
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[23].r, style[23].g, style[23].b ,style[23].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }
	  
	    else{
	      filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN ,style[24].r, style[24].g, style[24].b ,style[24].a);
	      aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	    }
	    int name = 0;
	    if(option==1){
	      if(zo>500){
		for (name=0;name < chemins[inc].sizeT ; name++){
		  if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		    {
		      chemins[inc].name = chemins[inc].tagues[name].v ;
		      char stre[100];
		      for (name=0 ;name <100;name++){
			stre[name]=chemins[inc].name[name];
		      }
		      // printf("%s",stre);
		      chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		      position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
		      position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
		      SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		      break;    
		    } 
		}
	      }
	    }
	    break;
	  }
	  drawing =0 ;
        }
	else  if(!strcmp(chemins[inc].tagues[tagI].k ,"place")){
	  short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode ,zo, zo2, zo3, zo4,angular,ang,theta);
	    s[k]=tmp[0];
	    s2[k]=tmp[1];
	    if(dessinePas(s[k],s2[k]) == 1){
	      drawing =1;
	    }
	  }  
	  if(drawing == 1){
	    filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[25].r, style[25].g, style[25].b ,style[25].a);
	    aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);    
	    int name = 0;
	    if(option==1){
	      if(zo>500){
		for (name=0;name < chemins[inc].sizeT ; name++){
		  if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		    {
		      chemins[inc].name = chemins[inc].tagues[name].v ;
		      char stre[100];
		      for (name=0 ;name <100;name++){
			stre[name]=chemins[inc].name[name];
		      }
		      // printf("%s",stre);
		      chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		      position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
		      position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
		      SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		      break;    
		    } 
		}
	      }
	    }
	    break;
	  }
	  drawing =0 ;
        }
        
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"waterway") && !strcmp(chemins[inc].tagues[tagI].v ,"riverbank")){
	  short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	    s[k]=tmp[0];
	    s2[k]=tmp[1]; 
	    
	  }  
          filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , style[26].r, style[26].g, colar,style[26].a);
	  aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);    
	  int name = 0;
	  if(option==1){
	    for (name=0;name < chemins[inc].sizeT ; name++){
	      if(!strcmp(chemins[inc].tagues[name].k ,"name"))
	        {
		  chemins[inc].name = chemins[inc].tagues[name].v ;
		  char stre[100];
		  for (name=0 ;name <100;name++){
		    stre[name]=chemins[inc].name[name];
		  }
		  // printf("%s",stre);
		  chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		  position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
		  position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
		  SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		  break;    
	        } 
	    }
	  }
	  break;
        }
        
        else{
	  if(mode == 6 || mode == 45 || mode == 43 || mode ==8){
	    short* s = malloc((chemins[inc].sizeN)*sizeof(short));
	    short* s2= malloc((chemins[inc].sizeN)*sizeof(short)) ;
	    int k;
	    if(zo >= -120){
	      for(k=0;k<chemins[inc].sizeN ; k++){
		tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4,angular,ang,theta);
		s[k]=tmp[0];
		s2[k]=tmp[1];
		if(dessinePas(s[k],s2[k]) == 1){
		  drawing =1;
		}
	      }  
	      if(drawing ==1){
		filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 16, 52, 166,80 );
		aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 100);    
		int name = 0;
		if(option==1){
		  if(zo>500){
		    for (name=0;name < chemins[inc].sizeT ; name++){
		      if(!strcmp(chemins[inc].tagues[name].k ,"name"))
			{
			  chemins[inc].name = chemins[inc].tagues[name].v ;
			  char stre[100];
			  for (name=0 ;name <100;name++){
			    stre[name]=chemins[inc].name[name];
			  }
			  // printf("%s",stre);
			  chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
			  position.x=(s[0]+s[(chemins[inc].sizeN)/2])/2; 
			  position.y=(s2[0]+s[(chemins[inc].sizeN)/2])/2;
			  SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
			  break;    
			} 
		    }
		  }
		}
		break;
	      }
	    }
	    drawing =0 ;
	  }
        }
	
      }
    }else{
      
      if(mode == 45){
        if(chemins[inc].where == 1){
	  double *tp3 = convert(chemins[inc].noeuds[0].lat , chemins[inc].noeuds[0].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	  double *tp4 =convert(chemins[inc].noeuds[1].lat , chemins[inc].noeuds[1].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	
	  if(listsize <10){
	    list[listsize].px= (tp3[0]+tp4[0])/2 ;
	    list[listsize].py=(tp3[1]+tp4[1])/2 ;
	    list[listsize].rad=60;
	    listsize ++ ;
	  }            
        }
      }
	
      for(tagI=0; tagI < chemins[inc].sizeT ; tagI++){
        if(!strcmp(chemins[inc].tagues[tagI].k ,"highway")){
        
	  couleur[3]=SDL_MapRGB(screen->format,40,40,40); 
	  for(jin=0;jin<(chemins[inc].sizeN)-1 ; jin ++){
	    tmp=convert(chemins[inc].noeuds[jin].lat , chemins[inc].noeuds[jin].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	    deb=tmp[0];
	    deb2=tmp[1];
	    tmp=convert(chemins[inc].noeuds[jin+1].lat , chemins[inc].noeuds[jin+1].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4,angular,ang,theta );
	    fin=tmp[0];
	    fin2=tmp[1];
	  
	    // lineRGBA(screen, deb ,deb2 ,fin,fin2,0,0,0,255);
	    // linewithwidth( deb ,deb2 ,fin,fin2,0,couleur[3]);
	    /*  if(couleur[3]==SDL_MapRGB(screen->format,10,40,190)){
		aalineColor(screen,deb,deb2,fin,fin2,couleur[3]);
	
		}else */
	    //  linewithwidth(deb, deb2, fin, fin2, 2,couleur[3]);
	    if(!strcmp(chemins[inc].tagues[tagI].v ,"motorway")){
	      if(mode==8){
		if(zo >=200)
		  linewithwidth(deb, deb2, fin, fin2,  style[28].r,couleur[10]);
	      }else
		linewithwidth(deb, deb2, fin, fin2,  style[28].r,couleur[10]);
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"trunk")){
	      linewithwidth(deb, deb2, fin, fin2,  style[29].r,couleur[11]);
	    
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"primary")){
	      if(mode ==8){
		if(zo >=100)
		  linewithwidth(deb, deb2, fin, fin2,  style[30].r,couleur[12]); 
	      }else
		linewithwidth(deb, deb2, fin, fin2,  style[30].r,couleur[12]); 
	    } 
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"secondary")){
	      if(mode == 8){
		if(zo >=300)
		  linewithwidth(deb, deb2, fin, fin2,  style[31].r,couleur[13]);;
	      }else    
		linewithwidth(deb, deb2, fin, fin2,  style[31].r,couleur[13]);;
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"residential")){
	      if(mode ==8){
		if(zo >=300)
		  linewithwidth(deb, deb2, fin, fin2,  style[32].r,couleur[14]);;
	      }else
		linewithwidth(deb, deb2, fin, fin2,  style[32].r,couleur[14]);;
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"unclassified")){
	      linewithwidth(deb, deb2, fin, fin2,  style[33].r,couleur[15]);;
	
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"tertiary")){
	      if(mode ==8){
		if(zo >=500)
		  linewithwidth(deb, deb2, fin, fin2, style[34].r,couleur[16]);
	      }else
		linewithwidth(deb, deb2, fin, fin2, style[34].r,couleur[16]);
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"footway")){
	      if(mode ==8){
		if(zo >=200)
		  linewithwidth(deb, deb2, fin, fin2, style[35].r,couleur[17]);;
	      }else linewithwidth(deb, deb2, fin, fin2, style[35].r,couleur[17]);;
	    }
	    else if(!strcmp(chemins[inc].tagues[tagI].v ,"steps")){
	      linewithwidth(deb, deb2, fin, fin2, style[36].r,couleur[18]);;
	           
	    }
	    else{
	      linewithwidth(deb, deb2, fin, fin2, style[36].r,couleur[18]);
	    }
	  }
	  if(zo>100){
	    int name = 0;
	    if(option==1){
	      if(zo>-50 ){
		for (name=0;name < chemins[inc].sizeT ; name++){
		  if(!strcmp(chemins[inc].tagues[name].k ,"name"))
		    {
		      chemins[inc].name = chemins[inc].tagues[name].v ;
		      char stre[100];
		      for (name=0 ;name <100;name++){
			stre[name]=chemins[inc].name[name];
		      }
		      // printf("%s",stre);
		      chemins[inc].text = TTF_RenderText_Blended(police, stre, couleurNoire);
		      position.x=((deb+fin)/2)-50;
		      position.y=((deb2+fin2)/2)-50;
		      SDL_BlitSurface( chemins[inc].text, NULL, screen, &position);
		      break;    
		    } 
		}
	      }
	    }
	  }
	  
	
        }else if(!strcmp(chemins[inc].tagues[tagI].v ,"coastline")){
       
	  short* s = malloc((chemins[inc].sizeN+3)*sizeof(short));
	  short* s2= malloc((chemins[inc].sizeN+3)*sizeof(short)) ;
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4 ,angular,ang,theta);
	    s[k]=tmp[0];
	    s2[k]=tmp[1];
	  } 
	  s[k+1]=0;
	  s2[k+1]=0;
	  s[k+2]=0;
	  s2[k+2]=tailley;
	  s[k+3]=s[0]; 
	  s2[k+3]=s[0];
	  filledPolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 187, 172, 172 ,130);
	  //aapolygonRGBA(screen, s, s2,  chemins[inc].sizeN , 70, 70, 70, 255);   
	  break; 
        }
        
        else
	  if(!strcmp(chemins[inc].tagues[tagI].k ,"waterway")){
	    couleur[3]=SDL_MapRGB(screen->format,10,40,190); 
	  }else{
            for(jin=0;jin<(chemins[inc].sizeN)-1 ; jin ++){
	      tmp=convert(chemins[inc].noeuds[jin].lat , chemins[inc].noeuds[jin].lon , minlo ,maxlo ,minla ,mode,zo, zo2, zo3, zo4,angular,ang,theta);
	      deb=tmp[0];
	      deb2=tmp[1];
	      tmp=convert(chemins[inc].noeuds[jin+1].lat , chemins[inc].noeuds[jin+1].lon , minlo ,maxlo ,minla,mode,zo, zo2, zo3, zo4,angular,ang,theta );
	      fin=tmp[0];
	      fin2=tmp[1];
	   
	      if(couleur[3]==SDL_MapRGB(screen->format,10,40,190)){
		aalineColor(screen,deb,deb2,fin,fin2,couleur[3]);
	
	      }else 
		//  linewithwidth(deb, deb2, fin, fin2, 2,couleur[3]);
		aalineRGBA(screen,deb,deb2,fin,fin2,80,100,100,200);
	 
	    }
	  }
      }
    }

  }
  //filledCircleRGBA(screen,100,100,100,200,0,00,5);
}

//We define order firstable water then earth
void order(conteneur *chemins ,double minla ,double maxla ,double minlo ,double maxlo,int mode,int zo,int zo2,int zo3,int zo4 ,int angular,int ang,int theta){

  if(chemins[0].numba !=0)
    dessin(chemins[0].cheminss, chemins[0].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
 
  if(chemins[6].numba !=0)
    dessin(chemins[6].cheminss, chemins[6].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta); 
  if(chemins[2].numba !=0)
    dessin(chemins[2].cheminss, chemins[2].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  if(chemins[5].numba !=0)
    dessin(chemins[5].cheminss, chemins[5].numba,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);  
  if(chemins[1].numba !=0)
    dessin(chemins[1].cheminss, chemins[1].numba,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  if(chemins[4].numba !=0)
    dessin(chemins[4].cheminss, chemins[4].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  if(chemins[3].numba !=0)
    dessin(chemins[3].cheminss, chemins[3].numba,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[7].cheminss, chemins[7].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[8].cheminss, chemins[8].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[9].cheminss, chemins[9].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[10].cheminss, chemins[10].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[13].cheminss, chemins[13].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[11].cheminss, chemins[11].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[12].cheminss, chemins[12].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[14].cheminss, chemins[14].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);  
  dessin(chemins[15].cheminss, chemins[15].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  dessin(chemins[16].cheminss, chemins[16].numba ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
  if(mode ==45)
    {
      zonage();
    }
}
 
void update(double minla ,double maxla ,double minlo ,double maxlo){
  double delta_x_meter = lon_to_x(maxlo) - lon_to_x(minlo);
  double delta_y_meter = lat_to_y(maxla) - lat_to_y(minla);
  double coeff = delta_x_meter / delta_y_meter;
  
  tailley=700;
  taillex = 700*coeff ;

}

//version 2 of printing map
void rendu2(conteneur *chemins,int size ,double minla ,double maxla ,double minlo ,double maxlo,int mode,int zo,int zo2,int zo3,int zo4 ,int angular,int ang,int theta,colour *styl,char *fi){
        
  int boolean2=0 ;
  FILE *fichier = NULL;
  if(mode==43){
    FILE* fichier2 = NULL; 
    fichier2 = fopen(fi, "w");
    fputs("",fichier2);
    fclose(fichier2); 
       
    fichier = fopen(fi, "a+");
  }else {
    boolean2 =0 ;
  }
  style=styl ;     
  //initialisation de base
  SDL_Init(SDL_INIT_VIDEO);
  SDL_WM_SetCaption("Renderer OpenStreetMap", NULL);
  putenv("SDL_VIDEO_WINDOW_POS=center");
  update( minla ,maxla ,minlo ,maxlo);
  screen=SDL_SetVideoMode(taillex,tailley,32, SDL_HWSURFACE|SDL_DOUBLEBUF); 
 
  couleur[0]=SDL_MapRGB(screen->format,255,255,255); //white
  couleur[1]=SDL_MapRGB(screen->format,25,255,25); 
  couleur[2]=SDL_MapRGB(screen->format,25,129,25); 
  couleur[4]=SDL_MapRGB(screen->format,255,25,25); 
  couleur[3]=SDL_MapRGB(screen->format,0,0,0); // black
  SDL_Surface *texte =NULL; 
  SDL_Rect position;
  TTF_Init();
  police = TTF_OpenFont("police/angelina.TTF", 20);
 
 
  SDL_Gif* gif;
 
  SDL_Surface* tmp2;
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
      printf("%s", Mix_GetError());
    }
  Mix_VolumeMusic(MIX_MAX_VOLUME / 2); 
  Mix_Music *musique; 
  musique = Mix_LoadMUS("sound/world.mp3");  
  Mix_PlayMusic(musique, 1); 
 
  if(mode !=2 )
    // SDL_FillRect(screen,0,couleur[0]); 
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 17, 206,112));
  else{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
      {
	printf("%s", Mix_GetError());
      }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2); 
    Mix_Music *musique; 
    musique = Mix_LoadMUS("sound/calib.wav"); 
    Mix_PlayMusic(musique, 1); 
   
   
    gif = SDLLoadGif("holo.gif");
    //while(!KeyHit()){
    tmp2 = SDLGifAutoFrame(gif);
    SDL_BlitSurface(tmp2,NULL,screen,NULL);
    SDL_Flip(screen);
  }
  
  //here 
  dessin(chemins[6].cheminss, chemins[6].numba ,minla ,maxla , minlo ,maxlo, mode,zo,zo2,zo3,zo4 ,360,180,0 );
  // SDL_SaveBMP(screen,"out.bmp");
  
  if(mode ==2 ){
    while(!KeyHit()){
      tmp2 = SDLGifAutoFrame(gif);
      SDL_BlitSurface(tmp2,NULL,screen,NULL);
      SDL_Flip(screen);
    }
  }
  
  if(mode==3){  
    SDL_SaveBMP(screen,"out.bmp");
    rendu3() ;
    // SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
  }else{
    //pause2();  
    SDL_Event event;
    order(chemins ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
    int continuer = 1;
    char str[100];
    int boo=0;
    position.x = 0;
    position.y = 40;
   
  
    while(continuer)
      {
	/*if(colar<240)
	  colar+=5;
	  else
	  colar =100;*/
	if(mode == 44){
	  int c2 =1 ;
	  
          char caractere = 0;
          FILE * fiche = NULL;
         
          fiche = fopen(fi, "r");
	  while(c2){
	
	    if(mer==0)
	      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 187, 172, 172));
	    else
	      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 7, 2, 170));
	    order(chemins ,minla , maxla ,minlo , maxlo,6,zo,zo2,zo3,zo4,angular,ang,theta);
	    caractere = fgetc(fiche);
	    if(caractere == EOF) break ;
	    switch(caractere){
	    case '1':zo4+=80;
	      break ;
	    case '2':  zo3+=80;break ;
	    case '3':
	      zo3-=80;
	      break;
	    case'4':
	      zo4-=80;
	      break;
	    case 'a':
	      zo+=100;
	      zo2+=100;
	      zo3-=50;
	      zo4-=50;
	      break;
	    case 'q':
	      if(angular <360){
		angular+=20;
		zo4+=20;
	      }
	      break;
	    case 's' :
	      if(angular>0){
		angular-=20 ;
		zo4-=20;
	      }
	      break;
	    case'z':
	      if(zo >=-500){
		zo-=100;
		zo2-=100;
		zo3+=50;
		zo4+=50;
	      }
	      break ;
	    case 'o':
	      zo=0;
	      zo2=0;
	      zo3=0;
	      if(boo==0)
		zo4=0;
	      else
		zo4=700;
	      angular=360;
	      break;
	    case 'b' :
	      boo=1;
	      zo4=-700;
	      ang =-180;
	      break;
	    case 'n':
	      boo=0;
	      zo4=0;
	      ang =180;
	      break;
	    case 'p':option=1;
	      break; 
	    case 'm' :
	      option =0 ;
	      break;
	    default : ; 
	    }
	    SDL_Delay(100);
	    SDL_Flip(screen);
	 
	  }
	  continuer = 0 ;
	}
     
	
	if(mer==0)
	  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 187, 172, 172));
	else
	  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 7, 2, 170));
	
	order(chemins ,minla , maxla ,minlo , maxlo,mode,zo,zo2,zo3,zo4,angular,ang,theta);
	     
	SDL_PollEvent(&event);
	switch(event.type)
	  {   
	  case SDL_QUIT:
	    continuer = 0;
	    break; 
	  case SDL_KEYDOWN:
	    if(boolean2 ==0){
	      switch(event.key.keysym.sym)
		{
	      
		case SDLK_ESCAPE :
		  continuer = 0;
		  break;
		case SDLK_z :
		  zo4+=80;
		  if(strcmp(fi ,"")){
		    fputs("1", fichier);
		  }
		  break;
		case SDLK_q :
		
		  SDL_Flip(screen);
		  zo3+=80;
		  if(strcmp(fi ,"")){
		    fputs("2", fichier);
		  }
		  break;
		case SDLK_d :
		  zo3-=80;
		  if(strcmp(fi ,"")){
		    fputs("3", fichier);
		  }
		  break;
		case SDLK_s :
		  zo4-=80;
		  if(strcmp(fi ,"")){
		    fputs("4", fichier);
		  }
		  break;
		case SDLK_UP :
	      
		  zo+=100;
		  zo2+=100;
		  zo3-=50;
		  zo4-=50;
		  if(strcmp(fi ,"")){
		    fputs("a",fichier);
		  }
		  break;
		
		case SDLK_LEFT:
		  if(angular <360){
		    angular+=20;
		    zo4+=20;
		  }
		  if(strcmp(fi ,"")){
		    fputs("q",fichier);
		  }
		  break;
		case SDLK_RIGHT:
		  if(angular>0){
		    angular-=20 ;
		    zo4-=20;
		  }
		  if(strcmp(fi ,"")){
		    fputs("s",fichier);
		  }
		  break;
		case SDLK_DOWN :
		  //if(zo>100){
		  if(zo >=-200){
		    zo-=100;
		    zo2-=100;
		    zo3+=50;
		    zo4+=50;
		  }
		  if(strcmp(fi ,"")){
		    fputs("z",fichier);
		  }
		  //}else{
		  //zo3=0;
		  //zo4=0;
		  //}		
		  break;
	    
		case SDLK_o :
		  zo=0;
		  zo2=0;
		  zo3=0;
		  if(boo==0)
		    zo4=0;
		  else
		    zo4=700;
		  angular=360;
		  if(strcmp(fi ,"")){
		    fputs("o",fichier);
		  }
		 
		  break ;
		
		case SDLK_t :
		  theta+=40;
		  
		  //if(theta>=360)
		  // theta=0;
		  break;
		case SDLK_y :
		  theta-=1 ;
		  break;
		
		case SDLK_b:
		  boo=1;
		  zo4=-700;
		  ang =-180;
		  if(strcmp(fi ,"")){
		    fputs("b",fichier);
		  }
		  break;
		case SDLK_n:
		  boo=0;
		  zo4=0;
		  ang =180;
		  if(strcmp(fi ,"")){
		    fputs("n",fichier);
		  }
		  break;
		case SDLK_p:
		  //name street
		  option=1;
		  if(strcmp(fi ,"")){
		    fputs("p",fichier);
		  }
		  break;
		case SDLK_m:
		  option=0;
		  if(strcmp(fi ,"")){
		    fputs("m",fichier);
		  }
		  break;
		default : ;
		}
	    }
	   
	    break; 
	
	    //  default :/*do nothing*/;break;
	  }
	
	if(mode ==4 || mode == 45 || mode ==6 || mode == 44 || mode == 8){
	  sprintf(str, "[ cordX : %d cordY : %d Angular : %d Zoomage : %d ]",zo3,zo4,angular,zo);
	  texte = TTF_RenderText_Blended(police, str, couleurNoire);
	  
	  SDL_BlitSurface(texte, NULL, screen, &position); /* Blit du texte */
                
        }
	SDL_Flip(screen); 
	
      }
     
    SDL_SaveBMP(screen,"sortie.bmp");
  }
  //free(chemins);
  return;
}

/******************Mathematics Part*********************************/


double radians(double degrees){
  return degrees * (M_PI/180);
}

double lat_to_y(double lat) { 
  int earth=6400000;
  return earth * log(tan(M_PI/4 + radians(lat)/2));
}

double lon_to_x(double lon) {
  int earth=6400000; 
  return radians(lon) * earth;
}

/*
  Fonction permettant la conversion et les devers deplacements
*/
double* convert(double lat ,double lon ,double mapLonLeft,double mapLonRight, double mapLatBottom ,int mode,int zo,int zo2,int zo3,int zo4,int angular,int ang,int theta){
  int mapWidth = taillex+zo ;   
  int mapHeight = tailley+zo2;
  double mapLonDelta = mapLonRight - mapLonLeft;
  
  double co =1;
  if(mode == 2){
    co =0.7;
  }
  if(mode == 0){
    co =1 ;
  }  
  double mapLatBottomDegree = mapLatBottom *co* 3.141592654 / ang;
  double x = (lon - mapLonLeft) * (mapWidth / mapLonDelta);
  lat = lat *co* 3.141592654 / ang;
  double worldMapWidth = ((mapWidth / mapLonDelta) * angular) / (2 * 3.141592654);
  double mapOffsetY = (worldMapWidth / 2 * log((1 + sin(mapLatBottomDegree)) / (1 - sin(mapLatBottomDegree))));
  double y = mapHeight - ((worldMapWidth / 2 * log((1 + sin(lat)) / (1 - sin(lat)))) - mapOffsetY);
  
  double* tab = malloc(2*sizeof(double)); 

  //  double tempX = x - (taillex/2);
  // double tempY = y - (tailley/2);  
  //int theta = 0 ;      
  /*double rotatedX =0;
    double rotatedY = 0;
    if(zo==0 && zo2==0 && zo3==0 && zo4==0){}else{
    //rotation matrix
    rotatedX = tempX*cos(theta) + tempY*sin(theta);
    rotatedY = tempX*sin(theta) + tempY*cos(theta);
   
    }*/
  // tab[0]=x+rotatedX +zo3 ;
  //tab[1]=y+rotatedY +zo4;
 
  tab[0]=(x+zo3);
  tab[1]=(y+zo4);
  return tab;
}
