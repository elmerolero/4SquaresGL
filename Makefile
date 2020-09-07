CC = g++
CFLAGS = `pkg-config --cflags sdl2 glu`
LIBS = `pkg-config --libs sdl2 glu` -lSDL2_image

4Squares: source/main.cpp source/temporizador.o
	$(CC) source/main.cpp $(CFLAGS) -o 4Squares source/temporizador.o $(LIBS)

temporizador.o: source/temporizador.h source/temporizador.cpp
	$(CC) source/temporizador.h source/temporizador.cpp $(CFLAGS) -c $(LIBS)
