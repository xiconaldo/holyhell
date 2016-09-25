CXX = g++
CXXFLAGS = -std=c++11
CXXFLAGS_DEBUG = $(CXXFLAGS) -g3
LDLIBS = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lpthread -ldl -lktx.gl
SOURCES = src/main.cpp src/loader.cpp src/shader.cpp src/camera.cpp src/object.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bin/sword.run
INC = -Isrc -Iinclude

sword: $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) $(LDLIBS) $(INC) -o $(EXECUTABLE)
	$(EXECUTABLE)

%.o: %.cpp
	$(CXX) $^ $(CXXFLAGS) $(INC) -c -o $@

debug:
	$(CXX) $(SOURCES) $(CXXFLAGS_DEBUG) $(LIBDIR) $(LDLIBS) $(INC) -o $(EXECUTABLE)
	gdb $(EXECUTABLE)

clear:
	rm -f bin/* src/*.o
