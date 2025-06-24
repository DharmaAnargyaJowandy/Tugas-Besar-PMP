CC = gcc
CFLAGS = -Wall -g -lm
TARGET = program_jadwal.exe
SOURCES = Main.c Doctor.c Schedule.c anomali_preferensi.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

Main.o: Doctor.h Schedule.h anomali_preferensi.h common.h
Doctor.o: Doctor.h common.h
Schedule.o: Schedule.h Doctor.h common.h
anomali_preferensi.o: anomali_preferensi.h Doctor.h Schedule.h

run: all
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean run