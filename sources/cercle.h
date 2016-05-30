#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <SDL_gfxPrimitives.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_rotozoom.h>
#include <curl/curl.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <glib.h>

//circle for localisation

typedef struct cercle{
  int px ;
  int py ;
  int rad ;
}cercle ;


