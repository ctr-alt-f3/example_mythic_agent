CC = x86_64-w64-mingw32-gcc.exe
CFLAGS = -Wall
LIBS = -lwininet
TARGET = implant
SRC = main.c
OBJ = main.o

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET) $(LIBS)
$(OBJ): $(SRC)
	$(CC) $(FLAGS) -c $(SRC) $(LIBS) -o $(OBJ)