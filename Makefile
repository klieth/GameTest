CC=g++
CFLAGS=-c -Wall
#LDFLAGS=-lGL -lassimp -lglfw -lGLEW
LDFLAGS=-lGL -lglfw -lGLEW
SOURCES=Camera.cpp GameState.cpp Shader.cpp game.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
