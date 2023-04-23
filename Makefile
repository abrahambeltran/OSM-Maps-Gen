# Compiler to use.
CC := g++
# Compilation flags.
CFLAGS := -c -g -Wall -Werror -Wpedantic --std=c++11
# Linker flags.
LFLAGS := -g
# Source code directory.
SRC := ./src
# Object code directory.
OBJ := ./obj
# Test code directory
TEST := ./tests
#main
main: $(OBJ)/main.o $(OBJ)/point2d.o $(OBJ)/osmnode.o $(OBJ)/osm.o $(OBJ)/image.o
	$(CC) $(LFLAGS) $(OBJ)/main.o $(OBJ)/point2d.o $(OBJ)/osmnode.o $(OBJ)/osm.o $(OBJ)/image.o -o main
#OBJ code for main
$(OBJ)/main.o: $(SRC)/main.cpp $(SRC)/osm.hpp $(SRC)/image.hpp
	$(CC) $(CFLAGS) $(SRC)/main.cpp -o $(OBJ)/main.o
#OBJ code for Osm
$(OBJ)/osm.o: $(SRC)/osm.cpp $(SRC)/osm.hpp $(SRC)/osmnode.hpp
	$(CC) $(CFLAGS) $(SRC)/osm.cpp -o $(OBJ)/osm.o
#OBJ code for OsmNode
$(OBJ)/osmnode.o: $(SRC)/osmnode.cpp $(SRC)/osmnode.hpp $(SRC)/point2d.hpp
	$(CC) $(CFLAGS) $(SRC)/osmnode.cpp -o $(OBJ)/osmnode.o
#OBJ code for Point2D
$(OBJ)/point2d.o: $(SRC)/point2d.cpp $(SRC)/point2d.hpp
	$(CC) $(CFLAGS) $(SRC)/point2d.cpp -o $(OBJ)/point2d.o
#OBJ code for Image
$(OBJ)/image.o: $(SRC)/image.cpp $(SRC)/image.hpp $(SRC)/osm.hpp
	$(CC) $(CFLAGS) $(SRC)/image.cpp -o $(OBJ)/image.o

.PHONY: clean doc

# Remove object files and executable to ensure next make is entire.
clean:
	rm -rf $(OBJ)/*.o main

# Generate HTML documentation.
doc:
	doxygen Doxyfile