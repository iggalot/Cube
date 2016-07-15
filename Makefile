WX_LIBS = `wx-config --cxxflags --libs --gl-libs`
OGL_LIBS = -lGLEW -lglfw3 -lglut -lGL -lGLU -lrt -lm -lX11 -lXrandr -lXi -lXxf86vm -lXcursor -lXinerama -lpthread -ldl -lz
BIN_DIR = bin
SRC_DIR = src
INCL_DIR = include
BUILD_DIR = build
EXECUTABLE = myapp


#makes the executable
all: cube.o
	g++ $(BIN_DIR)/cube.o -o $(EXECUTABLE) $(WX_LIBS) $(OGL_LIBS) 

#makes the object file
cube.o: cube.cpp
	g++ $(WX_LIBS) $(OGL_LIBS) -c cube.cpp
	mv cube.o $(BIN_DIR)

#deletes the executable and object files
clean:
	rm -rf *.o
	rm -rf $(EXECUTABLE)
	rm -rf bin/*

init:
	mkdir $(BIN_DIR)
	mkdir $(SRC_DIR)
	mkdir $(INCL_DIR)
	mkdir $(BUILD_DIR)

