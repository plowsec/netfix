# Declaration of variables

CXXFLAGS = -Iinclude -Iwolfssl -w -s -m32
LDFLAGS = -L/usr/local/lib -Xlinker -Bstatic -lwolfssl -lm -Xlinker -Bdynamic

# File names
EXEC = netfix
#SOURCES = $(wildcard **/*.cpp)
SOURCES = $(shell find . -name "*.cpp")
OBJECTS = $(SOURCES:.cpp=.o)
#OBJECTS = $(lastword $(subst /, , $(SOURCES:.cpp=o)))
OBJDIR = obj

all: createdir $(EXEC) tidy

createdir:
	mkdir -p obj && mkdir -p bin

tidy: createdir $(EXEC)
	find . -name "*.o" -not -path "./obj/*" -exec mv -v {} obj \;

#print variables
print-%: ; @echo $*=$($*)

# Main target
$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -m32 -o $(EXEC)

# To obtain object files
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC)
	rm -f $(OBJECTS)
	rm -rf obj

#non-file targets
.phony: clean tidy createdir all
