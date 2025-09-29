CC=/Users/biko/repos/soft/djgpp/bin//i586-pc-msdosdjgpp-gcc
CFLAGS=-O2 -g
TARGET=main.exe

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
