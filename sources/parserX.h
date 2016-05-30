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
#include <glib.h>



/*
Structures contains ways nodes and k/v of tags
*/
//Tag
typedef struct tag{
  char* k ; 
  char*  v ;
}tag ;

//Node
typedef struct noeud{
  double lat ;
  double lon ;
  long id ;
  char* visible ;
  tag *tagues ; 
  int sizeT ;
}noeud  ;


//ways
typedef struct ways{
  long id ;
  char* visible ;
  noeud *noeuds  ;
  int sizeN ;
  tag *tagues ;
  int sizeT ;
  char *name ;
  SDL_Surface*  text;
  SDL_Rect position;
  long num ;
  int where ;
}ways ;


//orderer
typedef struct conteneur{
  int numba ;
  ways* cheminss ;
}conteneur ;



typedef struct relation{
  ways * chem ;
}relation ;


//Hash1
typedef struct struct_process {
    long id;
    noeud file_to_process;
}struct_process;

typedef struct struct_process Process;




//Hash 2
typedef struct struct_process2{
    long id ;
    ways file_to_process2 ;
}struct_process2 ;

typedef struct struct_process2 Process2;




SDL_Surface * screen;


//pour plus tard les couleurs..
Uint32 white, black, couleur[100]; 



int nbelement (xmlNodePtr cur ,const xmlChar* str);
noeud refNoeud(noeud* nodeu ,int n, long ref);



