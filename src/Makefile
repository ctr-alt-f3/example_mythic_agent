CC = x86_64-w64-mingw32-gcc.exe
CFLAGS = -Wall -pedantic-errors -Wextra -Wshadow -Wformat=2
LIBS = -lwininet -lm
TARGET = implant
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET) $(LIBS)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@