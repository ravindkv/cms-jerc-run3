#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include <iostream>
using namespace std;

class GlobalFlag{
    public: 
        GlobalFlag(const string& outName);
        ~GlobalFlag() = default;
        
        bool isDebug;

        //Year 
        bool is2022;
        bool is2023;
        bool is2024;


        //Data or MC
        bool isData;
        bool isMC;
        
        //Channels
        bool isZeeJet;
        bool isZmmJet;
        bool isGamJet;
        bool isMultiJet;
        bool isWqqm;
        bool isWqqe;

        //Sample (e.g. QCD)
        bool isQCD;
        bool isMG;
        void printFlag();
};
#endif
