WX_LIBS = `wx-config --cxxflags --libs --gl-libs`
OGL_LIBS = -lGLEW -lglfw3 -lglut -lGL -lGLU -lrt -lm -lX11 -lXrandr -lXi -lXxf86vm -lXcursor -lXinerama -lpthread -ldl -lz
BIN_DIR = bin
SRC_DIR = src
INCL_DIR = include
BUILD_DIR = build
EXECUTABLE = myapp


#makes the executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(BIN_DIR)/cube.o $(BIN_DIR)/DrawMenuDialog.o
	g++ $(BIN_DIR)/cube.o $(BIN_DIR)/DrawMenuDialog.o -o $(EXECUTABLE) $(WX_LIBS) $(OGL_LIBS) 

#makes the object file
$(BIN_DIR)/cube.o: $(SRC_DIR)/cube.cpp
	g++ $(WX_LIBS) $(OGL_LIBS) -c $(SRC_DIR)/cube.cpp
	mv cube.o $(BIN_DIR)

$(BIN_DIR)/DrawMenuDialog.o: $(SRC_DIR)/DrawMenuDialog.cpp
	g++ $(WX_LIBS) $(OGL_LIBS) -c $(SRC_DIR)/DrawMenuDialog.cpp
	mv DrawMenuDialog.o $(BIN_DIR)

#Needed for items that aren't actually made (i.e. no "all" file is made).  Without this
#Make will never find an "all" file and thus have to rebuild everything.
.PHONY: all clean init

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


