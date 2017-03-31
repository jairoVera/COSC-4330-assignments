CXX = g++
CXXFLAGS = -g -Wall -O0

SOURCES = laptop.cpp processTable.cpp process.cpp deviceTable.cpp device.cpp dataTable.cpp data.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: assignment1

assignment1: main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) main.o $(OBJECTS) -o assignment1
	
main.o: main.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) -c main.cpp $(SOURCES)

laptop.o: $(SOURCES)
	$(CXX) $(CXXFLAGS) -c $(SOURCES)

processTable.o: processTable.cpp process.cpp
	$(CXX) $(CXXFLAGS) -c processTable.cpp process.cpp

deviceTable.o: deviceTable.cpp device.cpp
	$(CXX) $(CXXFLAGS) -c deviceTable.cpp device.cpp

dataTable.o: dataTable.cpp data.cpp
	$(CXX) $(CXXFLAGS) -c dataTable.cpp data.cpp

process.o: process.cpp
	$(CXX) $(CXXFLAGS) -c process.cpp

data.o: data.cpp
	$(CXX) $(CXXFLAGS) -c data.cpp

device.o: device.cpp
	$(CXX) $(CXXFLAGS) -c device.cpp

clean:
	rm *o hello