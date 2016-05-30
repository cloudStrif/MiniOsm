#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <SDL_gfxPrimitives.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_rotozoom.h>
#include <curl/curl.h>

//utilitaire pour la meteo Api
typedef struct meteo{
     int temperature ;//kelvin
     int pression;//pascal
     int humidite ;//%
     int ventMoyen ;
}meteo;

void parseMeteo(char* docname); 
