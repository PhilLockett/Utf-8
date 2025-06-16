# Makefile for Logger unit tests.
objects  = test.o
objects += unittest.o
objects += utf_8.o

headers  = unittest.h
headers += utf_8.h

options = -std=c++20

test:	$(objects)	$(headers)
	g++ $(options) -o test $(objects)
	./test

%.o:	%.cpp	$(headers)
	g++ $(options) -c -o $@ $<

genutf8:	genutf8.cpp	utf_8.cpp
	g++ -std=c++20 -c -o genutf8.o genutf8.cpp
	g++ -std=c++20 -c -o utf_8.o utf_8.cpp
	g++ -std=c++20 -o genutf8 genutf8.o utf_8.o

format:
	tfc -s -u -r test.cpp
	tfc -s -u -r unittest.cpp
	tfc -s -u -r unittest.h
	tfc -s -u -r utf_8.cpp
	tfc -s -u -r utf_8.h

clean:
	rm -f *.exe *.o
	rm -f timings.txt
