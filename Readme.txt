gcc -o sdl2 sdl2.c `sdl2-config --cflags --libs` -lSDL2_image
gcc -o server serveurDame.c -lpthread