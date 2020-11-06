CC = g++
CFLAGS = -g -std=c++14 -Wall

all: makeapp main clean


makeapp: main.cpp PGMImage.cpp  
	$(CC) main.cpp PGMImage.cpp -o main.app $(CFLAGS)

main:
	./main.app Figures/fig3.pgm Figures/fig3_output.pgm

clean:
	rm -rf *.app
