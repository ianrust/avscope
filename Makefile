CC = g++

CFLAGS = -lao -ldl -lm

WFINCLUDE = wfassem.h wftypes.h
SVGINCLUDE = svgparser.h

all: svg2scope test_gen

svg2scope: svg2scope.cpp $(WFINCLUDE) $(SVGINCLUDE)
	$(CC) svg2scope.cpp -o svg2scope $(CFLAGS)	

test_gen: test_gen.cpp $(WFINCLUDE)
	$(CC) test_gen.cpp -o test_gen $(CFLAGS)