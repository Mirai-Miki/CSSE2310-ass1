CFLAGS = -Wall -pedantic -std=c99 -g
OBJ = main.o error.o tilefile.o game.o humanPlayer.o autoPlayer.o saveGame.o \
		parseFile.o

fitz: ${OBJ}
	gcc ${OBJ} ${CFLAGS} -o fitz

fitz.o: main.c main.h
	gcc ${CFLAGS} -c main.c

error.o: error.c error.h
	gcc ${CFLAGS} -c error.c

tilefile.o: tilefile.c tilefile.h
	gcc ${CFLAGS} -c tilefile.c

game.o: game.c game.h
	gcc ${CFLAGS} -c game.c

humanPlayer.o: humanPlayer.c humanPlayer.h
	gcc ${CFLAGS} -c humanPlayer.c

autoPlayer.o: autoPlayer.c autoPlayer.h
	gcc ${CFLAGS} -c autoPlayer.c

saveGame.o: saveGame.c saveGame.h
	gcc ${CFLAGS} -c saveGame.c

parseFile.o: parseFile.c parseFile.h
	gcc ${CFLAGS} -c parseFile.c

clean:
	rm *.o fitz