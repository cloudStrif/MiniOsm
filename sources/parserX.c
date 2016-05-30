/*
 * Copyright (C) <2015>  <chenp>
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <SDL_gfxPrimitives.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_rotozoom.h>
#include <curl/curl.h>
#include <glib.h>
  


#include "DrawSdl1.h" 
#include "Xgraphics.h"




char *file ="";
char *seek ="";
FILE *fichier =NULL;

//compte le nombre de balise str(seulement les fils dun noeud).
/**
 * \brief count the number of elements
 * \details linear count
 *          
 * \param cur is a node
 * \param str the name
 * \return Un error, à valeur -1 si le disque n'existe pas, 0 sinon.
 */
int nbelement (xmlNodePtr cur ,const xmlChar* str){
  int c = 0 ;
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *) str))) {
      c++ ;	  
    } 
    cur = cur->next;
  }
  return c ;
}

/**
 * old fonction finding nodes
 */
noeud refNoeud(noeud* nodeu ,int n, long ref){
  //donne a <way> le bon noeud ,penser a desalouer la memoire des <node> ...
  int i ;
  for(i=0 ; i<n ; i++){
    if(ref == nodeu[i].id){
      return nodeu[i];
    }
  }
  //ce cas n arrive jamais si le .osm est bien de openstreetmap 
  return nodeu[0];
}


/**
 *convert a image into SVG format
 */
