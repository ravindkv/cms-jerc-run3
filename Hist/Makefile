
ROOT_I=-I`root-config --incdir`
ROOT_L=`root-config --libs`
CORR_A=`correction config --cflags --ldflags --rpath`
GCC=g++ -std=c++17

all:  src/SkimTree.o src/ObjectScale.o  src/ObjectPick.o src/EventPick.o runGamHistosFill

src/SkimTree.o: src/SkimTree.cpp src/SkimTree.h
	${GCC} -o src/SkimTree.o -c ${ROOT_I} -D STANDALONE src/SkimTree.cpp

src/ObjectScale.o: src/ObjectScale.cpp src/ObjectScale.h
	${GCC} -o src/ObjectScale.o -c ${CORR_A} ${ROOT_L} ${ROOT_I}  src/ObjectScale.cpp

src/ObjectPick.o: src/SkimTree.o src/ObjectPick.cpp src/ObjectPick.h
	${GCC} -o src/ObjectPick.o -c ${CORR_A} ${ROOT_L} ${ROOT_I} -D STANDALONE src/ObjectPick.cpp

src/EventPick.o: src/ObjectPick.o src/EventPick.cpp src/EventPick.h
	${GCC} -o src/EventPick.o -c ${CORR_A} ${ROOT_I}  -D STANDALONE src/EventPick.cpp 

#/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/py3-correctionlib/2.1.0-bc1f1a24613552be911056d8eb959866/lib/python3.9/site-packages/correctionlib/
runGamHistosFill: src/ObjectPick.o src/EventPick.o src/SkimTree.o GamHistosFill.C parsePileUpJSON.C GamHistosFill.h src/ObjectScale.o	
	${GCC} -o runGamHistosFill ${CORR_A} ${ROOT_L} ${ROOT_I} -D STANDALONE GamHistosFill.C src/SkimTree.o src/EventPick.o src/ObjectPick.o src/ObjectScale.o
clean:
	rm runGamHistosFill src/*.o 
