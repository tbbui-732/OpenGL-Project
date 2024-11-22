CC 			= clang++
CXXFLAGS 	= -std=c++17 -Wall -Iinclude
LDFLAGS 	= -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
SRC 		= src/main.cpp src/glad.c
OBJ 		= src/main.o src/glad.o
TARGET 		= OpenGLApp

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
