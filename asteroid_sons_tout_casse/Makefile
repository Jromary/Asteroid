SRC=Main.c
BIN=projet

$(BIN): $(SRC)
	gcc -Wall -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -lSDL_mixer -o Main

clean:
	rm -f $(BIN)
