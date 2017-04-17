CC = g++
CFLAGS = -pedantic -Wall -Werror -Wextra -std=c++14

TARGET = automata
SOURCEFOLDER = src
SOURCES = $(wildcard $(SOURCEFOLDER)/*.cpp)
HEADERS = $(SOURCEFOLDER)
LIBS += $(shell pkg-config --libs ncurses)

.PHONY = clean

all: $(TARGET)

$(TARGET): $(SOURCEFOLDER)/$(TARGET).cpp
	$(CC) $(CFLAGS) -I$(HEADERS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	$(RM) $(TARGET)