void devienSvg(ways *chemins,int size,double minla ,double maxla ,double minlo ,double maxlo,colour * colo){

  int inc  =0;
  //int jin =0 ;
  int tagI =0;
  /*short deb =0 ; 
    short deb2=0 ;
    short fin =0 ; 
    short fin2=0 ;*/
  double* tmp =malloc(2*sizeof(double));

  
  for(inc=0 ; inc < size ; inc++){  
    if(chemins[inc].noeuds[0].lat == chemins[inc].noeuds[(chemins[inc].sizeN -1)].lat && chemins[inc].noeuds[0].lon == chemins[inc].noeuds[(chemins[inc].sizeN -1)].lon ){
    
     
      for(tagI=0; tagI < chemins[inc].sizeT ; tagI++){
	if(!strcmp(chemins[inc].tagues[tagI].k ,"landuse")) 
	  {
	    char line [1000]="<polygon points='";
	    fputs(line,fichier);
	    int k;
	    for(k=0;k<chemins[inc].sizeN ; k++){
	      tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	      char line3 [1000] ;
	      sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	      fputs(line3,fichier);
	    }
	
	    if(!strcmp(chemins[inc].tagues[tagI].v ,"allotments")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[0].r,colo[0].g,colo[0].b);
	      fputs(line2,fichier); 
	    }else if(!strcmp(chemins[inc].tagues[tagI].v ,"basin")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[1].r,colo[1].g,colo[1].b);
	      fputs(line2,fichier); 
	    }else if(!strcmp(chemins[inc].tagues[tagI].v ,"brownfield")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[2].r,colo[2].g,colo[2].b);
	      fputs(line2,fichier); 
	    }else if(!strcmp(chemins[inc].tagues[tagI].v ,"cemetery")){
	      char line2 [1000];  
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90;0.5)'/>\n",colo[3].r,colo[3].g,colo[3].b);
	      fputs(line2,fichier); 
	    }else if(!strcmp(chemins[inc].tagues[tagI].v ,"commercial")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[4].r,colo[4].g,colo[4].b);
	      fputs(line2,fichier); 
	    }else if(!strcmp(chemins[inc].tagues[tagI].v ,"construction")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[5].r,colo[5].g,colo[5].b);
	      fputs(line2,fichier); 
	    }else{
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgb(90,90,90,0.5)'/>\n",colo[6].r,colo[6].g,colo[6].b);
	      fputs(line2,fichier); 
	
	    }
	    break;
	  }
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"leisure") ){
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  char line2 [1000];
	  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[7].r,colo[7].g,colo[7].b);
	  fputs(line2,fichier); 
	  break ;
	}
	  
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"building")){
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  if(!strcmp(chemins[inc].tagues[tagI].v ,"yes")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,0.6);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[8].r,colo[8].g,colo[8].b);
	    fputs(line2,fichier);      
	  }else
	    if(!strcmp(chemins[inc].tagues[tagI].v ,"school")){
	      char line2 [1000];
	      sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[9].r,colo[9].g,colo[9].b);
	      fputs(line2,fichier);    
	    } else 
	      if(!strcmp(chemins[inc].tagues[tagI].v ,"hotel")){
		char line2 [1000];
		sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[10].r,colo[10].g,colo[10].b);
		fputs(line2,fichier);
	      } else
		if(!strcmp(chemins[inc].tagues[tagI].v ,"church")){
		  char line2 [1000];
		  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[11].r,colo[11].g,colo[11].b);
		  fputs(line2,fichier);
		}else{
		  char line2 [1000];
		  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[12].r,colo[12].g,colo[12].b);
		  fputs(line2,fichier);
		}
	    
	  break;
	}
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"natural")){
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  if (!strcmp(chemins[inc].tagues[tagI].v ,"water")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[13].r,colo[13].g,colo[13].b);
	    fputs(line2,fichier); 
	  } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"wood")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[14].r,colo[14].g,colo[14].b);
	    fputs(line2,fichier);  
	  }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"heath")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[15].r,colo[15].g,colo[15].b);
	    fputs(line2,fichier);  
	  } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"fell")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[16].r,colo[16].g,colo[16].b);
	    fputs(line2,fichier);
	  } else  if (!strcmp(chemins[inc].tagues[tagI].v ,"bare_rock")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[17].r,colo[17].g,colo[17].b);
	    fputs(line2,fichier);  
	  }  else  if (!strcmp(chemins[inc].tagues[tagI].v ,"scree")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[18].r,colo[18].g,colo[18].b);
	    fputs(line2,fichier);
	  }  else  if (!strcmp(chemins[inc].tagues[tagI].v ,"shingle")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[19].r,colo[19].g,colo[19].b);
	    fputs(line2,fichier);  
	  }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"sand")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[20].r,colo[20].g,colo[20].b);
	    fputs(line2,fichier);  
	  }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"beach")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[21].r,colo[21].g,colo[21].b);
	    fputs(line2,fichier); 
	  }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"cliff")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[22].r,colo[22].g,colo[22].b);
	    fputs(line2,fichier); 
	  }else  if (!strcmp(chemins[inc].tagues[tagI].v ,"land")){
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[23].r,colo[23].g,colo[23].b);
	    fputs(line2,fichier);
	  }
	  
	  else{
	    char line2 [1000];
	    sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[24].r,colo[24].g,colo[24].b);
	    fputs(line2,fichier);
	  }
	    
	  break;
	}   
	    
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"place")){
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  char line2 [1000];
	  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[25].r,colo[25].g,colo[25].b);
	  fputs(line2,fichier);
	  break;
	}
	
	else if(!strcmp(chemins[inc].tagues[tagI].k ,"waterway") && !strcmp(chemins[inc].tagues[tagI].v ,"riverbank")){
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  char line2 [1000];
	  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[26].r,colo[26].g,colo[26].b);
	  fputs(line2,fichier);
	
	}else{
	  char line [1000]="<polygon points='";
	  fputs(line,fichier);
	  int k;
	  for(k=0;k<chemins[inc].sizeN ; k++){
	    tmp=convert(chemins[inc].noeuds[k].lat , chemins[inc].noeuds[k].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
	    char line3 [1000] ;
	    sprintf(line3,"%d,%d ",(int)tmp[0],(int)tmp[1]);
	    fputs(line3,fichier);
	  }
	  char line2 [1000];
	  sprintf(line2,"'  style='fill:rgba(%d,%d,%d,1);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",colo[26].r,colo[26].g,colo[26].b);
	  fputs(line2,fichier);
	}
      }
    }/*else{
       for(tagI=0; tagI < chemins[inc].sizeT ; tagI++){
       if(!strcmp(chemins[inc].tagues[tagI].k ,"highway")){
	
       for(jin=0;jin<(chemins[inc].sizeN)-1 ; jin ++){
       tmp=convert(chemins[inc].noeuds[jin].lat , chemins[inc].noeuds[jin].lon , minlo ,maxlo ,minla ,1,2,2,0,0,360,180,0);
       deb=tmp[0];
       deb2=tmp[1];
       tmp=convert(chemins[inc].noeuds[jin+1].lat , chemins[inc].noeuds[jin+1].lon , minlo ,maxlo ,minla,1,2,2,0,0,360,180,0);
       fin=tmp[0];
       fin2=tmp[1];
	    
	    
                
       if(!strcmp(chemins[inc].tagues[tagI].v ,"motorway")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:7;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);
       }
       else if(!strcmp(chemins[inc].tagues[tagI].v ,"trunk")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);
       }
       else if(!strcmp(chemins[inc].tagues[tagI].v ,"primary")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:6;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);  }
       else if(!strcmp(chemins[inc].tagues[tagI].v ,"secondary")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:6;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);}
       else if(!strcmp(chemins[inc].tagues[tagI].v ," residential")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:7;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);
       }
       else if(!strcmp(chemins[inc].tagues[tagI].v ,"unclassified")){
       char line [1000]="<polygon points='";
       fputs(line,fichier);
       char line2 [1000];
       sprintf(line2,"%d,%d %d,%d'  style='fill:rgb(%d,%d,%d);stroke-width:3;stroke:rgba(90,90,90,0.5)'/>\n",(int)deb,(int)deb2,(int)fin,(int)fin2,colo[26].r,colo[26].g,colo[26].b);
       fputs(line2,fichier);}
       else{
	
       }
       break;   
       }
       }else break ;
       }
       }*/
  }
 
}

