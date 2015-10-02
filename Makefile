CC = g++

SRC = src
BIN = bin

CFLAGS = -g -lao -ldl -lm -Iinclude -Iinclude/rapidxml

.DEFAULT_GOAL = all

test: test_gen
exec: scopeclock svg2scope
live: dopescope
all: scopeclock svg2scope test_gen dopescope

scopeclock: $(SRC)/clock/scopeclock.cpp include/*
	$(CC) $(SRC)/clock/scopeclock.cpp -o $(BIN)/scopeclock $(CFLAGS)

svg2scope: $(SRC)/svg2scope.cpp include/*
	$(CC) $(SRC)/svg2scope.cpp -o $(BIN)/svg2scope $(CFLAGS)

test_gen: $(SRC)/test_gen.cpp include/wfassem.h
	$(CC) $(SRC)/test_gen.cpp -o $(BIN)/test_gen $(CFLAGS)

dopescope: $(SRC)/dopescope.cpp include/*
			$(CC) $(SRC)/dopescope.cpp -o $(BIN)/dopescope -lrtmidi -lasound -lpthread $(CFLAGS)

prefix = /usr/local
datadir = $(prefix)/share

install: all
	install -m 0755 $(BIN)/scopeclock $(prefix)/bin
	install -m 0755 $(BIN)/svg2scope $(prefix)/bin
	mkdir -p $(datadir)/avscope/clock && cp -r ./resources/clock $(datadir)/avscope
	sudo cp ./scripts/scopeclock /etc/init.d && chmod +x /etc/init.d/scopeclock && update-rc.d scopeclock defaults
