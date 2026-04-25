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


# CC = x86_64-w64-mingw32-gcc.exe

# # Dodałem -MMD -MP, które generują pliki .d (śledzenie nagłówków .h)
# CFLAGS = -Wall -pedantic-errors -Wextra -Wshadow -Wformat=2 -MMD -MP
# LIBS = -lwininet -lm

# # Skoro używasz MinGW-w64, program docelowy na Windowsa powinien mieć .exe
# TARGET = implant.exe 

# # Znajduje WSZYSTKIE pliki .c w folderze (main.c, base64.c, itp.)
# SRC = $(wildcard *.c)
# OBJ = $(SRC:.c=.o)
# # Lista plików z zależnościami nagłówkowymi
# DEP = $(OBJ:.o=.d)

# # Główna reguła budująca
# $(TARGET): $(OBJ)
# 	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

# # Reguła jak zrobić .o z .c
# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Dołączenie plików zależności (żeby Make widział zmiany w plikach .h)
# -include $(DEP)

# # Reguła czyszcząca - użyj komendy: make clean
# clean:
# 	rm -f *.o *.d $(TARGET)