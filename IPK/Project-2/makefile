.PHONY: clean

CXX = g++
CXXFLAGS = -std=c++0x -lpcap

OBJECTS = TCP_Scanner.o ipk-scan.o

ipk-scan: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJECTS): TCP_Scanner.h TCP_Scanner.cpp ipk-scan.h ipk-scan.cpp

clean:
	rm -f ipk-scan *.o