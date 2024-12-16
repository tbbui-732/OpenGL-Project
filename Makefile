# Compiler and flags
CC 			= clang++
CXXFLAGS 	= -std=c++17 -Wall -Iinclude
LDFLAGS 	= -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
DEBUGFLAGS  = -g -O0
RELEASEFLAGS = -O2

# Source and header files
# SRCPATH and SRCNAME need to be set!
SRCPATH     = 1.getting_started/10.camera
SRCNAME     = main
SRC 		= src/$(SRCPATH)/$(SRCNAME).cpp src/glad.c src/stb_image.c
OBJ 		= src/$(SRCPATH)/$(SRCNAME).o src/glad.o src/stb_image.o
HEADERS 	= $(wildcard include/*.h)
TARGET 		= OpenGLApp

# Build the target
.PHONY: all
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) $(DEBUGFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each source file into an object file
%.o: %.cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(DEBUGFLAGS) -c $< -o $@

# Compile glad's and stb_image's source file into object file
%.o: %.c $(HEADERS)
	$(CC) $(CXXFLAGS) $(DEBUGFLAGS) -c $< -o $@

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
