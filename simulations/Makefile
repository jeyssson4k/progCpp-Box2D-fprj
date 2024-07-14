# Variables
COMPILER = g++
INCLUDE = -I "C:/Program Files (x86)/box2d/include"
LIBS = -L "C:/Program Files (x86)/box2d/build/bin"
FILENAME = main.cpp
COMPLEMENTS = lib.cpp
CXXLIBS = -lbox2d
OUTPUT = out.x
ARGUMENTS = w.txt b.txt
   
# Default target
all: $(OUTPUT)

# Compile target
$(OUTPUT): $(FILENAME)
	$(COMPILER) $(INCLUDE) $(LIBS) $(FILENAME) $(COMPLEMENTS) $(CXXLIBS) -o $(OUTPUT)

# Run target
run: $(OUTPUT)
	./$(OUTPUT) $(ARGUMENTS)

# Clean target
clean:
	rm -f $(OUTPUT)

.PHONY: all clean run
