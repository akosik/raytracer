CC = g++

INCLUDE = -lfreeimage

SOURCES = src/main.cpp src/raytrace.cpp src/transform.cpp src/parser.cpp src/variables.cpp

CFLAGS = -O3 -std=c++11

all: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o trace

clean:
	rm trace
