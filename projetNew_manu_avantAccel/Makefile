SRC=Main.c
BIN=projet

$(BIN): $(SRC)
	gcc -g -std=c99 Main.c `sdl-config --cflags --libs` -lm -o Main

clean:
	rm -f $(BIN)
