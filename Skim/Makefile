all: src/NanoTree.o  runMakeSkim 
runMakeSkim:   src/NanoTree.o  MakeSkim.C
	g++ -o runMakeSkim `root-config --libs` -I `root-config --incdir` -std=c++17 src/NanoTree.o  MakeSkim.C 

src/NanoTree.o: src/NanoTree.cpp src/NanoTree.h
	g++ -c -I `root-config --incdir` -std=c++17 src/NanoTree.cpp -o src/NanoTree.o

clean:
	rm src/*.o 
