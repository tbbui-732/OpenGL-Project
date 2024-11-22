# Compiler and flags
CC 			= clang++
CXXFLAGS 	= -std=c++17 -Wall -Iinclude
LDFLAGS 	= -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

# Source and header files
SRC 		= src/main.cpp src/glad.c
OBJ 		= src/main.o src/glad.o
HEADERS 	= $(wildcard include/*.h)
TARGET 		= OpenGLApp



# Build the target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each source file into an object file
src/%.o: src/%.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.c $(HEADERS)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
