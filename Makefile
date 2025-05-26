fractal.out:
	$(CC) -o fractal.out src/main.c $(shell sdl2-config --cflags --libs)
