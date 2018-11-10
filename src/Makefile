all: client

# jeu_de_dame: main.o affichage.o deplacement.o
	# gcc -o jeu_de_dame main.o affichage.o deplacement.o `sdl2-config --cflags --libs`
client: sdl2.o affichage.o deplacement.o clientTCP.o login.o SQLite.o
	gcc -o client sdl2.o affichage.o deplacement.o clientTCP.o login.o SQLite.o  `sdl2-config --cflags --libs` -lSDL2_image -lsqlite3 -lpthread

sdl2.o: sdl2.c sdl2.h
	gcc -c sdl2.c `sdl2-config --cflags --libs` -lSDL2_image

clientTCP.o: clientTCP.c
	gcc -c clientTCP.c
# main.o: main.c main.h
# 	gcc -c main.c `sdl2-config --cflags --libs`
login.o:  login.c login.h
	gcc -c -lncurses login.c
SQLite.o: SQLite.c SQLite.h
	gcc -c SQLite.c

# communication.o: communication.c communication.h
# 	gcc -c communication.c

affichage.o: affichage.c affichage.h main.h
	gcc -c affichage.c

deplacement.o: deplacement.c deplacement.h main.h
	gcc -c deplacement.c
