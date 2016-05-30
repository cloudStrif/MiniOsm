/*
 * Copyright (C) <2016>  <chenP>
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
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <glib.h>

#include "parserX.h"
#include "DrawSdl1.h"
#include "colour.h"


/*
typedef struct meteo{
     int temperature [4];//kelvin
     int pression;//pascal
     int humidite ;//%
     int ventMoyen ;
}meteo;*/



void res(int n);
void res2(int n);
int dessinePas(int a ,int b);
double radians(double degrees);
double lat_to_y(double lat);
double lon_to_x(double lon);
void update(double minla ,double maxla ,double minlo ,double maxlo);
double* convert(double lat ,double lon ,double mapLonLeft,double mapLonRight, double mapLatBottom ,int mode,int zo, int zo2, int zo3, int zo4,int angular,int ang,int theta);
void rendu2(conteneur *chemins,int size ,double minla ,double maxla ,double minlo ,double maxlo,int mode ,int zo,int zo2,int zo3,int zo4,int angular,int ang,int theta,colour * styl,char *file);
void rendu3();
void rendu (ways *chemins,int size ,double minla ,double maxla ,double minlo ,double maxlo  );
