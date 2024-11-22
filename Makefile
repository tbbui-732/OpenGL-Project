CC=clang++
CFLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit
PROJ=main

$(PROJ): $(PROJ).cpp
	$(CC) $(PROJ).cpp -o $(PROJ) $(CFLAGS)

clean:
	rm $(PROJ)