void ordre (conteneur *chemins,int size,double minla ,double maxla ,double minlo ,double maxlo,colour * colo){
  FILE* fichier2 = NULL; 
  double delta_x_meter = lon_to_x(maxlo) - lon_to_x(minlo);
  double delta_y_meter = lat_to_y(maxla) - lat_to_y(minla);
  double coeff = delta_x_meter / delta_y_meter;
  int tx = 700 ;
  int ty = 700*coeff ;
  fichier2 = fopen("dessin.svg", "w");
  fputs("",fichier2);
  fclose(fichier2);
  //FILE* fichier = NULL; 
  fichier = fopen("dessin.svg", "a+");
  char ligne[1000];
  update( minla ,maxla ,minlo ,maxlo);
  sprintf(ligne ,"<?xml version='1.0' encoding='utf-8'?>\n<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='%d' height='%d'>\n<title>Map SVG</title>\n",ty,tx);   
  fputs(ligne, fichier);
  
  devienSvg(chemins[0].cheminss ,chemins[0].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[6].cheminss ,chemins[6].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[2].cheminss ,chemins[2].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[5].cheminss ,chemins[5].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[1].cheminss ,chemins[1].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[4].cheminss ,chemins[4].numba, minla ,maxla , minlo , maxlo, colo);
  devienSvg(chemins[3].cheminss ,chemins[3].numba, minla ,maxla , minlo , maxlo, colo);
       
  fputs("</svg>",fichier);
  fclose(fichier);
  printf("Fic is now created");

}

/**
   Fonction  permettant de voir 
*/
int hashtag(){
  int l =0 ;
  while(seek[l] !='\0'){
    if(seek[l] ==':')
      return 1 ;
    l++;
  }
  return 0 ;
}

