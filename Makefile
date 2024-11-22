# Compiler and flags
CC 			= clang++
CXXFLAGS 	= -std=c++17 -Wall -Iinclude
LDFLAGS 	= -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
DEBUGFLAGS  = -g -O0
RELEASEFLAGS = -O2

# Source and header files
# SRCPATH and SRCNAME need to be set!
SRCPATH     = 1.getting_started/1.hello_triangle
SRCNAME     = hello_triangle
SRC 		= src/$(SRCPATH)/$(SRCNAME).cpp src/glad.c
OBJ 		= src/$(SRCPATH)/$(SRCNAME).o 	src/glad.o
HEADERS 	= $(wildcard include/*.h)
TARGET 		= OpenGLApp

# Build the target
.PHONY: all
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each source file into an object file
%.o: %.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Compile glad's source file into object file
%.o: %.c $(HEADERS)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
