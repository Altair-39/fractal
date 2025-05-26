fractal.out: src/main.c
	$(CC) -o fractal.out src/main.c $(shell sdl2-config --cflags --libs) -fopenmp

clean:
	rm -f fractal.out

.PHONY: clean