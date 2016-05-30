CC=gcc
CFLAGSSDL= -lSDLmain -lSDL_ttf -lSDL_gfx -lSDL_mixer -lm -lcurl
CONFIGXML= `xml2-config --cflags --libs`
CONFIGSDL= `sdl-config --cflags --libs`
CONFIGLIB= `pkg-config --cflags --libs glib-2.0`
WALL=-Wall 
EXEC=miniosm
DESSIN=dessin.svg

all : $(EXEC)

miniosm : DrawSdl1.c parserX.c Xgraphics.c colour.c colour.h parserX.h  Xgraphics.h cercle.h *.o 
	$(CC) -o $@ $^ $(CONFIGXML) $(CONFIGSDL) $(CONFIGLIB) $(CFLAGSSDL) $(WALL)

clean:
	rm -rf $(EXEC) $(DESSIN)

meteo: meteo.c
	$(CC) -o $@ $^ $(CONFIGXML) $(CONFIGSDL) $(CONFIGLIB) $(CFLAGSSDL) $(WALL)
        

mrproper: clean
	rm -rf out.bmp meteo

svg: 
	rm dessin.svg
