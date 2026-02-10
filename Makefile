CC = gcc
CFLAGS = -Wall -Wextra
LIBS = `pkg-config --cflags --libs sdl2` -lm
TARGET = random-walk
SRC = random-walk.c
ARGS = 

all: $(TARGET)
	./$(TARGET) $(ARGS)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
