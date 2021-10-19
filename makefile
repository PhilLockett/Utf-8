# Makefile for Logger unit tests.
objects  = test.o
objects += unittest.o
objects += utf_8.o

headers  = unittest.h
headers += utf_8.h

options = -std=c++20

test:	$(objects)	$(headers)
	g++ $(options) -o test $(objects)

%.o:	%.cpp	$(headers)
	g++ $(options) -c -o $@ $<

format:
	tfc -s -u -r test.cpp
	tfc -s -u -r unittest.cpp
	tfc -s -u -r unittest.h
	tfc -s -u -r utf_8.cpp
	tfc -s -u -r utf_8.h

clean:
	rm -f *.exe *.o
	rm -f timings.txt
