CXX = g++
CXXFLAGS = -std=c++11
CXXFLAGS_DEBUG = $(CXXFLAGS) -g3
LDLIBS = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lpthread -ldl
SOURCES = src/triangle.cpp src/loader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bin/program
INC = -Isrc -Iinclude

program: $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) $(LDLIBS) $(INC) -o $(EXECUTABLE)
	$(EXECUTABLE)

%.o: %.cpp
	$(CXX) $^ $(CXXFLAGS) $(INC) -c -o $@

debug:
	$(CXX) $(SOURCES) $(CXXFLAGS_DEBUG) $(LDLIBS) $(INC) -o $(EXECUTABLE)
	gdb $(EXECUTABLE)

clear:
	rm -f bin/* src/*.o
