CC = cc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
SRC = syslinux-customizer.c
OBJ = $(SRC:.c=.o)
EXE = syslinux-customizer

all: $(EXE)
syslinux-customizer: syslinux-customizer.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
debug:
	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)
test:
	./syslinux-customizer
clean:
	rm -f $(OBJ) $(EXE) debug
