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
#include"ObjectPick.h"
#include"GlobalFlag.h"

class EventPick: public GlobalFlag{
public:
	EventPick(TString oName): GlobalFlag(oName){
    title = oName;
    year = "2016";
    printEvent = -1;

    // Cut levels
    MET_cut = 20.0;
    Nlep_eq = 1;
    Njet_ge = 3;
    NBjet_ge = 1;
    Nmu_eq = 1;
    Nele_eq = 1;
    Npho_eq = 1;
    
    NlooseMuVeto_le = 0;
    NlooseEleVeto_le = 0;


    }

	~EventPick();
	
	void process_event(SkimTree* tree, ObjectPick* selector);
	std::string title;
	std::string year;

	int printEvent;

	// cuts as parameters, to modify easily
	double MET_cut;
	int Nlep_eq;
	int Njet_ge;
	int NBjet_ge;
	int Nmu_eq;
	int Nele_eq;
	int Npho_eq;

	int NlooseMuVeto_le;
	int NlooseEleVeto_le;
	
	// variables showing passing or failing selections
    bool passPreselMu;
    bool passPreselEle;

private:
	SkimTree* tree;
	ObjectPick* selector;
};
#endif
