CXXFLAGS = -std=c++14 -Wall

all : test-maze genmaze

test-maze : maze.o test-maze.o testbase.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

genmaze : genmaze.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean :
	rm -f genmaze test-maze *.o *~

.PHONY : all clean

