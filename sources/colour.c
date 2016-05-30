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

#include "colour.h"



/**
FOnction qui permet de changer les couleurs
du renderer via un fichier style.xml 
*/

colour* parseColour(char *docname){ 
  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlParseFile(docname);
  //Toutes les verifications basiques
  if (doc == NULL ) {
    fprintf(stderr,"Document not parsed successfully. \n");
    return NULL;
  }
  cur = xmlDocGetRootElement(doc); 
  if (cur == NULL) {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return NULL;
  }  
 
  colour *color ;
  color = malloc(40*sizeof(colour));
  int i =0 ;
  cur = cur->xmlChildrenNode;
  //follow an order
  while (cur != NULL) {

    if(cur->xmlChildrenNode == NULL  && (xmlStrcmp(cur->name, (const xmlChar *)"")) && (xmlStrcmp(cur->name, (const xmlChar *)"text"))){
      // printf("%s\n ",cur->name);
      color[i].r =atof((char *)xmlGetProp(cur,(const xmlChar *)"r"));
      color[i].g =atof((char *)xmlGetProp(cur,(const xmlChar *)"g"));
      color[i].b =atof((char *)xmlGetProp(cur,(const xmlChar *)"b"));
      color[i].a =atof((char *)xmlGetProp(cur,(const xmlChar *)"a"));
      i++ ;
    }
    if(cur->xmlChildrenNode != NULL  && (xmlStrcmp(cur->name, (const xmlChar *)"")) && (xmlStrcmp(cur->name, (const xmlChar *)"text"))){
      //printf("ont des enfants (fdp)  %s\n ",cur->name);
      
      xmlNodePtr cur2 = cur ;
      cur2=cur2->xmlChildrenNode;
      while(cur2 != NULL){
	if((xmlStrcmp(cur2->name, (const xmlChar *)"")) && (xmlStrcmp(cur2->name, (const xmlChar *)"text"))){
	  //  printf("sous enfants  %s\n ",cur2->name);
	  color[i].r =atof((char *)xmlGetProp(cur2,(const xmlChar *)"r"));
	  color[i].g =atof((char *)xmlGetProp(cur2,(const xmlChar *)"g"));
	  color[i].b =atof((char *)xmlGetProp(cur2,(const xmlChar *)"b"));
	  color[i].a =atof((char *)xmlGetProp(cur2,(const xmlChar *)"a"));
	  i++;
	}
	cur2=cur2->next;
      }
      color[i].r =atof((char *)xmlGetProp(cur,(const xmlChar *)"r"));
      color[i].g =atof((char *)xmlGetProp(cur,(const xmlChar *)"g"));
      color[i].b =atof((char *)xmlGetProp(cur,(const xmlChar *)"b"));
      color[i].a =atof((char *)xmlGetProp(cur,(const xmlChar *)"a"));
      i++;
    }
    
    cur=cur->next;   
  }
  
  /*   int l=0;
       for(l=0 ; l< 35 ; l++)
       printf("%d  %d   %d\n  ",color[l].r,color[l].g,color[l].b);*/
  return color;
}
/*
  int main(int argc, char **argv) {

  char *docname;
		
  if (argc <= 1) {
  printf("Usage: %s docname\n", argv[0]);
  return(0);
  }

  docname = argv[1];
  parseColour     (docname);

  return (1);
  }
*/

