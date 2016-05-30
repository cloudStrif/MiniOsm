#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

/*
 *data for color following an order
 *filled -> draw
 
 * 
 */

typedef struct colour{
int  r;
int  g;
int  b;
int  a ;
int  r2;
int  g2;
int  b2;
int  a2 ;
}colour ;


colour* parseColour(char *docname) ;
