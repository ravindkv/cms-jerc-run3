#ifndef GlobalFlag_H
#define GlobalFlag_H
        
#include<iostream>
#include <fstream>
#include <TString.h>

class GlobalFlag{
    public: 
        GlobalFlag(TString oName);
        ~GlobalFlag();
        
        //Year 
        bool is22;
        bool is23;
        bool is24;

        //Eras 2022
        bool is22B;
        bool is22C;
        bool is22D;
        bool is22E;
        bool is22F;
        bool is22G;
        //Eras 2023
        bool is23B;
        bool is23C;
        bool is23D;
        bool is23E;
        bool is23F;
        bool is23G;
        //Eras 2024
        bool is24B;
        bool is24C;
        bool is24D;
        bool is24E;
        bool is24F;
        bool is24G;

        //Data or MC
        bool isData;
        bool isMC;
        
        //Channels
        bool isGamJet;
        bool isDiEleJet;
        bool isDiMuJet;
        bool isDiJet;

        //Sample (e.g. QCD)
        bool isQCD;
        bool isMG;
        void printFlag();
};
#endif
