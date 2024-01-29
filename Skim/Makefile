all: src/SkimTree.o  makeSkim 
makeSkim:   src/SkimTree.o  makeSkim.C
	g++ -o makeSkim `root-config --libs` -I `root-config --incdir` -std=c++17 src/SkimTree.o  makeSkim.C 

src/SkimTree.o: src/SkimTree.cpp src/SkimTree.h
	g++ -c -I `root-config --incdir` -std=c++17 src/SkimTree.cpp -o src/SkimTree.o

clean:
	rm src/*.o 
