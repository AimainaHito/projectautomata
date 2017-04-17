CC = g++
CFLAGS = -pedantic -Wall -Werror -Wextra -std=c++14

TARGET = automata
SOURCES = $(wildcard *.cpp)
HEADERS = .
LIBS += $(shell pkg-config --libs ncurses)

.PHONY = clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -I$(HEADERS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	$(RM) $(TARGET)