//fonction de parsing de OSM
static void parseDoc(char *docname,int version) {

  xmlDocPtr doc;
  xmlNodePtr cur;
  
  double minlat ;double minlon;
  double maxlat ; double maxlon;
  doc = xmlParseFile(docname);
  //Toutes les verifications basiques
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully. \n");
    return;
  }
  cur = xmlDocGetRootElement(doc); 
  if (cur == NULL) {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return;
  }  
  if (xmlStrcmp(cur->name, (const xmlChar *) "osm")) {
    fprintf(stderr,"document of the wrong type not openstreetMap xml");
    xmlFreeDoc(doc);
    return;
  }
  
  cur = cur->xmlChildrenNode;
  //on va dans le noeud juste apres osm
  //on s'occupe d'abord des balises nodes du debut
  
  Process * pp;
  Process2 * pp2 ;
  //tous les noeuds de depart
  noeud *nodeu =NULL; 
  //les chemins
  ways  *chemins =NULL;
  
  relation *relations =NULL;
  
  conteneur * container = malloc(27*sizeof(conteneur));

  int nombre =(nbelement(cur->parent ,(const xmlChar *) "node"));//size
  int nombre2=(nbelement(cur->parent, (const xmlChar *)"way"));//size
  int nombre3=(nbelement(cur->parent, (const xmlChar *)"relation"));
  
  int in=0; 
  
  for (;in<27;in++){
    container[in].numba=0;
    container[in].cheminss = malloc((nombre2)*sizeof(ways));
  }
  in=0;
  //printf("%d",nombre3);
  //allocation en memoire
  nodeu = malloc(nombre *sizeof(noeud)); 
  chemins = malloc(nombre2*sizeof(ways));
  relations = malloc(nombre3*sizeof(ways));
  GHashTable* hash_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
  GHashTable* hash_table2 = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
  int incremente = 0 ;
  int incremente2 = 0 ;
  int incremente3 = 0 ;
  int count =0 ;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"bounds"))){ 
      //utile pour le parametrage de la fenetre du renderer
      minlat=(double)(atof((char*)xmlGetProp(cur,(const xmlChar*)"minlat")));
      minlon=(double)(atof((char*)xmlGetProp(cur,(const xmlChar*)"minlon")));
      maxlat=(double)(atof((char*)xmlGetProp(cur,(const xmlChar*)"maxlat"))); 
      maxlon=(double)(atof((char*)xmlGetProp(cur,(const xmlChar*)"maxlon")));
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"node"))){     
    
      if(cur->xmlChildrenNode == NULL){
	//si pas de balise <tag>
	tag * t ;
        t =malloc(sizeof(tag));
        t->v="vide";
        t->k="vide";
        nodeu[incremente].tagues=t ;
        nodeu[incremente].sizeT = 0 ;
      }else{
	//dans le cas contraire
	int n = nbelement(cur, (const xmlChar*)"tag");
	tag* t = malloc(n*sizeof(tag));
	
        xmlNodePtr cur2 = cur ;
	cur2=cur2->xmlChildrenNode ;
	int in=0 ;
	cur2=cur2->next;
        
	for(;in<n ;in++){
	  //  printf("%s ll \n", cur2->name);
	  if ((!xmlStrcmp(cur2->name, (const xmlChar *)"tag"))){              
	    t[in].v = (char*)xmlGetProp(cur2,(const xmlChar*)"v");
	    t[in].k = (char*)xmlGetProp(cur2,(const xmlChar *)"k");
	   
	  }
	  cur2=cur2->next;
	  cur2=cur2->next;
	}	
	nodeu[incremente].tagues=t ;
	nodeu[incremente].sizeT = n ;
	/*if(n>=3)  
	  printf("%s \n\n\n",nodeu[incremente].tagues[2].v);*/
      }
      nodeu[incremente].lat = atof((char*)xmlGetProp(cur,(const xmlChar*)"lat"));
      nodeu[incremente].lon= atof((char*)xmlGetProp(cur,(const xmlChar*)"lon"));
      nodeu[incremente].id = atol((char*)xmlGetProp(cur , (const xmlChar*)"id"));
      //   nodeu[incremente].visible=(char*)xmlGetProp(cur,(const xmlChar*)"visible");
     
      pp=(Process*)(malloc(sizeof(Process)));
      pp->id=nodeu[incremente].id;
      pp->file_to_process=nodeu[incremente] ;
      g_hash_table_insert(hash_table, GINT_TO_POINTER(pp->id), GINT_TO_POINTER(pp));
       
      
      
      incremente ++ ;      
    }
    
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"way"))){   
      //  chemins[incremente2].visible =(char*) xmlGetProp(cur,(const xmlChar*)"visible");
      chemins[incremente2].id = atol((char*)xmlGetProp(cur,(const xmlChar*)"id"));
      xmlNodePtr cur3 = cur ;
      int num = nbelement(cur,(const xmlChar *)"nd");//size du tab1
       
      int num2 = nbelement(cur,(const xmlChar *)"tag");
      chemins[incremente2].sizeN=num;
      chemins[incremente2].sizeT=num2;
      noeud *listeNode = malloc(num*sizeof(noeud));
      tag *listeTag = malloc(num2*sizeof(tag));
      cur3=cur3->xmlChildrenNode ;//on descend dans l arbre
      cur3=cur3->next;//on passe le champs text
      // printf("%s\n\n",cur2->name);
      int ni=0;int ni2 = 0 ;
      for(;ni<num ; ni++)
	{
	  long value=atol((char*)xmlGetProp(cur3 ,(const xmlChar*)"ref"));
	
	  Process* p3 = (Process*)(g_hash_table_lookup(hash_table, GINT_TO_POINTER(value))); 
	
	  listeNode[ni]= p3->file_to_process;
	  cur3=cur3->next;
	  cur3=cur3->next;        
	}
      int booleanix = 0 ;
      chemins[incremente2].where = 0 ;
      for(;ni2<num2;ni2++)
	{
	  listeTag[ni2].k=(char*)xmlGetProp(cur3,(const xmlChar*)"k");
	  listeTag[ni2].v=(char*)xmlGetProp(cur3,(const xmlChar*)"v");
	  if(!strcmp(listeTag[ni2].v,"coastline")){
	    res2(1);
	  }
	  if(version == 45){	        
	    if(!strcmp(seek ,listeTag[ni2].k) || !strcmp(seek ,listeTag[ni2].v) ){
	      chemins[incremente2].where =1;
	      count ++ ;
	    } 
	  }
	  if(booleanix==0){
	    if(!strcmp(listeTag[ni2].k ,"waterway")){
	      in = 0 ;booleanix=1;
	    }else  if(!strcmp(listeTag[ni2].k ,"landuse")){
	      in = 1 ;booleanix=1;
	    }else  if(!strcmp(listeTag[ni2].k ,"leisure")){
	      in = 2 ;booleanix=1;
	    }else  if(!strcmp(listeTag[ni2].k ,"building")){
	      in = 3 ;booleanix=1;
	    }
	    else  if(!strcmp(listeTag[ni2].k ,"natural")){
	      in = 4 ;booleanix=1;
	    } 
	    else  if(!strcmp(listeTag[ni2].k ,"place")){
	      in = 5 ;booleanix=1;
	    }
	    else if(!strcmp(listeTag[ni2].k ,"highway")){
	   
	       
	      if(!strcmp(listeTag[ni2].v ,"motoray")){
		in = 7 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"trunk")){
		in = 8 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"primary")){
		in = 9 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"secondary")){
		in = 10 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"residential")){
		in = 11 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"unclassified")){
		in = 12 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"tertiary")){
		in = 13 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"footway")){
		in = 14 ;booleanix=1;
	      }else if(!strcmp(listeTag[ni2].v ,"steps")){
		in = 15 ;booleanix=1;
	      }else{
		in = 16 ;booleanix=1;
	      }
	     
	      
	    }
	    else{
	      in=6;booleanix=1;
	    }
	  }
	  cur3=cur3->next;
	  cur3=cur3->next;
	}
      
      chemins[incremente2].noeuds=listeNode ;
      chemins[incremente2].tagues=listeTag ;
      chemins[incremente2].num=incremente2 ;
     
      container[in].cheminss[container[in].numba] =chemins[incremente2]; 
      container[in].numba++ ;
	
      pp2=(Process2*)(malloc(sizeof(Process2)));
      pp2->id=chemins[incremente2].id;
      pp2->file_to_process2=chemins[incremente2] ;
      g_hash_table_insert(hash_table2, GINT_TO_POINTER(pp2->id), GINT_TO_POINTER(pp2));
      // printf("id way: %ld\n",chemins[incremente2].id);   
      incremente2 ++ ;
    }
    
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"relation"))){  
      xmlNodePtr cur4 = cur ;
      int num = nbelement(cur4,(const xmlChar *)"member");
      ways* cheme = malloc(num*sizeof(ways));
      cur4=cur4->xmlChildrenNode ;
     
      cur4=cur4->next ;
    
      int nn=0;
      int boolean = 0 ;
      for(nn=0;nn<num ; nn++)
	{
	  if ((!xmlStrcmp(cur4->name, (const xmlChar *)"member"))  && !xmlStrcmp(xmlGetProp(cur4,(const xmlChar *)"type"),(const xmlChar *)"way") &&   (!xmlStrcmp(xmlGetProp(cur4,(const xmlChar *)"role"),(const xmlChar *)"inner") || !xmlStrcmp(xmlGetProp(cur4,(const xmlChar *)"role"),(const xmlChar *)"outer") ) ){
        
	    long value=atol((char*)xmlGetProp(cur4 ,(const xmlChar*)"ref"));
	    Process2* p4 = (Process2*)(g_hash_table_lookup(hash_table2, GINT_TO_POINTER(value))); 
	    if (p4 == NULL) {
              
	      boolean = 1 ;
	      break ;
	    } else {
              cheme[nn] = p4->file_to_process2 ;
	    }
	  }
        
	  cur4=cur4->next ;
          cur4=cur4->next ; 
        }
      if(boolean != 1){
        //all's ok
	relations[incremente3].chem = cheme ;
	incremente3++ ;       
      }
    }
    
    cur = cur->next;
  }/*
     int kk=0 ; 
     for (kk=0; kk< incremente3 ; kk++){
     printf("verif %ld\n",relations[kk].chem[0].id);
     }*/
  printf("%d" ,count);
  res(count);
 
  g_hash_table_destroy(hash_table);
  g_hash_table_destroy(hash_table2);
  // printf("%d",incremente2);
  if(incremente2 < 10000 && version == 0){
    version =4 ;
  }
  // colour *couleur ;
   
  colour* style = parseColour("style/style.xml");
  free(nodeu);//liberation de la memoire libre
  xmlFreeDoc(doc);
  
  
  if(version == 42){
    //do it
    ordre(container ,nombre2,minlat ,maxlat , minlon ,maxlon,style);
    // devienSvg(chemins,nombre2,minlat ,maxlat , minlon ,maxlon,style);
  }else if(version==1)
    rendu(chemins,nombre2,minlat,maxlat,minlon,maxlon);
  else 
    rendu2(container,nombre2,minlat,maxlat,minlon,maxlon,version,2,2,0,0,360,180,0,style,file);
  // free(chemins); 
  
  
  return;
}



