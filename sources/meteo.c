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
/*
Test file used Meteo free Api
//http://www.infoclimat.fr/api-previsions-meteo.html?id=2988507&cntry=FR
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
#include "meteo.h"
  
/**
Fonction permettant le parsing d'un xml
genere par une Api meteo Gratuite  
*/
void parseMeteo(char* docname){
  xmlDocPtr doc;
  xmlNodePtr cur;
  
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
  meteo * met = malloc(65*sizeof(meteo));
  int inc = 0; 
  xmlChar *key;
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"echeance")) /*&& !xmlStrcmp(xmlGetProp(cur,"hour"),"156")*/  ){
      xmlNodePtr cur2 = cur ;
      cur2=cur2->xmlChildrenNode;
      while(cur2 !=NULL){
        if ((!xmlStrcmp(cur2->name, (const xmlChar *)"temperature"))){
	  xmlNodePtr cur3 = cur2 ;
	  cur3=cur3->xmlChildrenNode;
	  while(cur3 !=NULL){
	    if ((!xmlStrcmp(cur3->name, (const xmlChar *)"level") && !xmlStrcmp((xmlChar *)xmlGetProp(cur3, (const xmlChar *)"val"), (const xmlChar *)"2m"))  ){
	      key = xmlNodeListGetString(doc, cur3->xmlChildrenNode, 1);
	      met[inc].temperature = atoi((char *)key);
	      //   printf("%s\n",key);
	    }
	   
	    cur3=cur3->next ;
	  }
	  //   printf("\n");
	}
	
	if ((!xmlStrcmp(cur2->name, (const xmlChar *)"pression"))){
	  xmlNodePtr cur3 = cur2 ;
	  cur3=cur3->xmlChildrenNode;
	  while(cur3 !=NULL){
	    if ((!xmlStrcmp(cur3->name, (const xmlChar *)"level"))){
	      key = xmlNodeListGetString(doc, cur3->xmlChildrenNode, 1);
	      met[inc].pression = atoi((char *)key);
	      //  printf("%s\n",key);
	    }
	    cur3=cur3->next; 
	  }
	}
	if ((!xmlStrcmp(cur2->name, (const xmlChar *)"humidite"))){
	  xmlNodePtr cur3 = cur2 ;
	  cur3=cur3->xmlChildrenNode;
	  while(cur3 !=NULL){
	    if ((!xmlStrcmp(cur3->name, (const xmlChar *)"level"))){
	      key = xmlNodeListGetString(doc, cur3->xmlChildrenNode, 1);
	      met[inc].humidite = atoi((char *)key);
	      // printf("%s\n",key);
	    }
	    cur3=cur3->next; 
	  }
	  printf("\n");
	}
	if ((!xmlStrcmp(cur2->name, (const xmlChar *)"vent_moyen"))){
	  xmlNodePtr cur3 = cur2 ;
	  cur3=cur3->xmlChildrenNode;
	  while(cur3 !=NULL){
	    if ((!xmlStrcmp(cur3->name, (const xmlChar *)"level"))){
	      key = xmlNodeListGetString(doc, cur3->xmlChildrenNode, 1);
	      met[inc].ventMoyen = atoi((char *)key);
	      inc++;
	      //  printf("%s\n",key);
	    }
	    cur3=cur3->next; 
	  }
	  printf("\n");
	}
	cur2=cur2->next;
	
      }
    }
 
    
    cur=cur->next;
  }
  int k = 0 ;
  int hour =0 ;
  printf("===================Estimation Meteo de Paris=======================\n");
  for (;k<62 ; k++){
    printf("hour = %d  temperature(degres): %d   pression(pa) :%d   humidité :%d  Vent Moyen : %d KM/H\n",hour,(met[k].temperature-273) ,met[k].pression , met[k].humidite ,met[k].ventMoyen);
    hour=hour+3;
  }
} 

int main(int argc, char **argv){
 
  if (argc <= 1) {
    printf("Usage: %s docname\n", argv[0]);
    return(0);
  }
  if(argc == 2){
    parseMeteo("meteoApi/meteo.xml");
    return(0);
  }
  if(argc == 3){
  char api [1000];
    sprintf(api,"http://www.infoclimat.fr/public-api/gfs/xml?_ll=%s,%s&_auth=ARsEEwV7VnRSf1ViBHJVfFc%%2FATQKfAkuC3dXNFw5VitTOFIzVTVUMlM9WyZVegQyUn9VNgw3UGBWPQJ6AHJSMwFrBGgFblYxUj1VMAQrVX5XeQFgCioJLgtgVzZcL1Y3UzRSMVUoVDRTPVsxVXsENlJ%2BVSoMMlBsVjACbABoUjUBYQRkBWFWMVIiVSgEMVVnVzIBNAphCWULOlczXGVWZFNmUjdVNlQzUyJbO1VkBDVSaVU2DDtQalY3AnoAclJIAREEfQUmVnZSaFVxBClVNFc6ATU%3D&_c=451b37b71ac7f799e7892f74887a6f90",argv[1],argv[2]);
    printf("%s",api);
    CURL *session = curl_easy_init(); 
    curl_easy_setopt(session, CURLOPT_URL, api);
    FILE * fp = fopen("./meteoApi/meteo.xml", "w"); 
    curl_easy_setopt(session,  CURLOPT_WRITEDATA, fp);  
    curl_easy_setopt(session,  CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_perform(session);
    fclose(fp);
    parseMeteo("meteoApi/meteo.xml");
    return(0);
  }
  return (1);
}
