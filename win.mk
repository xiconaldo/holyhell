CXX = g++
CXXFLAGS = -std=c++11
CXXFLAGS_DEBUG = $(CXXFLAGS) -g3
LDLIBS = -lglfw3 -lopengl32 -lgdi32 -lglew32 -lglu32 -lktx.gl
SOURCES = src\main.cpp src\loader.cpp src\shader.cpp src\camera.cpp src\object.cpp src\terrain.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bin\sword
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
	del bin\* src\*.o