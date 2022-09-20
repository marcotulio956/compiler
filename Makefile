CXX=g++
CXXFLAGS=-g -ggdb3 -O2 -Wall

TARGET=comp
OBJS=comp.o \
	lexical/LexicalAnalysis.o \
	lexical/SymbolTable.o

all: $(TARGET)

clean:
	rm -frd $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)	

make new: clean all

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<