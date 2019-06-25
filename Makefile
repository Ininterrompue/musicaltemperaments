COMPILER = g++
CPPFLAGS = -std=c++11 -Wall -O3
PROGRAM  = main
OBJECTS  = main.o tuning.o

$(PROGRAM): $(OBJECTS)
	$(COMPILER) $(CPPFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(COMPILER) $(CPPFLAGS) -c -o $@ $<

clean:
	rm $(PROGRAM) *.o *.dat