int main(int argc, char **argv) {
  
  char *docname;
  res2(0);
  
  if (argc <= 1) {
    printf("Usage: %s docname\n", argv[0]);
    return(0);
  }
  //By default ,we can move and zoom and rotate
  if(argc == 3){
    if(!strcmp(argv[2],"--past")){
      docname = argv[1];
      parseDoc (docname,1);
      return(0);
    }
    if(!strcmp(argv[2],"--futur")){
      docname = argv[1];
      parseDoc (docname,2);
      return (0);
    }
    if(!strcmp(argv[2],"--present")){
      docname = argv[1];
      parseDoc (docname,8);
      return (0);
    }
    if(!strcmp(argv[2],"-p")){
      docname = argv[1];
      parseDoc (docname,6);
      return (0);
    }
    if(!strcmp(argv[2],"--svg")){
      docname = argv[1];   
      parseDoc (docname,42);
      return (0);         
    }
    if(!strcmp(argv[2],"--zoom")){
      //take a picture then zoom and move on it
      docname = argv[1];
     
      parseDoc (docname,3);
      return (0);         
    }
    if(!strcmp(argv[2],"--request")){
      // (node(51.249,7.148,51.251,7.152);<;);out;
      char api [100] ; 
      sprintf(api,"http://overpass.osm.rambler.ru/cgi/interpreter?data=%s",argv[1] );
      CURL *session = curl_easy_init(); 
      curl_easy_setopt(session, CURLOPT_URL, api);
      FILE * fp = fopen("./ApiOverpass/para.osm", "w"); 
      curl_easy_setopt(session,  CURLOPT_WRITEDATA, fp); 
      curl_easy_setopt(session,  CURLOPT_WRITEFUNCTION, fwrite);
      curl_easy_perform(session);
      fclose(fp);
      return 0 ;
    }
    //Use Overpass Api to download an osm then print it on the graphical interface
  }else if( argc == 4){
    if(!strcmp(argv[2],"--recording")){
      file=argv[3] ;
      docname = argv[1];
      parseDoc (docname,43);
      return (0);  
    }
    if(!strcmp(argv[2],"--play")){
      file=argv[3] ;
      docname = argv[1];
      parseDoc (docname,44);
      return (0);  
    } 
    if(!strcmp(argv[3],"--search")){
      
      docname = argv[1];
      seek=argv[2];
      parseDoc (docname,45);
      return (0);
    }
    int value=atoi(argv[3]);
    //./projet 2.36589 48.85280 0
    double parX =0 ;
    double parY =0 ;
    switch(value){
    case 0 :
      parX = 0.0205 ;
      parY =0.0088  ;
      break ;
    case 1 :
      parX = 0.0407  ;
      parY =0.0177 ;
      break ;
    case 2 :
      parX = 0.0816  ;
      parY = 0.0354;
      break ;
    case 3 :
      parX =  0.1633  ;
      parY = 0.0709;
       
      break ;
    default : printf("No value for this..");
    }
    
    char api [100] ; 
    sprintf(api,"http://overpass-api.de/api/map?bbox=%lf,%lf,%lf,%lf",(atof(argv[1])-parX),(atof(argv[2])-parY),(atof(argv[1])+parX),(atof(argv[2])+parY));
  
    CURL *session = curl_easy_init(); 
    curl_easy_setopt(session, CURLOPT_URL, api);
    FILE * fp = fopen("./ApiOverpass/map3.osm", "w"); 
    curl_easy_setopt(session,  CURLOPT_WRITEDATA, fp); 
    curl_easy_setopt(session,  CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_perform(session);
    fclose(fp);
    docname = "ApiOverpass/map.osm"; //Name of the OSM download thanks to APi
    parseDoc (docname,0);
    
    curl_easy_cleanup(session);
    return (0) ;
  }else if (argc == 5){
    char api [100] ;
    // example .projet 2.36589 48.85280 2.36924  48.85410
    //keep in mind that the overpass api is very slow... 
    sprintf(api,"http://overpass-api.de/api/map?bbox=%s,%s,%s,%s",argv[1],argv[2],argv[3],argv[4]);
   
    CURL *session = curl_easy_init(); 
    curl_easy_setopt(session, CURLOPT_URL, api);
    FILE * fp = fopen("./ApiOverpass/map.osm", "w"); 
    curl_easy_setopt(session,  CURLOPT_WRITEDATA, fp); 
    curl_easy_setopt(session,  CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_perform(session);
    fclose(fp);
    docname = "ApiOverpass/map.osm"; //Name of the OSM download thanks to APi
    parseDoc (docname,0);
    
    curl_easy_cleanup(session);
    return (0) ;
  }
 
  docname = argv[1];
  parseDoc (docname,0);
  
  return (1);
  
}
