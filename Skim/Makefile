all: src/NanoTree.o  makeSkim 
makeSkim:   src/NanoTree.o  makeSkim.C
	g++ -o makeSkim `root-config --libs` -I `root-config --incdir` -std=c++17 src/NanoTree.o  makeSkim.C 

src/NanoTree.o: src/NanoTree.cpp src/NanoTree.h
	g++ -c -I `root-config --incdir` -std=c++17 src/NanoTree.cpp -o src/NanoTree.o

clean:
	rm src/*.o 
