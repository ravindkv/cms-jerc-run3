#ifndef EVENTPICK_H
#define EVENTPICK_H

#include<vector>
#include<string>
#include<set>
#include<iostream>
#include<fstream>
#include<TH1F.h>
#include<TH1D.h>

#include"SkimTree.h"
#include"GlobalFlag.h"

class EventPick: public GlobalFlag{
public:
	EventPick(TString oName): GlobalFlag(oName){};

    bool passFilter(SkimTree *tree);
	~EventPick();
	
};
#endif
