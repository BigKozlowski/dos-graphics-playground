CC=/Users/biko/repos/soft/djgpp/bin//i586-pc-msdosdjgpp-gcc
CFLAGS=-O2 -g
CLIBS=-lm
TARGET=main.exe

all: $(TARGET)

$(TARGET): main.c fix16.c mat4.c quat.c vec2.c vec3.c vec4.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean:
	rm -f $(TARGET)
