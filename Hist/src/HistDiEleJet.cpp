#include "HistDiEleJet.h"
   
int HistDiEleJet::Run(TString oName, SkimTree *tree, ObjectScale *objS, TFile *fout){
    TStopwatch fulltime, laptime;
     fulltime.Start();
     TDatime bgn;
     int nlap(0);
   
     int _ntot(0), _nevents(0), _nbadevents_json(0), _nbadevents_trigger(0);
     int _nbadevents_veto(0);
     
     bool isPrint = false;
   
     string dataset = "2022C"; 
     string& ds = dataset;
     // Select appropriate L1RC for type-I MET L1L2L3-RC calculation
     string sera("");
     cout<<"AA2"<<endl;
     if (ds=="2016APVP8" || ds=="2016APVQCD") sera = "2016APV";
     if (ds=="2016P8" || ds=="2016QCD") sera = "2016FGH";
     if (ds=="2017P8" || ds=="2017QCD") sera = "2017";
     if (ds=="2018P8" || ds=="2018QCD") sera = "2018";
     if (ds=="2022P8" || ds=="2022QCD") sera = "2022";
     if (ds=="2022EEP8" || ds=="2022EEQCD") sera = "2022EE";
     if (ds=="2023P8" || ds=="2023QCD") sera = "2023";
     //
     if (ds=="2016B"||ds=="2016C"||ds=="2016D"||ds=="2016BCD"||
         ds=="2016E"||ds=="2016F"||ds=="2016EF"||ds=="2016BCDEF") sera = "2016APV";
     if (ds=="2016FG"||ds=="2016H"||ds=="2016FGH") sera = "2016FGH";
     if (ds=="2017B"||ds=="2017C"||ds=="2017D"||ds=="2017E"||ds=="2017F"||
         ds=="2017BCDEF") sera = "2017";
     if (ds=="2018A"||ds=="2018A1"||ds=="2018A2"||ds=="2018B"||ds=="2018C"||
         ds=="2018D"||ds=="2018D1"||ds=="2018D2"||ds=="2018D3"||ds=="2018D4")
       sera = "2018";
     //
     if (ds=="2022P8" || ds=="2022QCD") sera = "2022";
     if (ds=="2022EEP8" || ds=="2022EEQCD") sera = "2022EE";
     if (ds=="2022C" || ds=="2022D") sera ="2022";
     if (ds=="2022E" || ds=="2022F" || ds=="2022G") sera = "2022EE";
     if (ds=="2023B" || ds=="2023Cv123" || ds=="2023Cv4" || ds=="2023D")
       sera = "2023";
     assert(sera!="");
   
     
     cout<<"DD"<<endl;
   
     // Setup B and C tagging thresholds according to Z+jet settings (Sami)
     double bthr(0.7527), cthr(0.3985), frac(0.5);
     if (TString(ds.c_str()).Contains("2016")) {
       //btagDeepB->set("2016",0.2217,0.6321,0.8953);
       //btagDeepC->set("2016",-0.48,-0.1,-0.1+frac*(0.69+0.1));
       //bthr = 0.8953; // tight
       bthr = 0.6321; // medium
       cthr = -0.1+frac*(0.69+0.1);
     }
     if (TString(ds.c_str()).Contains("2017")) {
       //btagDeepB->set("2017",0.1522,0.4941,0.8001);
       //btagDeepC->set("2017",0.05,0.15,0.15+frac*(0.8-0.15));
       //bthr = 0.8001; // tight
       bthr = 0.4941; // medium
       cthr = 0.15+frac*(0.8-0.15);
     }
     if (TString(ds.c_str()).Contains("2018")) {
       //btagDeepB->set("2018",0.1241,0.4184,0.7527);
       //btagDeepC->set("2018",0.04,0.137,0.137+frac*(0.66-0.137));
       //bthr = 0.7527; // tight
       bthr = 0.4184; // medium
       cthr = 0.137+frac*(0.66-0.137);
     }
     if (TString(ds.c_str()).Contains("22")  ||
         TString(ds.c_str()).Contains("23")) {
       // Copy of 2018
       bthr = 0.4184;
       cthr = 0.137+frac*(0.66-0.137);
     }
   
     // Create histograms. Copy format from existing files from Lyon
     // Keep only histograms actually used by global fit (reprocess.C)
     TDirectory *curdir = gDirectory;
     //TFile *fout = new TFile(oName, "RECREATE");
     assert(fout && !fout->IsZombie());
     
     // Original gamma+jet binning
     //     old bin trigger edges  (20,30,60,85,*95*,105,130,230)
     //double vx[] = {15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 105, 130, 175, 230,
     //		 300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750};
     // 22-23 binning
     double vx[] = {15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
     		 300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750,
   		 2100, 2500, 3000};
     const int nx = sizeof(vx)/sizeof(vx[0])-1;
     
     // L2L3Res eta binning
     double vy[] = {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
   		 -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
   		 -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
   		 1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
   		 2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
     const int ny = sizeof(vy)/sizeof(vy[0])-1;
   
     string dir = (tree->isMC ? "MC" : "DATA");
     
     vector<pair<double,double> > etas;
     etas.push_back(make_pair<double,double>(0,1.305));
     etas.push_back(make_pair<double,double>(0,2.500));
     
     vector<double> alphas;
     alphas.push_back(1.00); 
     alphas.push_back(0.30);
     alphas.push_back(0.20); 
     alphas.push_back(0.15);
     alphas.push_back(0.10);
     alphas.push_back(0.50); 
   
     // Setup HT bin weighting and monitoring
     TH1D *hxsec(0), *hnevt(0), *hsumw(0), *hLHE_HT(0), *hHT(0);
     //double vht_DiEle[] = {0, 25, 50, 100, 200, 300, 500, 700, 1000, 1500, 2000, 6500};
     double vht_DiEle[] = {0, 40, 70, 100, 200, 400, 600, 6500}; // G-4Jets
     const int nht_DiEle = sizeof(vht_DiEle)/sizeof(vht_DiEle[0])-1;
     int nMG_DiEle(0);
     double wMG_DiEle(0);
     if (isMG && !isQCD) {
   
       hxsec = new TH1D("hxsec",";H_{T} (GeV);pb",nht_DiEle,vht_DiEle);
       hnevt = new TH1D("hnevt",";H_{T} (GeV);N_{evt}",nht_DiEle,vht_DiEle);
       hsumw = new TH1D("hsumw",";H_{T} (GeV);Sum(weights)",nht_DiEle,vht_DiEle);
       hLHE_HT = new TH1D("hLHE_HT",";H_{T} (GeV);N_{evt} (unweighted)",
   		       nht_DiEle,vht_DiEle);
       hHT = new TH1D("hHT",";H_{T} (GeV);N_{evt} (weighted)",2485,15,2500);
   
       // Reference number of events, retrieved manuallay with
       // TChain c("Events"); c.AddFile("<path to files>/*.root"); c.GetEntries();
       // Also re-calculated this code before event loop when needed
       //int vnevt[nht] = {0, 0, 11197186, 23002929, 17512439, 16405924, 14359110,
       //		      13473185, 4365993, 2944561, 1836165};
       int vnevt[7] = {1, 6862, 7213, 5825, 6575, 3185, 2815}; // 2022EEP8 (local)
       double vsumw[7] = {0, 5.277e+08, 3.126e+08, 2.698e+08, 7.937e+07, 4.976e+06, 1.596e+06}; // 2022EEP8 (local)
       for (int i = 0; i != nht_DiEle; ++i) {
         hnevt->SetBinContent(i+1, vnevt[i]);
         nMG_DiEle += vnevt[i];
         hsumw->SetBinContent(i+1, vsumw[i]);
         wMG_DiEle += vsumw[i];
       }
       cout << "Loaded (local) MadGraph event numbers ("
   	 << nMG_DiEle << ", sumw=" << wMG_DiEle << ")" << endl << flush;
       
       // xsec from jetphys/settings.h_template
       //double vxsec[nht] = {0, 0, 246300000.*23700000./28060000., 23700000,
       //			 1547000, 322600, 29980, 6334, 1088, 99.11, 20.23};
       // pT^-4
       //double vxsec[nht] = {0, 1, 1.07e-1, 2.56e-2, 1.60e-3, 1.00e-4, 1.98e-5};
       // pT^-3
       //double vxsec[nht] = {0, 1, 1.86e-1, 6.40e-2, 8.00e-3, 1.00e-3, 2.97e-4};
       // pT^-2
       //double vxsec[nht] = {0, 1, 3.27e-1, 1.60e-1, 4.00e-2, 1.00e-2, 4.44e-3};
       // pT^-2 + fine tuning on hHT histogram (a*pT^b fits at boundary)
       //double k70 = 5056./3181.;
       //double k100 = 1475./831.3 * k70;
       //double k200 = 106.8/124.3 * k100;
       //double k400 = 9.339/21.11 * k200;
       //double k600 = 3.622/5.122 * k400;
       //double vxsec[nht] = {0, 1, k70*3.27e-1, k100*1.60e-1, k200*4.00e-2,
       //			 k400*1.00e-2, k600*4.44e-3};
     //double vxsec[7] = {0, 2.727, 1.417, 1.23, 0.2643, 0.02923, 0.009177}; // 2022EEP8 hand-adjusted, including vxsec[i] *= 4./1.467; for above
   
       // Values from Laurent Thomas, 20 Sep 2023, based on
       // https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer#Running_the_GenXSecAnalyzer_on_a
       double vxsec[nht_DiEle] = {0, 1.524e+04, 8.111e+03, 7.327e+03, 1.541e+03,
   			     1.676e+02, 5.439e+01}; // xsec in pb
       cout << Form("double vxsec[%d] = {",nht_DiEle);
       for (int i = 0; i != nht_DiEle; ++i) {
         //vxsec[i] *= 1.7893701e-4; // match 4to70 to 2.727 for 2022C test file
         hxsec->SetBinContent(i+1, vxsec[i]);
         cout << Form("%s%1.4g",i==0 ? "" : ", ", vxsec[i]);
       }
       cout << "}; // 2022EEP8 hand-adjusted" << endl << flush;
     }
   
     // Setup HT bin weighting and monitoring for QCD
     double vht_qcd2[] =
       {0, 25, 50, 100, 200, 300, 500, 700, 1000, 1500, 2000, 6500};
     const int nht_qcd2 = sizeof(vht_qcd2)/sizeof(vht_qcd2[0])-1;
     double vht_qcd3[] =
       {0, 40, 70, 100, 200, 400, 600, 800, 1000, 1200, 1500, 2000, 6800};
     const int nht_qcd3 = sizeof(vht_qcd3)/sizeof(vht_qcd3[0])-1;
     const double *vht_qcd = (tree->isRun3 ? &vht_qcd3[0] : &vht_qcd2[0]);
     const int nht_qcd = (tree->isRun3 ? nht_qcd3 : nht_qcd2);
     int nMG_qcd(0);
     double wMG_qcd(0);
     if (isMG && isQCD) {
        
        hxsec = new TH1D("hxsec",";H_{T} (GeV);pb",nht_qcd,vht_qcd);
        hnevt = new TH1D("hnevt",";H_{T} (GeV);N_{evt}",nht_qcd,vht_qcd);
        hsumw = new TH1D("hsumw",";H_{T} (GeV);Sum of weights",nht_qcd,vht_qcd);
        hLHE_HT = new TH1D("hLHE_HT",";H_{T} (GeV);N_{evt} (unweighted)",
   			nht_qcd,vht_qcd);
        //hLHE_HTw = new TH1D("hLHE_HTw",";H_{T} (GeV);N_{evt} (weighted)",
        //			 nht_qcd,vht_qcd);
        hHT = new TH1D("hHT",";H_{T} (GeV);N_{evt} (weighted)",2485,15,2500);
   
        // Reference number of events, retrieved manuallay with
        // TChain c("Events"); c.AddFile("<path to files>/*.root"); c.GetEntries();
        // Also re-calculated this code before event loop when needed
        int vnevt2[nht_qcd2] =
          {0, 0, 11197186, 23002929, 17512439, 16405924, 14359110,
   	13473185, 4365993, 2944561, 1836165};
        //int vnevt3[nht3] = {0, 9929, 26573, 16411, 10495, 8260, 7929, 10082, 14390, 6548, 6250}; // Summer22MG, local files
        int vnevt3[nht_qcd3] =
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Summer22MG, local files
        int vnwgt3[nht_qcd3] =
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Summer22MG, local files
        const int *vnevt = (tree->isRun3 ? &vnevt3[0] : &vnevt2[0]);
        const int *vnwgt = (tree->isRun3 ? &vnwgt3[0] : &vnevt2[0]);
        for (int i = 0; i != nht_qcd; ++i) {
          hnevt->SetBinContent(i+1, vnevt[i]);
          hsumw->SetBinContent(i+1, vnwgt[i]);
          nMG_qcd += vnevt[i];
          wMG_qcd += vnwgt[i];
        }
        cout << "Loaded Hefaistos MadGraph event numbers ("
   	  << nMG_qcd << ")" << endl << flush;
        
        // xsec from jetphys/settings.h_template
        double vxsec2[nht_qcd2] =
          {0, 0, 246300000.*23700000./28060000., 23700000,
   	1547000, 322600, 29980, 6334, 1088, 99.11, 20.23};
        // Run3 xsec from Mikel Mendizabal, MatterMost 16 Oct 2023
        double vxsec3[nht_qcd3] =
          {0, 3.136e+08,
   	5.883e+07, //5.807e+07 * 3478241.4 / 5305581.5, // HT 70to100
   	2.520e+07, 1.936e+06, 9.728e+04,
   	1.323e+04, //3.044e+04, //HT 60to800
   	3.027e+03, 8.883e+02, 3.834e+02, 1.253e+02, 2.629e+01};
        const double *vxsec = (tree->isRun3 ? &vxsec3[0] : &vxsec2[0]);
        for (int i = 0; i != nht_qcd; ++i) {
          hxsec->SetBinContent(i+1, vxsec[i]);
        }
      } // isMG && isQCD
   
     const double *vht = (isMG ? (isQCD ? &vht_qcd[0] : &vht_DiEle[0]) : 0);
     const int nht = (isMG ? (isQCD ? nht_qcd : nht_DiEle) : 0);
     int nMG = (isMG ? (isQCD ? nMG_qcd : nMG_DiEle) : 0);
     const int wMG = (isMG ? (isQCD ? wMG_qcd : wMG_DiEle) : 0);
     
     // PF composition plots stored in a separate directory
     fout->mkdir("pf");
     fout->cd("pf");  
     
     TH2D *h2pteta = new TH2D("h2pteta","",nx,vx,ny,vy);
     TProfile *pabseta = new TProfile("pabseta","",nx,vx);
   
     // 1D composition and response
     TProfile *pdb = new TProfile("pdb","",nx,vx);
     TProfile *pmpf = new TProfile("pmpf","",nx,vx);
     TProfile *pchf = new TProfile("pchf","",nx,vx);
     TProfile *pnhf = new TProfile("pnhf","",nx,vx);
     TProfile *pnef = new TProfile("pnef","",nx,vx);
     TProfile *pcef = new TProfile("pcef","",nx,vx);
     TProfile *pmuf = new TProfile("pmuf","",nx,vx);
     TProfile *ppuf = new TProfile("ppuf","",nx,vx);
   
     // for (int i = 0; i != 72; ++i) cout << Form("%1.3f, ",-TMath::Pi()+i*TMath::TwoPi()/72.); cout << endl;
     const int nphi = 72;
     const double vphi[nphi+1] = 
       {-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
        -2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
        -1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
        -0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
        0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
        0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
        1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
        2.618, 2.705, 2.793, 2.880, 2.967, 3.054,3.142};
   
     // 2D composition and response
     TProfile2D *p2db = new TProfile2D("p2db","",ny,vy,nphi,vphi);
     TProfile2D *p2mpf = new TProfile2D("p2mpf","",ny,vy,nphi,vphi);
     TProfile2D *p2chf = new TProfile2D("p2chf","",ny,vy,nphi,vphi);
     TProfile2D *p2nhf = new TProfile2D("p2nhf","",ny,vy,nphi,vphi);
     TProfile2D *p2nef = new TProfile2D("p2nef","",ny,vy,nphi,vphi);
     TProfile2D *p2cef = new TProfile2D("p2cef","",ny,vy,nphi,vphi);
     TProfile2D *p2muf = new TProfile2D("p2muf","",ny,vy,nphi,vphi);
     TProfile2D *p2puf = new TProfile2D("p2puf","",ny,vy,nphi,vphi);
   
     // Run control plots stored in a separate directory
     fout->mkdir("runs");
     fout->cd("runs");
   
     // Time stability of xsec
     TH1D *pr30n = new TH1D("pr30n",";Run;N_{events};",16000,355000.5,371000.5);
     TH1D *pr110n = new TH1D("pr110n",";Run;N_{events};",16000,355000.5,371000.5);
     TH1D *pr230n = new TH1D("pr230n",";Run;N_{events};",16000,355000.5,371000.5);
     TH1D *prg1n = new TH1D("prg1n",";Run;N_{events};",16000,355000.5,371000.5);
     
     // Time stability of JEC
     TProfile *pr30b = new TProfile("pr30b",";Run;BAL;",16000,355000.5,371000.5);
     TProfile *pr110b = new TProfile("pr110b",";Run;BAL;",16000,355000.5,371000.5);
     TProfile *pr230b = new TProfile("pr230b",";Run;BAL;",16000,355000.5,371000.5);
     TProfile *prg1b = new TProfile("prg1b",";Run;BAL;",16000,355000.5,371000.5);
     TProfile *pr30m = new TProfile("pr30m",";Run;MPF;",16000,355000.5,371000.5);
     TProfile *pr110m = new TProfile("pr110m",";Run;MPF;",16000,355000.5,371000.5);
     TProfile *pr230m = new TProfile("pr230m",";Run;MPF;",16000,355000.5,371000.5);
     TProfile *prg1m = new TProfile("prg1m",";Run;MPF;",16000,355000.5,371000.5);
   
     // Time stability of PF composition
     TProfile *pr30chf = new TProfile("pr30chf",";Run;CHF;",16000,355000.5,371000.5);
     TProfile *pr110chf = new TProfile("pr110chf",";Run;CHF;",16000,355000.5,371000.5);
     TProfile *pr230chf = new TProfile("pr230chf",";Run;CHF;",16000,355000.5,371000.5);
     TProfile *prg1chf = new TProfile("prg1chf",";Run;CHF;",16000,355000.5,371000.5);
     //
     TProfile *pr30nhf = new TProfile("pr30nhf",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *pr110nhf = new TProfile("pr110nhf",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *pr230nhf = new TProfile("pr230nhf",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *prg1nhf = new TProfile("prg1nhf",";Run;NHF;",16000,355000.5,371000.5);
     //
     TProfile *pr30nef = new TProfile("pr30nef",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *pr110nef = new TProfile("pr110nef",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *pr230nef = new TProfile("pr230nef",";Run;NHF;",16000,355000.5,371000.5);
     TProfile *prg1nef = new TProfile("prg1nef",";Run;NHF;",16000,355000.5,371000.5);
   
     // Control plots stored in a separate directory
     fout->mkdir("control");
     fout->cd("control");
     
     // Cut flow controls
     TProfile *pcutflow1 = new TProfile("pcutflow1",";Cut;Pass",20,-0.5,19.5);
     TProfile *pcutflow2 = new TProfile("pcutflow2",";Cut;Pass",20,-0.5,19.5);
     TProfile *pcutflow3 = new TProfile("pcutflow3",";Cut;Pass",20,-0.5,19.5);
   
     const int nc = 18;
     string cuts[nc] = {"pass_trig", "pass_nDiEle", "pass_njet", "pass_DiEleeta",
   		     "pass_dphi", "pass_jetid", "pass_veto", "pass_leak",
   		     "pass_basic", "pass_bal", "pass_mpf", "pass_basic_ext",
   		     "pass_jeteta", "pass_alpha100", "pass_all", "pass_gen"};
     for (int i = 0; i != nc; ++i) {
       pcutflow1->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
       pcutflow2->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
       pcutflow3->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
     }
   
     // Follow up on problematic cuts
     TH1D *hdphi = new TH1D("hdphi",";#Delta#phi;N_{events}",
   			 126,-TMath::TwoPi(),TMath::TwoPi());
     TH1D *hdr = new TH1D("hdr",";#DeltaR;N_{events}",100,0,10);
   		       
   
     
   
     // 1D plots for mu per trigger
     //hmusmc,20,30,50,75,90,100,110,200
     TH1D *hmus  = new TH1D("hmus","",100,0,100);
     TH1D *hmusmc = new TH1D("hmusmc","",100,0,100);
     TH1D *hmus200 = new TH1D("hmus200","",100,0,100);
   
     // 2D plots for mu vs DiEle pT
     TH2D *h2mus = new TH2D("h2mus","",nx,vx,100,0,100);
   
     // Plots of npvgood, npvall vs mu
     TProfile *pmuvsmu = new TProfile("pmuvsmu","",100,0,100);
     TProfile *prhovsmu = new TProfile("prhovsmu","",100,0,100);
     TProfile *pnpvgoodvsmu = new TProfile("pnpvgoodvsmu","",100,0,100);
     TProfile *pnpvallvsmu = new TProfile("pnpvallvsmu","",100,0,100);
     // Plots of DiEle corr, err, hoe, r9, vs mu 
     TProfile *pgainvsmu = new TProfile("pgainvsmu","",100,0,100);
     TProfile *pcorrvsmu = new TProfile("pcorrvsmu","",100,0,100);
     TProfile *perrvsmu = new TProfile("perrvsmu","",100,0,100);
     TProfile *DiEle_evsmu = new TProfile("DiEle_evsmu","",100,0,100);
     TProfile *pr9vsmu = new TProfile("pr9vsmu","",100,0,100);
     // ...and vs pT
     TProfile *pmuvspt = new TProfile("pmuvspt","",nx,vx);
     TProfile *prhovspt = new TProfile("prhovspt","",nx,vx);
     TProfile *pnpvgoodvspt = new TProfile("pnpvgoodvspt","",nx,vx);
     TProfile *pnpvallvspt = new TProfile("pnpvallvspt","",nx,vx);
     // ..and vs pT
     TProfile *pgain1vspt = new TProfile("pgain1vspt","",nx,vx);
     TProfile *pgain6vspt = new TProfile("pgain6vspt","",nx,vx);
     TProfile *pgain12vspt = new TProfile("pgain12vspt","",nx,vx);
     TProfile *pgainvspt = new TProfile("pgainvspt","",nx,vx);
     TProfile *pcorrvspt = new TProfile("pcorrvspt","",nx,vx);
     TProfile *perrvspt = new TProfile("perrvspt","",nx,vx);
     TH2D *h2hoevspt = new TH2D("h2hoevspt","",nx,vx,125,0,0.025);
     TProfile *DiEle_evspt = new TProfile("DiEle_evspt","",nx,vx);
     TH2D *h2r9vspt = new TH2D("h2r9vspt","",nx,vx,150,0.90,1.05);
     TProfile *pr9vspt = new TProfile("pr9vspt","",nx,vx);
   
     // 2D plots for jet response
     TH2D *h2bal = new TH2D("h2bal","",nx,vx,200,0,4);
     TH2D *h2mpf = new TH2D("h2mpf","",nx,vx,300,-2,4);
     TH2D *h2balc = new TH2D("h2balc","",nx,vx,200,0,4);
     TH2D *h2mpfc = new TH2D("h2mpfc","",nx,vx,300,-2,4);
     TH2D *h2balc2 = new TH2D("h2balc2","",nx,vx,200,0,4);
     TH2D *h2mpfc2 = new TH2D("h2mpfc2","",nx,vx,300,-2,4);
     
     // 2D and profile for DiEle-jet pT
     TH2D *h2_DiEle_j = new TH2D("h2_DiEle_j","",nx,vx,240,-0.1,0.5);
     TProfile *p_DiEle_j = new TProfile("p_DiEle_j","",nx,vx);
   
     // Extras without zero suppression and for gain paths
     TH2D *h2_DiEle_j0 = new TH2D("h2_DiEle_j0","",nx,vx,140,-0.2,0.5);
     TProfile *p_DiEle_j0 = new TProfile("p_DiEle_j0","",nx,vx);
     TH2D *h2_DiEle_j1 = new TH2D("h2_DiEle_j1","",nx,vx,140,-0.2,0.5);
     TProfile *p_DiEle_j1 = new TProfile("p_DiEle_j1","",nx,vx);
     TH2D *h2_DiEle_j6 = new TH2D("h2_DiEle_j6","",nx,vx,140,-0.2,0.5);
     TProfile *p_DiEle_j6 = new TProfile("p_DiEle_j6","",nx,vx);
     TH2D *h2_DiEle_j12 = new TH2D("h2_DiEle_j12","",nx,vx,140,-0.2,0.5);
     TProfile *p_DiEle_j12 = new TProfile("p_DiEle_j12","",nx,vx);
   
     // Plots for DiEle properties (more in MC)
     TH2D *h2Zetaphi = new TH2D("h2Zetaphi","",30,-1.305,+1.305,
   			       72,-TMath::Pi(),TMath::Pi());
     //TH2D *h2Zetaphi2 = new TH2D("h2Zetaphi2","",150,-1.305,+1.305,
     // Match barrel edge to 1.305 with 3.132, even though EC edge should be 3.139
     TH2D *h2Zetaphi2 = new TH2D("h2Zetaphi2","",360,-3.132,+3.132,
   				360,-TMath::Pi(),TMath::Pi());
     //TH2D *h2Zetaphi3 = new TH2D("h2Zetaphi3","",150,-1.305,+1.305,
     //				720,-TMath::Pi(),TMath::Pi());
     //TH2D *h2Zetaphi4 = new TH2D("h2Zetaphi4","",150,-1.305,+1.305,
     //				1440,-TMath::Pi(),TMath::Pi());
     TH2D *h2nZ = new TH2D("h2nZ","",nx,vx,5,0,5);
     TH1D *hgen = new TH1D("hgen","",nx,vx);
     TH1D *hZ = new TH1D("hZ","",nx,vx);
     TH1D *hZtrg = new TH1D("hZtrg","",nx,vx);
     TProfile *peffgr = new TProfile("peffgr","",nx,vx);
     TProfile *peffid = new TProfile("peffid","",nx,vx);
     TProfile *pfake = new TProfile("pfake","",nx,vx);
     TProfile *pfakeqcd = new TProfile("pfakeqcd","",nx,vx); // for QCD bkg
     TProfile *pfakeqcd2 = new TProfile("pfakeqcd2","",nx,vx); // for QCD bkg
     TH2D *h2rZ = new TH2D("h2rZ","",nx,vx,350,0.80,1.15);
     TH2D *h2rZqcd = new TH2D("h2rZqcd","",nx,vx,350,0.80,1.15); // for QCD bkg
     TProfile *prZ = new TProfile("prZ","",nx,vx);
     TProfile *prZqcd = new TProfile("prZqcd","",nx,vx); // for QCD bkg
     TProfile *prZqcd2 = new TProfile("prZqcd2","",nx,vx); // for QCD bkg
     TProfile *prZorigqcd = new TProfile("prZorigqcd","",nx,vx); // fpr QCD bkg
     TProfile *prZorigqcd2 = new TProfile("prZorigqcd2","",nx,vx); // QCD
     TH2D *h2cZ = new TH2D("h2cZ","",nx,vx,100,0.90,1.10);
     TProfile *pcZ = new TProfile("pcZ","",nx,vx);
   
     // Plots for jet properties
     TH2D *h2gjet = new TH2D("h2gjet","",nx,vx,100,0.90,1.10);
     TProfile *pgjet = new TProfile("pgjet","",nx,vx);
     TH2D *h2rjet = new TH2D("h2rjet","",nx,vx,100,0.90,1.10);
     TProfile *prjet = new TProfile("prjet","",nx,vx);
     TH2D *h2rgen = new TH2D("h2rgen","",nx,vx,100,0.90,1.10);
     TProfile *prgen = new TProfile("prgen","",nx,vx);
   
     TH2D *h2gjet2 = new TH2D("h2gjet2","",nx,vx,100,0.90,1.10);
     TProfile *pgjet2 = new TProfile("pgjet2","",nx,vx);
     TH2D *h2rjet2 = new TH2D("h2rjet2","",nx,vx,100,0.90,1.10);
     TProfile *prjet2 = new TProfile("prjet2","",nx,vx);
     TH2D *h2rgen2 = new TH2D("h2rgen2","",nx,vx,100,0.90,1.10);
     TProfile *prgen2 = new TProfile("prgen2","",nx,vx);
   
     // Plots for jet flavor
     /*
     TProfile *prgenud = new TProfile("prgenud","",nx,vx);
     TProfile *prgens = new TProfile("prgens","",nx,vx);
     TProfile *prgenc = new TProfile("prgenc","",nx,vx);
     TProfile *prgenb = new TProfile("prgenb","",nx,vx);
     TProfile *prgeng = new TProfile("prgeng","",nx,vx);
     TProfile *prgeno = new TProfile("prgeno","",nx,vx);
     TProfile *pgjetud = new TProfile("pgjetud","",nx,vx);
     TProfile *pgjets = new TProfile("pgjets","",nx,vx);
     TProfile *pgjetc = new TProfile("pgjetc","",nx,vx);
     TProfile *pgjetb = new TProfile("pgjetb","",nx,vx);
     TProfile *pgjetg = new TProfile("pgjetg","",nx,vx);
     TProfile *pgjeto = new TProfile("pgjeto","",nx,vx);
     TProfile *pfud = new TProfile("pfud","",nx,vx);
     TProfile *pfs = new TProfile("pfs","",nx,vx);
     TProfile *pfc = new TProfile("pfc","",nx,vx);
     TProfile *pfb = new TProfile("pfb","",nx,vx);
     TProfile *pfg = new TProfile("pfg","",nx,vx);
     TProfile *pfo = new TProfile("pfo","",nx,vx);
     */
   
     // Plots for DiEle response in data
     TProfile *prbal = new TProfile("prbal","",nx,vx);
     TProfile *prmpf = new TProfile("prmpf","",nx,vx);
     TProfile *prbal0 = new TProfile("prbal0","",980,20,1000);
     TProfile *prmpf0 = new TProfile("prmpf0","",980,20,1000);
     TProfile *prbal1 = new TProfile("prbal1","",nx,vx);
     TProfile *prmpf1 = new TProfile("prmpf1","",nx,vx);
     TProfile *prbal6 = new TProfile("prbal6","",nx,vx);
     TProfile *prmpf6 = new TProfile("prmpf6","",nx,vx);
     TProfile *prbal12 = new TProfile("prbal12","",nx,vx);
     TProfile *prmpf12 = new TProfile("prmpf12","",nx,vx);
     
     // Plots for DiEle trigger efficiencies
     // TBD: need to create these more systematically with a loop
     TH1D *hZ0_data = new TH1D("hZ0_data","",197,15,1000);
     TH1D *hZ0_mc = new TH1D("hZ0_mc","",197,15,1000);
     TH1D *hZ0 = new TH1D("hZ0","",197,15,1000);
     TH1D *hZ300 = new TH1D("hZ300","",197,15,1000);
     TH1D *hZtrig = new TH1D("hZtrig","",197,15,1000);
     TH1D *hZtrig_data = new TH1D("hZtrig_data","",197,15,1000);
     TH1D *hZtrig_mc = new TH1D("hZtrig_mc","",197,15,1000);
   
     // Flavor plots stored in a separate directory
     fout->mkdir("flavor");
     fout->cd("flavor");
   
     map<string, double> mvar;
     map<string, map<string, map<string, TH1*> > > mp;
     string avar[] = {"counts","mpfchs1","ptchs","mpf1","mpfn","mpfu","rho",
   		   "rjet","gjet","rgen"};
     string atag[] = {"i","b","c","q","g","n"};
     //string aflv[] = {"i","b","c","q","g","n"};
     string aflv[] = {"i","b","c","q","s","ud","g","n"};
     const int nvar = sizeof(avar)/sizeof(avar[0]);
     const int ntag = sizeof(atag)/sizeof(atag[0]);
     const int nflv = sizeof(aflv)/sizeof(aflv[0]);
     
     cout << "Creating flavor histograms/profiles in folder flavor/..." << endl;
     for (int ivar = 0; ivar != nvar; ++ivar) {
       for (int itag = 0; itag != ntag; ++itag) {
         for (int iflv = 0; iflv != nflv; ++iflv) {
   	string& var = avar[ivar]; const char *cv = var.c_str();
   	string& tag = atag[itag]; const char *ct = tag.c_str();
   	string& flv = aflv[iflv]; const char *cf = flv.c_str();
   	if (var=="counts")
   	  mp[var][tag][flv] = new TH1D(Form("%s_g%s%s",cv,ct,cf),"",nx,vx);
   	else
   	  mp[var][tag][flv] = new TProfile(Form("%s_g%s%s",cv,ct,cf),"",nx,vx);
         } // for iflv
       } // for itag
     } // for ivar
   
     // Results similar to Multijet in dijet package
     MoreHistosDiEleJet *hg(0);
     if (doGamjet) {
   
       // L3Res (DiEleJet) pT binning adapted and extended
       //const double vpt[] = {15, 20, 25, 30, 35,
       //			  40, 50, 60, 70, 85, 100, 125, 155, 180, 210, 250, 300,
       //			  350, 400, 500, 600, 800, 1000, 1200, 1500,
       //			  1800, 2100, 2400, 2700, 3000};
       // 22-23 binning
       const double vpt[] = {15, 20, 25, 30, 35,
   			  40, 50, 60, 75, 90, 110, 130, 175, 230, 300,
   			  400, 500, 600, 700, 850, 1000, 1200, 1450,
   			  1750, 2100, 2500, 3000};
       double npt = sizeof(vpt)/sizeof(vpt[0])-1;
   
       if (debug) cout << "Setup doGamjet " << endl << flush;
         
       fout->mkdir("Gamjet");
       fout->cd("Gamjet");
   
       MoreHistosDiEleJet *h = new MoreHistosDiEleJet();
       hg = h;
   
       // Counting of events, and JEC L2L3Res+JERSF for undoing
       h->hpt13 = new TH1D("hpt13",";p_{T,DiEle};N_{events}",npt,vpt);
       h->hpt13a = new TH1D("hpt13a",";p_{T,avg};N_{events}",npt,vpt);
       h->hpt13j = new TH1D("hpt13j",";p_{T,jet};N_{events}",npt,vpt);
       
       h->pptg = new TProfile("pptg",";p_{T,DiEle};p_{T,DiEle}",npt,vpt);
       h->pptj = new TProfile("pptj",";p_{T,DiEle};p_{T,jet}",npt,vpt);
   
      // MPF decomposition for HDM method
       h->pres = new TProfile("pres",";p_{T,DiEle} (GeV);JES",npt,vpt);
       h->pjsf = new TProfile("pjsf",";p_{T,DiEle} (GeV);JERSF",npt,vpt);
       h->pm0 = new TProfile("pm0",";p_{T,DiEle} (GeV);MPF0",npt,vpt);
       h->pm2 = new TProfile("pm2",";p_{T,DiEle} (GeV);MPF2",npt,vpt);
       h->pmn = new TProfile("pmn",";p_{T,DiEle} (GeV);MPFn",npt,vpt);
       h->pmu = new TProfile("pmu",";p_{T,DiEle} (GeV);MPFu",npt,vpt);
       
       h->pm0x = new TProfile("pm0x",";p_{T,DiEle} (GeV);MPF0X (MPFX)", npt, vpt, "S");
       h->pm2x = new TProfile("pm2x",";;p_{T,DiEle} (GeV);MPF2X (DBX)", npt, vpt, "S");
   
       // Extra for FSR studies
       h->pmnu = new TProfile("pmnu",";p_{T,DiEle} (GeV);MPFnu", npt, vpt);
       h->pmnx = new TProfile("pmnx",";p_{T,DiEle} (GeV);MPFNX", npt, vpt, "S");
       h->pmux = new TProfile("pmux",";p_{T,DiEle} (GeV);MPFUX", npt, vpt, "S");
       h->pmnux = new TProfile("pmnux",";p_{T,DiEle} (GeV;MPFNUX", npt, vpt, "S");
       
       // Composition
       h->prho = new TProfile("prho",";p_{T,DiEle};#rho (GeV)",npt,vpt);
       h->pchf = new TProfile("pchf",";p_{T,DiEle};CHF",npt,vpt);
       h->pnhf = new TProfile("pnhf",";p_{T,DiEle};NHF",npt,vpt);
       h->pnef = new TProfile("pnef",";p_{T,DiEle};NEF",npt,vpt);
   
       // Alternative pT bins
       h->presa = new TProfile("presa",";p_{T,avp} (GeV);JES",npt,vpt);
       h->pm0a = new TProfile("pm0a",";p_{T,avp} (GeV);MPF0",npt,vpt);
       h->pm2a = new TProfile("pm2a",";p_{T,avp} (GeV);MPF2",npt,vpt);
       h->pmna = new TProfile("pmna",";p_{T,avp} (GeV);MPFn",npt,vpt);
       h->pmua = new TProfile("pmua",";p_{T,avp} (GeV);MPFu",npt,vpt);
       //
       h->presj = new TProfile("presj",";p_{T,jet} (GeV);JES",npt,vpt);
       h->pm0j = new TProfile("pm0j",";p_{T,jet} (GeV);MPF0",npt,vpt);
       h->pm2j = new TProfile("pm2j",";p_{T,jet} (GeV);MPF2",npt,vpt);
       h->pmnj = new TProfile("pmnj",";p_{T,jet} (GeV);MPFn",npt,vpt);
       h->pmuj = new TProfile("pmuj",";p_{T,jet} (GeV);MPFu",npt,vpt);
       
     } // doGamjet
     
     // Results similar to Dijet2 directory in dijet package
     MoreHistosDiEleJet2 *hg2(0);
     if (doGamjet2) {
   
       // L2Res pT binning (central+forward hybrid)
       double vptd[] = 
       //{59.,85.,104.,170.,236., 302., 370., 460., 575.}; // central
       //{86., 110., 132., 204., 279., 373.} // forward
         {15, 21, 28, 37, 49,
          59, 86, 110, 132, 170, 204, 236, 279, 302, 373, 460, 575,
          638, 737, 846, 967, 1101, 1248,
          1410, 1588, 1784, 2000, 2238, 2500, 2787, 3103};
       double nptd = sizeof(vptd)/sizeof(vptd[0])-1;
       // L3Res (gamma+jet) pT binning adapted and extended
       const double vpt[] = {15, 20, 25, 30, 35,
   			  40, 50, 60, 70, 85, 100, 125, 155, 180, 210, 250, 300,
   			  350, 400, 500, 600, 800, 1000, 1200, 1500,
   			  1800, 2100, 2400, 2700, 3000};
       double npt = sizeof(vpt)/sizeof(vpt[0])-1;
   
       // Current L2Res |eta| binning from Jindrich
       // https://indico.cern.ch/event/1263476/contributions/5311425/attachments/2612023/4513129/L2Res+HDM-March15.pdf
       double vxd[] =
       //{0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322,
       // 2.5, 2.65, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
       // Newer L2Res |eta| binning from Mikel
       // https://indico.cern.ch/event/1335203/#7-update-on-l2res-for-2022-rer
         {0., 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191};
       const int nxd = sizeof(vxd)/sizeof(vxd[0])-1;
   
       if (debug) cout << "Setup doGamjet2 " << endl << flush;
         
       fout->mkdir("Gamjet2");
       fout->cd("Gamjet2");
   
       MoreHistosDiEleJet2 *h = new MoreHistosDiEleJet2();
       hg2 = h;
   
       // Counting of events, and JEC L2L3Res+JERSF for undoing
       h->h2pteta = new TH2D("h2pteta",";#eta;p_{T,avp} (GeV);"
   			  "N_{events}",nxd,vxd, nptd, vptd);
       h->p2res = new TProfile2D("p2res",";#eta;p_{T,avp} (GeV);JES",
   			      nxd,vxd, nptd, vptd);
       h->p2jsf = new TProfile2D("p2jsf",";#eta;p_{T,avp} (GeV);JERSF",
   			      nxd,vxd, nptd, vptd);
          
       // MPF decomposition for HDM method
       h->p2m0 = new TProfile2D("p2m0",";#eta;p_{T,avp} (GeV);MPF0",
   			     nxd,vxd, nptd, vptd);
       h->p2m2 = new TProfile2D("p2m2",";#eta;p_{T,avp} (GeV);MPF2",
   			     nxd,vxd, nptd, vptd);
       h->p2mn = new TProfile2D("p2mn",";#eta;p_{T,avp} (GeV);MPFn",
   			     nxd,vxd, nptd, vptd);
       h->p2mu = new TProfile2D("p2mu",";#eta;p_{T,avp} (GeV);MPFu",
   			     nxd,vxd, nptd, vptd);
       
       h->p2m0x = new TProfile2D("p2m0x",";#eta;p_{T,avp} (GeV);"
   			      "MPF0X (MPFX)",nxd,vxd, nptd, vptd, "S");
       h->p2m2x = new TProfile2D("p2m2x",";#eta;p_{T,avp} (GeV);"
   			      "MPF2X (DBX)",nxd,vxd, nptd, vptd, "S");
   
       // Extra for FSR studies
       h->p2mnu = new TProfile2D("p2mnu",";#eta;p_{T,avp} (GeV);MPFnu",
   			      nxd,vxd, nptd, vptd);
       h->p2mnx = new TProfile2D("p2mnx",";#eta;p_{T,avp} (GeV);"
   			      "MPFNX",nxd,vxd, nptd, vptd, "S");
       h->p2mux = new TProfile2D("p2mux",";#eta;p_{T,avp} (GeV);"
   			      "MPFUX",nxd,vxd, nptd, vptd, "S");
       h->p2mnux = new TProfile2D("p2mnux",";#eta;p_{T,avp} (GeV);"
   			       "MPFNUX",nxd,vxd, nptd, vptd, "S");
   
       /*
       h->h2ptetatc = new TH2D("h2ptetatc",";#eta;p_{T,tag} (GeV);"
   			    "N_{events}",nxd,vxd, nptd, vptd);
       h->p2restc = new TProfile2D("p2restc",";#eta;p_{T,tag} (GeV);"
   				"JES(probe)/JES(tag)",
   				nxd,vxd, nptd, vptd);
       h->p2jsftc = new TProfile2D("p2jsftc ",";#eta;p_{T,tag} (GeV);"
   				"JERSF(probe)/JERSF(tag)",
   				nxd,vxd, nptd, vptd);
       h->p2m0tc = new TProfile2D("p2m0tc",";#eta;p_{T,tag} (GeV);MPF0",
   			       nxd,vxd, nptd, vptd);
       h->p2m2tc = new TProfile2D("p2m2tc",";#eta;p_{T,tag} (GeV);MPF2",
   			       nxd,vxd, nptd, vptd);
       h->p2mntc = new TProfile2D("p2mntc",";#eta;p_{T,tag} (GeV);MPFn",
   			       nxd,vxd, nptd, vptd);
       h->p2mutc = new TProfile2D("p2mutc",";#eta;p_{T,tag} (GeV);MPFu",
   			       nxd,vxd, nptd, vptd);
       
       h->h2ptetapf = new TH2D("h2ptetapf",";#eta;p_{T,probe} (GeV);"
   			    "N_{events}",nxd,vxd, nptd, vptd);
       h->p2respf = new TProfile2D("p2respf",";#eta;p_{T,probe} (GeV);"
   				"JES(probe)/JES(tag)",
   				nxd,vxd, nptd, vptd);
       h->p2jsfpf = new TProfile2D("p2jsfpf",";#eta;p_{T,probe} (GeV);"
   				"JERSF(probe)/JERSF(tag)",
   				nxd,vxd, nptd, vptd);
       h->p2m0pf = new TProfile2D("p2m0pf",";#eta;p_{T,probe} (GeV);MPF0",
   			       nxd,vxd, nptd, vptd);
       h->p2m2pf = new TProfile2D("p2m2pf",";#eta;p_{T,probe} (GeV);MPF2",
   			       nxd,vxd, nptd, vptd);
       h->p2mnpf = new TProfile2D("p2mnpf",";#eta;p_{T,probe} (GeV);MPFn",
   			       nxd,vxd, nptd, vptd);
       h->p2mupf = new TProfile2D("p2mupf",";#eta;p_{T,probe} (GeV);MPFu",
   			       nxd,vxd, nptd, vptd);
       */    
     } // doGamjet2
     
   
     fout->cd();
     
     // Loop to create histograms and profiles
     // Match ordering to Lyon files (alpha->eta->data/MC) when creating
     // Although otherwise ordering is data/MC->eta->alpha
     // Add PS weight variations
     unsigned int nps = (tree->isMC ? tree->nPSWeightMax+1 : 1);
     //unsigned int nps = ((tree->isMC && !tree->isRun3) ? tree->nPSWeightMax+1 : 1);
     map<int, map<int, map<int, BasicHistosDiEleJet*> > > mBasicHistosDiEleJet;
     for (unsigned int ialpha = 0; ialpha != alphas.size(); ++ialpha) {
       for (unsigned int ieta = 0; ieta != etas.size(); ++ieta) { 
       for (unsigned int ips = 0; ips != nps; ++ips) {
   
         // Select data/MC, alpha and eta bin
         const char *cd = dir.c_str();
         int ia = int(100*alphas[ialpha]);
         int iy1 = int(10*etas[ieta].first);
         int iy2 = int(10*etas[ieta].second);
         int iy = 100*int(iy1) + int(iy2);
         const char *cps = (ips==0 ? "" : Form("_ps%d",ips-1));
         
         // Counts
         TH1D *hn = new TH1D(Form("resp_MPFchs_%s_a%d_eta%02d_%02d_RawNEvents"
   			       "_data_vs_pt%s",cd,ia,iy1,iy2,cps),"",nx,vx);
         TH1D *hxsec = new TH1D(Form("resp_MPFchs_%s_a%d_eta%02d_%02d_Xsec"
   				  "_data_vs_pt%s",cd,ia,iy1,iy2,cps),"",nx,vx);
         
         // Response profiles
         string name = Form("resp_%%s_%s_a%d_eta%02d_%02d%s",cd,ia,iy1,iy2,cps);
         const char *cname = name.c_str();
         TProfile *prpt  =  new TProfile(Form(cname,"PtGam"),"",nx,vx);
         TProfile *prbal =  new TProfile(Form(cname,"PtBalchs"),"",nx,vx);
         TProfile *prdb =  new TProfile(Form(cname,"DBchs"),"",nx,vx);
         TProfile *prmpf =  new TProfile(Form(cname,"MPFchs"),"",nx,vx);
         TProfile *prmpf1 = new TProfile(Form(cname,"MPFR1chs"),"",nx,vx);
         TProfile *prmpfn = new TProfile(Form(cname,"MPFRnchs"),"",nx,vx);
         TProfile *prmpfu = new TProfile(Form(cname,"MpfRuchs"),"",nx,vx);
         TProfile *prho = new TProfile(Form(cname,"Rho_CHS"),"",nx,vx);
         TProfile *pdjes = new TProfile(Form(cname,"DeltaJES"),"",nx,vx);
         TProfile *pjes = new TProfile(Form(cname,"JES"),"",nx,vx);
         TProfile *pres = new TProfile(Form(cname,"RES"),"",nx,vx);
   
         // Store links to histograms and profiles into maps
         BasicHistosDiEleJet *pmh = new BasicHistosDiEleJet();
         BasicHistosDiEleJet& mh = (*pmh);
         mh.hn = hn;
         mh.hxsec = hxsec;
         mh.prpt = prpt;
         mh.prbal = prbal;
         mh.prdb = prdb;
         mh.prmpf = prmpf;
         mh.prmpf1 = prmpf1;
         mh.prmpfn = prmpfn;
         mh.prmpfu = prmpfu;
         mh.prho = prho;
         mh.pdjes = pdjes;
         mh.pjes = pjes;
         mh.pres = pres;
         mBasicHistosDiEleJet[iy][ia][ips] = pmh;
       } // for ips in PSWeight
       } // for ieta in etas
     } // for ialpha in alphas
     
     curdir->cd();
     
     TLorentzVector Z, Zi, lhe, genZ, DiEle_j, DiEle_j0, DiEle_j0off, jet, jet2, jetn;
     TLorentzVector Zorig; // for QCD bkg
     TLorentzVector met, met1, metn, metu, metnu, rawmet, corrmet, rawZ;
     TLorentzVector jeti, corrjets, rawjet, rawjets, rcjet, rcjets, rcoffsets;
     TLorentzVector geni, genjet, genjet2;
     TLorentzVector fox; // for isQCD
     TLorentzVector Zx; // for MPFX
     
     //Long64_t nentries = tree->GetEntriesFast();
     Long64_t nentries = tree->GetEntries(); // Long startup time
     cout << "\nStarting loop over " << dataset << " with "
          << nentries << " entries" << endl;
   
     if (isMG && nentries!=nMG) {
       cout << "Nentries = "<<nentries<<", expected nMG = "<<nMG<<endl << flush;
        //assert(false);
       cout << "Recalculate HT bin counts prior to starting."
   	 << " This will take a few minutes" << endl;
       hnevt->Reset();
       for (Long64_t jentry=0; jentry<nentries;jentry++) {
         hnevt->Fill(tree->LHE_HT);
         hsumw->Fill(tree->LHE_HT, tree->genWeight);
         if (jentry%1000000==0) cout << "." << flush;
         if (jentry%50000000==0 && jentry!=0) cout << "\nn="<<jentry<<endl<<flush;
       } // for jentry
       nMG = nentries;
       cout << "\nProcessed " << nMG << " entries" << endl << flush;
       //
       cout << Form("int vnevt[%d] = ",hnevt->GetNbinsX());
       for (int i = 1; i != hnevt->GetNbinsX()+1; ++i) {
         cout<<Form("%s%d",(i==1 ? "{" : ", "),int(hnevt->GetBinContent(i)+0.5));
       }
       cout << "}; // " << dataset << endl << flush;
       //
       cout << Form("double vsumw[%d] = ",hsumw->GetNbinsX());
       for (int i = 1; i != hsumw->GetNbinsX()+1; ++i) {
         cout<<Form("%s%1.4g",(i==1 ? "{" : ", "),hsumw->GetBinContent(i));
       }
       cout << "}; // " << dataset << endl << flush;
     } // tree->isMC && nentries!=nMG
     
     //int skip = 21700000; // 2018A first events without 110EB
     //int skip = 55342793; // 2018A first events with 92 DiEle
     //int skip = 265126992; // 2018A first events with 191 DiEles, 23 jets
     //int skip = 14648973; // 2017C bad HDM
   
     for (Long64_t jentry=0; jentry<nentries;jentry++) {
   
       // Skip events, typically for debugging purposes
       //if (jentry<skip) continue;
       if (jentry>10000) break;
       //if (_gh_debug && jentry%10000==0) cout << "," << endl << flush;
       
       ++_ntot;
       if (jentry==100000 || jentry==1000000 || jentry==1000000 ||
   	(jentry%1000000==0 && jentry<10000000) ||
   	(jentry%10000000==0 && jentry!=0) ||
   	jentry==nentries-1) {
         if (jentry==0) { laptime.Start(); }
         if (nentries!=0) {
   	cout << Form("\nProcessed %lld events (%1.1f%%) in %1.0f sec. "
   		     "(%1.0f sec. for last %d)",
   		     jentry, 100.*jentry/nentries, fulltime.RealTime(),
   		     laptime.RealTime(), nlap);
         }
         if (jentry!=0 && nlap!=0) {
   	cout << Form("\nEstimated runtime:  %1.0f sec. "
   		     " (%1.0f sec. for last %d)",
   		     1.*nentries/jentry*fulltime.RealTime(),
   		     1.*nentries/nlap*laptime.RealTime(),nlap) << flush;
   	laptime.Reset();
   	nlap = 0;
         }
         if (jentry==0) fulltime.Reset(); // Leave out initialization time
         fulltime.Continue();
         laptime.Continue();
       }
       if (jentry%10000==0) cout << "." << flush;
       ++nlap;
   
       tree->GetEntry(jentry);
        if(tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ){
   	        ++_nbadevents_trigger;
   	        continue;
        }
   
       // if (Cut(ientry) < 0) continue;
   
       // Safety check for rho being NaN
       if (!(tree->fixedGridRhoFastjetAll>=0 && tree->fixedGridRhoFastjetAll<150))
         tree->fixedGridRhoFastjetAll = 34; // average conditions
       
       // Sanity check PS weights
       if (!tree->isMC) { tree->nPSWeight = 0; }
       //if (!tree->isMC || tree->is22 || tree->is23) { tree->nPSWeight = 0; }
       assert(tree->nPSWeight<=tree->nPSWeightMax);
   
       // Does the run/LS pass the latest JSON selection?
       if (!tree->isMC && objS->loadedLumiJson[tree->run][tree->luminosityBlock]==0) {
         //_badjson.insert(pair<int, int>(run, lbn));
         ++_nbadevents_json;
         continue;
       }
       else 
         ++_nevents;
   
   
       // Safety checks for array sizes (segfault in 2018A)
       if (tree->nJet > tree->nJetMax || tree->nPhoton > tree->nPhotonMax) {
         cout << "Error: tree->nJet="<<tree->nJet<<" or tree->nPhoton="<<tree->nPhoton
   	   << " exceeding maximum. Skip event " << jentry << endl << flush;
         exit(0);
       }
       
       // Re-JEC + re-MET, if needed
       // Skipped for now      
       
       // Select leading DiEle. Use tight cut-based ID and PF relative isolation
       // Temporary: select DiEle based on LHE DiEle match
       int iGamGen(-1), iGam(-1), nGam(0);
       int iGamOrig(-1); // for QCD bkg
       genZ.SetPtEtaPhiM(0,0,0,0);
       Z.SetPtEtaPhiM(0,0,0,0);
       rawZ.SetPtEtaPhiM(0,0,0,0);
       DiEle_j.SetPtEtaPhiM(0,0,0,0);
       DiEle_j0.SetPtEtaPhiM(0,0,0,0);
   
       //------------------------------------------
       // Select Electrons
       //------------------------------------------
		vector<int> indexEle;
      	for(int i = 0; i < tree->nEle_; ++i){
            double eta = tree->eleEta_[i];
            double SCeta = eta + tree->eleDeltaEtaSC_[i];
            double absEta = TMath::Abs(eta);
            double absSCEta = TMath::Abs(SCeta);
            bool passEtaEBEEGap = (absSCEta < 1.4442) || (absSCEta > 1.566);
            if(passEtaEBEEGap && absEta <= 2.4 && tree->elePt_[i] >= 40.0 && tree->eleID_[i]){
                indexEle.push_back(i);
            }
        }
        if(indexEle.size()<2) continue;

       //------------------------------------------
       // Select Z boson
       //------------------------------------------
		vector<TLorentzVector> ZBosons;
		for (int j=0; j<indexEle.size(); j++){
			for (int k=0; k<indexEle.size(); k++){
				if(j==k) 
					continue;
				if((tree->eleCharge_[j])*(tree->eleCharge_[k]) == 1)
					continue;
				TLorentzVector ele1;
				TLorentzVector ele2;
				ele1.SetPtEtaPhiM(tree->elePt_[j], tree->eleEta_[j], tree->elePhi_[j], tree->eleMass_[j]);
				ele2.SetPtEtaPhiM(tree->elePt_[k], tree->eleEta_[k], tree->elePhi_[k], tree->eleMass_[k]);
				TLorentzVector ZBoson = ele1 + ele2;
				if ( abs(ZBoson.M() - 91.1876) > 10 ) 
					continue;
				ZBosons.push_back(ZBoson);
			}
		}

       //------------------------------------------
       // Select Gen Electrons
       //------------------------------------------
		vector<int> indexGenLep;
      	for(int i = 0; i < tree->nGenDressedLepton; ++i){
			if(tree->GenDressedLepton_pdgId[i]==11) 
                indexGenLep.push_back(i);
        }
        if(indexGenLep.size() != 2) continue;
		TLorentzVector lep1;
		TLorentzVector lep2;
		int i0 = indexGenLep.at(0);
		int i1 = indexGenLep.at(1);
		lep1.SetPtEtaPhiM(tree->GenDressedLepton_pt[i0], tree->GenDressedLepton_eta[i0], tree->GenDressedLepton_phi[i0], tree->GenDressedLepton_mass[i0]);
		lep2.SetPtEtaPhiM(tree->GenDressedLepton_pt[i1], tree->GenDressedLepton_eta[i1], tree->GenDressedLepton_phi[i1], tree->GenDressedLepton_mass[i1]);
		genZ = lep1 + lep2;

       	//Select tight DiEles and DiEle matching gen DiEle
       	for (int i = 0; i != ZBosons.size(); ++i) {
			Zi = ZBosons.at(i); 
			if(genZ.Pt()>0 && genZ.DeltaR(Zi)<0.2 && iGamGen==-1) {
				iGamGen = i;
         	} 
   			++nGam;
			if (iGam==-1) {
				iGam = i;
				Z = Zi;
			}
       } // for i in tree->nPhoton
   
       // Correct DiEle for gain1 and MPF for "footprint" (DiEle vs PFZma)
       rawZ = Z;
       if (iGam!=-1 && tree->Photon_seedGain[iGam]==1 && !tree->isMC) {
         //Z *= 1./1.01;
         // minitools/drawGainVsPt.C (add R_6/12+R_1/6, take MPF+statTowardsDB)
         if (!tree->isRun3) Z *= 1./1.011; // MPF=1.13+/-0.04%, DB=1.05+/-0.08%
         if ( tree->isRun3) Z *= 1./1.017; // MPF=1.74+/-0.07%, DB=1.41+/-0.16%
       }
       if (iGam!=-1 && !tree->isRun3) {
         // [0]+log(x)*([1]+log(x)*[2]) in range [15,1750] to MC p_DiEle_j0
         //1  p0           4.57516e-02   3.91871e-04   1.09043e-07   4.17033e-05
         //2  p1          -1.27462e-02   1.50968e-04   2.08432e-08   3.92715e-03
         //3  p2           1.07760e-03   1.45293e-05   3.93020e-09   1.65460e-01
         double x = max(60.,rawZ.Pt());
         double f = 4.57516e-02 + log(x) * ( -1.27462e-02 + log(x) * 1.07760e-03);
         rawZ *= (1+f);
       }
      
       // Photon-jet: uncorrected jet minus (uncorr.) DiEle minus L1RC
       if (iGam!=-1 && tree->Photon_jetIdx[iGam]!=-1) {
         //assert(tree->Photon_jetIdx[iGam]>=-1);
         //assert(tree->Photon_jetIdx[iGam]<tree->nJet);
         int idx = tree->Photon_jetIdx[iGam];
         if (!(idx<tree->nJet && idx>=0)) {
   	cout << endl << "idx = " << idx << " tree->nJet = " << tree->nJet << endl << flush;
   	cout << "Skip event " << tree->event << " in LS " << tree->luminosityBlock
   	     << " in  run " << tree->run << " in File: " << _filename << endl;
   	continue;
   	//assert(idx<tree->nJet);
         }
         DiEle_j.SetPtEtaPhiM(tree->Jet_pt[idx], tree->Jet_eta[idx], tree->Jet_phi[idx], tree->Jet_mass[idx]);
         DiEle_j *= (1-tree->Jet_rawFactor[idx]);
         if (rawZ.DeltaR(DiEle_j)<0.4) { // does not always hold in Run3
   	//DiEle_j.Pt() >= rawZ.Pt()) { // not always true in Run3 (esp. 2022C)
   	// 2022 data is missing proper Puppi DiEle protection for jets
   	// (but MET ok?)
   	//double r22 = max(0.15,min(1.0,(rawZ.Pt()-20.)/180.));
   	//DiEle_j -= (tree->is22v10 ? r22*rawZ : rawZ);
   	DiEle_j -= rawZ; // NanoV12
         }
         else {
   	if (cntErrDR<10) {
   	  cout << endl << "entry " << jentry << ", rawZ.DeltaR(DiEle_j) = "
   	       << rawZ.DeltaR(DiEle_j) << endl << flush;
   	  cout << "Skip event " << tree->event << " in LS " << tree->luminosityBlock
   	       << " in  run " << tree->run << " in File: " << _filename << endl;
   	  ++cntErrDR;
   	  if (cntErrDR==10) {
   	    cout << "Stop reporting rawZ.DeltaR, silently skip." << endl;
   	  }
   	}
   	DiEle_j.SetPtEtaPhiM(0,0,0,0);
   	continue;
         }
         DiEle_j0 = DiEle_j;
   
         // Calculate L1RC correction
         /*
         double corrl1rc(1.); // tree->isRun3
         DiEle_j *= corrl1rc;
   
         // Calculate L1RC correction without "zero suppression"
         double refpt = 30; // DiEle_j.Pt~0 leads to negative offset cutoff
         double corrl1rc0(1.); // tree->isRun3
         double off0 = (corrl1rc0 - 1) * refpt; // corr*ptref = (ptref-off)
         DiEle_j0off.SetPtEtaPhiM(off0,DiEle_j0.Eta(),DiEle_j0.Phi(),0.);
         DiEle_j0 -= DiEle_j0off;
       }
       */
   
       // For QCD background, emulate a DiEle+jet event by replacing
       // one of the leading jets with genjet taking the place of the DiEle
       int iFox(-1);
       fox.SetPtEtaPhiM(0,0,0,0);
       //if (isQCD && iGam==-1 && tree->nJet>=2) {
       if (isQCD && tree->nJet>=2) {
         // Save original good DiEle, if one was found
         iGamOrig = iGam;
         Zorig = Z;
   
         iFox = (jentry%2); // "random" selection from two leading jets
         // tree->Jet_genJetIdx would be great, but only there for UL18 nAOD? Maybe there
         int k = tree->Jet_genJetIdx[iFox];
         if (k>=0 && k<tree->nGenJet) {
   	Z.SetPtEtaPhiM(tree->GenJet_pt[k], tree->GenJet_eta[k], tree->GenJet_phi[k],
   			 tree->GenJet_mass[k]);
   	// NB: should remove UE clustered into Z. In Minsuk's rho_ZB_new.pdf
   	// QCD_CP5 has about 3.5 GeV/A of UE offset at generator level
   	double area = tree->Jet_area[iFox];
   	Z *= (Z.Pt()>0 ? 1 - 3.5*area/Z.Pt() : 1.);
   	rawZ = Z;
   
   	fox.SetPtEtaPhiM(tree->Jet_pt[iFox], tree->Jet_eta[iFox], tree->Jet_phi[iFox],
   			 tree->Jet_mass[iFox]);
   	fox *= (1-tree->Jet_rawFactor[iFox]);
   	// Calculate L1RC correction
   	double corrl1rc(1.); // tree->isRun3
   	fox *= corrl1rc;
   	// NB2: should also remove UE clustered into fox. In Minsuk's plot
   	// QCD_CP5 has about 2.5 GeV/A of UE offset at FullSim level
   	fox *= (fox.Pt()>0 ? 1 - 2.5*area/fox.Pt() : 1.);
   
   	// NB3: For consistency with Zma+jet, DiEle_j should still have reco UE
   	DiEle_j.SetPtEtaPhiM(2.5*area,0,0,0);
   	DiEle_j0 = DiEle_j;
   	nGam = 1;
         }
       } // isQCD
     
       // Event weights (1 for MadGraph)
       //bool tree->isMC = (run==1);
       assert((tree->isMC && tree->run==1) || (!tree->isMC && tree->run!=1));
       double w = (tree->isMC ? tree->genWeight : 1);
       if (isMG) {
         int iht = hxsec->FindBin(tree->LHE_HT);
         double xsec = hxsec->GetBinContent(iht);
         //double nevt = hnevt->GetBinContent(iht);
         //double wht = (nevt ? xsec / nevt : 1);
         double sumw = hsumw->GetBinContent(iht);
         double wht = (sumw ? xsec / sumw : 1);
         w *= wht;
         hLHE_HT->Fill(tree->LHE_HT); // cross-check hnevt afterwards
         hHT->Fill(tree->LHE_HT, w); // cross-check HT spectrum smoothness afterwards
       }
   
       //bool doPtHatFilter = true;
       //if (doPtHatFilter && tree->isMC) {
       //if ( isMG && 2.*tree->Pileup_pthatmax>LHE_HT) continue;
       //if (!isMG && tree->Pileup_pthatmax>Generator_binvar) continue;
       //}
       
       // Pileup
       double TruePUrms(0);
       if (!tree->isMC) tree->Pileup_nTrueInt = objS->getTruePU(tree->run,tree->luminosityBlock,&TruePUrms);
       double ptZ = Z.Pt();
   
       // Trigger selection. Take care to match pT bin edges
       // {15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 105, 130, 175, 230,
       //  300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750};
       // NB: Photon90 threshold could be 95, Photon175 coud be 185, if bins split?
       double pt = ptZ; // shorthand to make trigger selection more readable
       int itrg(0); // choose trigger for PU reweighing as side effect (hack...)
       bool pass_trig = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;

     
       // Select trigger pT bins by hand for QCD. Error prone...
       if (isQCD && !pass_trig) {
         pass_trig = 
   	((tree->is16 && 
   	  ((pt>=230            && (itrg=175)) ||
   	   (pt>=175 && pt<230  && (itrg=165)) ||
   	   (pt>=130 && pt<175  && (itrg=120)) ||
   	   (pt>=105 && pt<130  && (itrg=90)) ||
   	   (pt>=85  && pt<105  && (itrg=75)) ||
   	   (pt>=60  && pt<85   && (itrg=50)) ||
   	   (pt>=40  && pt<60   && (itrg=36)) ||
   	   (pt>=35  && pt<40   && (itrg=30)) ||
   	   (pt>=20  && pt<35   && (itrg=22))
   	   )) ||
   	 (tree->is17 &&
   	  ((pt>=230            && (itrg=200)) ||
   	   (pt>=175 && pt<230  && (itrg=165)) ||
   	   (pt>=130 && pt<175  && (itrg=120)) ||
   	   (pt>=105 && pt<130  && (itrg=90)) ||
   	   (pt>=85  && pt<105  && (itrg=75)) ||
   	   (pt>=60  && pt<85   && (itrg=50)) ||
   	   (pt>=35  && pt<60   && (itrg=30)) ||
   	   (pt>=20  && pt<35   && (itrg=20))
   	   )) ||
   	 (tree->is18 &&
   	  ((pt>=230           && (itrg=200))||
   	   (pt>=130 && pt<230 && (itrg=110))||
   	   (pt>=105 && pt<130 && (itrg=100))||
   	   (pt>=95  && pt<105 && (itrg=90)) ||
   	   (pt>=85  && pt<95  && (itrg=75)) ||
   	   (pt>=60  && pt<85  && (itrg=50)) ||
   	   (pt>=35  && pt<60  && (itrg=30)) ||
   	   (pt>=20  && pt<35  && (itrg=20))
   	   )) ||
   	 (tree->isRun3 &&
   	  ((pt>=230            && (itrg=200)) ||
   	   (pt>=110&&pt<230 && (itrg=110)) ||
   	   (pt>=90  && pt<110  && (itrg=90))  ||
   	   (pt>=75  && pt<90   && (itrg=75))  ||
   	   (pt>=50  && pt<75   && (itrg=50))  ||
   	   (pt>=30&&pt<50   && (itrg=30))  ||
   	   (pt>=20  && pt<30   && (itrg=20))
   	   ))
   	 );
       } // isQCD
   
       assert(itrg>0 || !pass_trig);
   
       // Reweight MC pileup (except for 22-23)
       if (tree->isMC && pass_trig && !tree->isRun3) {
         TH1D *hm = objS->loadedPuHist[dataset][1]; assert(hm);
         TH1D *hd = objS->loadedPuHist[sera][itrg];
         if (!hd) cout << "Missing objS->loadedPuHist[sera="<<sera<<"][itrg="<<itrg<<"]"
   		    << endl << flush;
         assert(hd);
         assert(hm->GetNbinsX()==hd->GetNbinsX());
         int k = hm->FindBin(tree->Pileup_nTrueInt);
         assert(hm->GetBinLowEdge(k)==hd->GetBinLowEdge(k));
         double nm  = hm->GetBinContent(k);
         assert(nm>0); // should never get here if hm made from fullMC
         double nd  = hd->GetBinContent(k);
         double wt = (nm>0 ? nd / nm : 0);
         w *= wt;
       }
       // Normalize data luminosity (except for 22-23)
       if (!tree->isMC && pass_trig && !tree->isRun3) {
         double lumi = objS->loadedPuLumi[sera][itrg];
         assert(lumi>0);
         w *= 1./lumi;
       }
   
       // Select leading jets. Just exclude DiEle, don't apply JetID yet
       Float_t         Jet_resFactor[tree->nJetMax]; // Custom addition
       Float_t         Jet_deltaJES[tree->nJetMax]; // Custom addition
       Float_t         Jet_CF[tree->nJetMax]; // Custom addition
       int iJet(-1), iJet2(-1), nJets(0);
       double djes(1), jes(1), res(1);
       jet.SetPtEtaPhiM(0,0,0,0);
       jet2.SetPtEtaPhiM(0,0,0,0);
       jetn.SetPtEtaPhiM(0,0,0,0);
       // Also calculate corrected type-I chsMET and HDM inputs
       corrjets.SetPtEtaPhiM(0,0,0,0);
       rawjets.SetPtEtaPhiM(0,0,0,0);
       rcjets.SetPtEtaPhiM(0,0,0,0);
       rcoffsets.SetPtEtaPhiM(0,0,0,0);
       //------------------------------------------------
       //Apply JEC
       //------------------------------------------------
       for (int i = 0; i != tree->nJet; ++i) {
           double rawJetPt = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
           double rawJetMass = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);
           double corrs = 1.0;
           if(isPrint)cout<<"---: Jet correction :---"<<endl;
           for(auto l2l3Ref:objS->loadedJetL2L3Refs){
               try{ 
                   auto corr = l2l3Ref->evaluate({tree->Jet_eta[i],rawJetPt}); 
                   corrs    = corr*corrs;
                   rawJetPt = corrs*rawJetPt;
                   rawJetMass = corrs*rawJetMass;
                   if(isPrint) cout<<"rawJetPt = "<<rawJetPt<<", corr = "<<corr<<", corrs = "<<corrs<<endl;
               } catch (const std::exception& e) {
                   cout<<"\nEXCEPTION: in l2l3Ref: "<<e.what()<<endl;
                   std::abort();
               }
           }
   	//double res = (v.size()>1 ? v[v.size()-1]/v[v.size()-2] : 1.);
   	//Jet_RES[i] = 1./res;
       res  = 1.0;
   	Jet_deltaJES[i] = (1./corrs) / (1.0 - tree->Jet_rawFactor[i]);
   	tree->Jet_pt[i] = rawJetPt ;
   	tree->Jet_mass[i] = rawJetMass; 
   	tree->Jet_rawFactor[i] = (1.0 - 1.0/corrs);
   	Jet_resFactor[i] = (1.0 - 1.0/res);
     
         // Smear jets
         if (smearJets) {
   	//assert(false);
         }
         
         // Check that jet is not DiEle and pTcorr>15 GeV
         if (tree->Jet_pt[i]>15 && (iGam==-1 || i != tree->Photon_jetIdx[iGam]) &&
   	  (!isQCD || i != iFox)) {
   	
   	//++nJets;
   	jeti.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
    	if (Z.DeltaR(jeti)<0.2) continue; // should not happen, but does?
   	++nJets;
   
   	if (iJet==-1) { // Leading jet for balance
   	  iJet = i;
   	  jet = jeti;
   	  djes = Jet_deltaJES[i];
   	  jes = (1.-tree->Jet_rawFactor[i]);
   	  res = (1.-Jet_resFactor[i]);
   	}
   	else { // Subleading jets 
   	  jetn += jeti;
   	  if (iJet2==-1) { // First subleading jet for alpha
   	    iJet2 = i;
   	    jet2 = jeti;
   	  }
   	}
   	
   	// Calculate L1RC correction
   	rawjet = (1-tree->Jet_rawFactor[i]) * jeti;
   	double corrl1rc(1.); // tree->isRun3
   	rcjet = corrl1rc * rawjet;
   	
   	// Corrected type-I chsMET calculation
   	corrjets += jeti;
   	rawjets += rawjet;
   	rcjets += rcjet;
   	rcoffsets += (rawjet - rcjet);
         } // non-DiEle jet
       } // for i in nJet
       
       // Select genjet matching leading and subleading reco jet
       int iGenJet(-1), iGenJet2(-1);
       genjet.SetPtEtaPhiM(0,0,0,0);
       genjet2.SetPtEtaPhiM(0,0,0,0);
       if (tree->isMC) {
         for (Int_t i = 0; i != tree->nGenJet; ++i) {
   	geni.SetPtEtaPhiM(tree->GenJet_pt[i],tree->GenJet_eta[i],tree->GenJet_phi[i],
   			  tree->GenJet_mass[i]);
   	if (iJet!=-1 && geni.DeltaR(jet)<0.4 && iGenJet==-1) {
   	  iGenJet = i;
   	  genjet = geni;
   	}
   	else if (iJet2!=-1 && geni.DeltaR(jet2)<0.4 && iGenJet2==-1) {
   	  iGenJet2 = i;
   	  genjet2 = geni;
   	}
         } // for i in tree->nGenJet
       } // tree->isMC
   
       // Set MET vectors
       if (tree->isRun3) {
         rawmet.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
       }
       else {
         rawmet.SetPtEtaPhiM(tree->ChsMET_pt, 0, tree->ChsMET_phi, 0);
       }
       if (isQCD && iFox!=-1) rawmet += fox - Z; // fox=rawjet-PU, Z=genjet
       else rawmet += rawZ - Z; // replace PF DiEle with Reco DiEle
       met1 = -jet -Z;
       metn = -jetn;
       //corrmet = rawmet +rawjets -corrjets -rcoffsets;
       corrmet = rawmet +rcjets -corrjets; // same as above
       // Unclustered MET from rawMET by taking out all the hard stuff
       // metu = rawmet +Z +rawjets -rcoffsets;
       // metu = rawmet +Z +rcjets;
       // Or equally well, from corrMET (modulo rounding errors)
       metu = corrmet +Z +corrjets;
       metnu = metn + 1.1*metu;
       met = corrmet;
       
       // Make MET transverse
       corrmet.SetPz(0);
       met.SetPz(0);
       metn.SetPz(0);
       met1.SetPz(0);
       metu.SetPz(0);
       
       // Calculate basic variables
       double ptjet = jet.Pt();
       double abseta = fabs(jet.Eta());
       double pt2 = jet2.Pt();
       double pt2min = 30;
       double bal(0), mpf(0), mpf1(0), mpfn(0), mpfu(0), mpfnu(0);
       double mpfx(0), mpf1x(0), mpfnx(0), mpfux(0), mpfnux(0);
       if (ptZ>0) {
         bal = ptjet / ptZ;
         mpf = 1 + met.Vect().Dot(Z.Vect()) / (ptZ*ptZ);
         mpf1 = 1 + met1.Vect().Dot(Z.Vect()) / (ptZ*ptZ);
         mpfn = metn.Vect().Dot(Z.Vect()) / (ptZ*ptZ);
         mpfu = metu.Vect().Dot(Z.Vect()) / (ptZ*ptZ);
         mpfnu = metnu.Vect().Dot(Z.Vect()) / (ptZ*ptZ);
         //
         Zx.SetPtEtaPhiM(Z.Pt(),Z.Eta(),Z.Phi()+0.5*TMath::Pi(),0.);
         mpfx = 1 + met.Vect().Dot(Zx.Vect()) / (ptZ*ptZ);
         mpf1x = 1 + met1.Vect().Dot(Zx.Vect()) / (ptZ*ptZ);
         mpfnx = metn.Vect().Dot(Zx.Vect()) / (ptZ*ptZ);
         mpfux = metu.Vect().Dot(Zx.Vect()) / (ptZ*ptZ);
         mpfnux = metnu.Vect().Dot(Zx.Vect()) / (ptZ*ptZ);
       }
       
       // Sanity checks for HDM inputs
       if (!(fabs(mpf1+mpfn+mpfu-mpf)<1e-4)) {
         cout << "\nHDM input error: mpf=" << mpf << " mpf1=" << mpf1
   	   << " mpfn=" << mpfn << " mpfu=" << mpfu << endl;
         cout << "Difference = " << mpf1+mpfn+mpfu-mpf << endl << flush;
         //assert(false);
         cout << "Skip entry " << jentry
   	   << " ("<<tree->run<<","<<tree->luminosityBlock<<","<<tree->event<<")"
   	   << " in file " << _filename << endl << flush;
         continue;
       }
   
       // Event filters for 2016 and 2017+2018 data and MC
       // UL lists are separate, but all filter recommendations looked the same
       // Run3: https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#Run_3_recommendations
       bool pass_filt = 
         (//(tree->isRun3 && tree->Flag_METFilters>0) ||
          (tree->isRun3 &&
   	tree->Flag_goodVertices &&
   	tree->Flag_globalSuperTightHalo2016Filter &&
   	tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
   	tree->Flag_BadPFMuonFilter &&
   	tree->Flag_BadPFMuonDzFilter &&
   	tree->Flag_hfNoisyHitsFilter &&
   	tree->Flag_eeBadScFilter &&
   	tree->Flag_ecalBadCalibFilter) ||
          (!tree->isRun3 &&
   	tree->Flag_goodVertices &&
   	tree->Flag_globalSuperTightHalo2016Filter &&
   	tree->Flag_HBHENoiseFilter &&
   	tree->Flag_HBHENoiseIsoFilter &&
   	tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
   	tree->Flag_BadPFMuonFilter &&
   	//tree->Flag_BadPFMuonDzFilter && // new in UL, but not in nAOD?
   	//tree->Flag_BadChargedCandidateFilter && // not recommended
   	//tree->Flag_globalTightHalo2016Filter && // obsolete?
   	//tree->Flag_CSCTightHaloFilter // obsolete?
   	(tree->is16 || tree->Flag_ecalBadCalibFilter) && //new in UL, not for UL16
   	//(tree->isMC || tree->Flag_eeBadScFilter) // data only
   	tree->Flag_eeBadScFilter // MC added 7 July 2021
   	));
       //) || tree->isRun3; // pass_filt
       
       // Photon control plots
       h2nZ->Fill(ptZ, nGam, w);
       if (genZ.Pt()>0 && fabs(genZ.Eta()) < 1.3) {
         hgen->Fill(genZ.Pt(), w);
         peffgr->Fill(genZ.Pt(), iGamGen!=-1 ? 1 : 0, w);
         peffid->Fill(genZ.Pt(), iGam==iGamGen ? 1 : 0, w);
       }
       if (ptZ>0 && fabs(Z.Eta()) < 1.3 && pass_filt) {
         hZ->Fill(ptZ, w);
         if (tree->isMC) pfake->Fill(ptZ, iGam!=iGamGen ? 1 : 0, w);
         if (isQCD) {
   	bool hasorig = (iGamOrig!=-1 && Z.DeltaR(Zorig)<0.2);
   	bool inwindow = (fabs(Zorig.Pt() / ptZ - 0.9) < 0.2); // [0.8,1.1]
   	pfakeqcd->Fill(ptZ, hasorig ? 1 : 0, w);
   	pfakeqcd2->Fill(ptZ, hasorig && inwindow ? 1 : 0, w);
   	if (hasorig) {
   	  h2rZqcd->Fill(ptZ, Zorig.Pt() / ptZ, w);
   	  prZqcd->Fill(ptZ, Zorig.Pt() / ptZ, w);
   	  if (inwindow) prZqcd2->Fill(ptZ, Zorig.Pt() / ptZ, w);
   	  prZorigqcd->Fill(Zorig.Pt(), ptZ / Zorig.Pt(), w);
   	  if (inwindow) prZorigqcd2->Fill(Zorig.Pt(), ptZ / Zorig.Pt(), w);
   	} // hasorig
         }
         if (iGam==iGamGen && genZ.Pt()>0) {
   	h2rZ->Fill(genZ.Pt(), ptZ / genZ.Pt(), w);
   	prZ->Fill(genZ.Pt(), ptZ / genZ.Pt(), w);
   	h2cZ->Fill(ptZ, genZ.Pt() / ptZ, w);
   	pcZ->Fill(ptZ, genZ.Pt() / ptZ, w);
         }
   
         // Plots for DiEle trigger efficiencies
         if (tree->isMC)  hZ0_mc->Fill(ptZ, w);
         if (!tree->isMC) hZ0_data->Fill(ptZ, w);
         
         hZ0 ->Fill(ptZ, w);
         // Backup high pT
         if (tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ)  hZ300->Fill(ptZ, w);
       } // barrel DiEle
   
         // Summary of combined trigger efficiencies
         if (ptZ>0 && fabs(Z.Eta())<1.3 && pass_trig && pass_filt) {
   	if (tree->isMC)  hZtrig_mc->Fill(ptZ, w);
   	if (!tree->isMC) hZtrig_data->Fill(ptZ, w);
   	hZtrig->Fill(ptZ, w); // 5 GeV bins to match hZ[trgX]
   	hZtrg->Fill(ptZ, w); // wider binning to higher pT (=hZ)
         }
         //if (ptZ>=105 && fabs(Z.Eta())<1.3 && pass_trig) {
         if (ptZ>=110 && pass_trig && pass_filt) {
   	h2Zetaphi->Fill(Z.Eta(), Z.Phi(), w);
   	h2Zetaphi2->Fill(Z.Eta(), Z.Phi(), w);
   	//h2Zetaphi3->Fill(Z.Eta(), Z.Phi(), w);
   	//h2Zetaphi4->Fill(Z.Eta(), Z.Phi(), w);
         }
         
         bool pass_nZ = (nGam>=1);
         bool pass_njet = (nJets>=1);
         bool pass_Zeta = (fabs(Z.Eta()) < 1.3);
         bool pass_dphi = (fabs(Z.DeltaPhi(jet)) > 2.7); // pi-0.44 as in KIT Z+j
         bool pass_jetid = (iJet!=-1 && tree->Jet_jetId[iJet]>=4); // tightLepVeto
         //------------------------------------------------
         //Check if jet to be vetoed
         //------------------------------------------------
         bool pass_veto = true;
         try{ 
             auto jvNumber= objS->loadedJetVetoRef->evaluate({objS->jetVetoKey, jet.Eta(), jet.Phi()});
             if(jvNumber>0){
               ++_nbadevents_veto;
               pass_veto = false;
             }
         } catch (const std::exception& e) {
             cout<<"\nEXCEPTION: in objS->loadedJetVetoRef: "<<e.what()<<endl;
             std::abort();
         }
       
         bool pass_leak = (DiEle_j.Pt()<0.06*ptZ);// || tree->isRun3);
         bool pass_basic = (pass_trig && pass_filt && pass_nZ && pass_njet &&
   			 pass_Zeta && pass_dphi && pass_jetid && pass_veto &&
   			 pass_leak); // add pass_Zeta v19 / 202111122 !
         
         // Control plots for jet response
         bool pass_bal = (fabs(1-bal)<0.7);
         bool pass_mpf = (fabs(1-mpf)<0.7);
         bool pass_jeteta = (abseta < 1.3);
         bool pass_alpha100 = (pt2 < ptZ || pt2 < pt2min);      
         bool pass_basic_ext = (pass_basic && pass_bal && pass_mpf);
         bool pass_all = (pass_basic_ext && pass_jeteta && pass_alpha100);
         bool pass_gen = (iGenJet!=-1);
   
         //const int nc = 18;
         bool cut[nc] = {pass_trig, pass_nZ, pass_njet, pass_Zeta,
   		      pass_dphi, pass_jetid, pass_veto, pass_leak,
   		      pass_basic, pass_bal, pass_mpf, pass_basic_ext,
   		      pass_jeteta, pass_alpha100, pass_all, pass_gen};
         bool passcuts(true);
         for (int i = 0; i != nc; ++i) {
   	pcutflow1->Fill(i, cut[i] ? 1 : 0, w);
   	if (passcuts) {
   	  pcutflow2->Fill(i, cut[i] ? 1 : 0, w);
   	  passcuts = cut[i];
   	}
   	pcutflow3->Fill(i, passcuts ? 1 : 0, w);
         }
         if (pass_trig && pass_nZ && pass_njet && pass_Zeta) {
   	hdphi->Fill(Z.DeltaPhi(jet), w);
   	hdr->Fill(Z.DeltaR(jet), w);
         }
   
         // Time controls for JES and PF composition
         if (pass_all) {
   	if (itrg==30 && ptZ>30) {
   	  pr30n->Fill(tree->run, w); 
   	  pr30b->Fill(tree->run, bal, w); 
   	  pr30m->Fill(tree->run, mpf, w);
   	  pr30chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  pr30nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  pr30nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
   	if (itrg==110 && ptZ>110) {
   	  pr110n->Fill(tree->run, w);
   	  pr110b->Fill(tree->run, bal, w); 
   	  pr110m->Fill(tree->run, mpf, w);
   	  pr110chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  pr110nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  pr110nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
   	if (itrg==200 && ptZ>230) {
   	  pr230n->Fill(tree->run, w);
   	  pr230b->Fill(tree->run, bal, w); 
   	  pr230m->Fill(tree->run, mpf, w);
   	  pr230chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  pr230nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  pr230nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==1) {
   	  prg1n->Fill(tree->run, w);
   	  prg1b->Fill(tree->run, bal, w); 
   	  prg1m->Fill(tree->run, mpf, w);
   	  prg1chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  prg1nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  prg1nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
         }
   
         if (_gh_debug100 && jentry<100) {
   	cout << "Event " << jentry << " decisions" << endl;
   	cout << Form("pass_nZ = %d, pass_njet = %d, pass_Zeta = %d "
   		     "pass_dphi = %d, pass_jetid = %d\n"
   		     "pass_veto = %d, pass_leak = %d, pass_basic = %d "
   		     "pass_bal = %d, pass_mpf = %d, \n"
   		     "pass_jeteta = %d, pass_alpha100 = %d, "
   		     "pass_basic_ext = %d, "
   		     "pass_gen = %d,\n"
   		     "pass_trig = %d, pass_filt = %d",
   		     pass_nZ, pass_njet, pass_Zeta,
   		     pass_dphi, pass_jetid,
   		     pass_veto, pass_leak, pass_basic,
   		     pass_bal, pass_mpf, pass_jeteta,
   		     pass_alpha100, pass_basic_ext,
   		     pass_gen, pass_trig, pass_filt) << endl << flush;
   	cout << Form("Flags/Filters: goodVertices = %d, "
   		     "globalSuperTightHalo2016 = %d,\n"
   		     "HBHENoise = %d, "
   		     "HBHENoiseIso = %d, "
   		     "EcalDeadCellTriggerPrimitive = %d,\n"
   		     "BadPFMuon = %d, "
   		     "ecalBadCalib = %d, "
   		     "eeBadSc = %d.\n",  
   		     tree->Flag_goodVertices,
   		     tree->Flag_globalSuperTightHalo2016Filter,
   		     tree->Flag_HBHENoiseFilter,
   		     tree->Flag_HBHENoiseIsoFilter,
   		     tree->Flag_EcalDeadCellTriggerPrimitiveFilter,
   		     tree->Flag_BadPFMuonFilter,
   		     tree->Flag_ecalBadCalibFilter,
   		     tree->Flag_eeBadScFilter) << endl << flush;
         }
   
         if (pass_basic && pass_jeteta && pass_alpha100) {
   	h2bal->Fill(ptZ, bal, w);
   	h2mpf->Fill(ptZ, mpf, w);
   	if (pass_mpf) h2balc->Fill(ptZ, bal, w);
   	if (pass_bal) h2mpfc->Fill(ptZ, mpf, w);
   	if (pass_mpf && pass_bal) h2balc2->Fill(ptZ, bal, w);
   	if (pass_mpf && pass_bal) h2mpfc2->Fill(ptZ, mpf, w);
   	if (pass_basic_ext) {
   
   	  if (pass_gen || !tree->isMC) {
   	    h2rjet->Fill(ptZ, jet.Pt() / ptZ, w);
   	    prjet->Fill(ptZ, jet.Pt() / ptZ, w);
   	  }
   	  if (pass_gen) {
   	    h2gjet->Fill(ptZ, genjet.Pt() / ptZ, w);
   	    pgjet->Fill(ptZ, genjet.Pt() / ptZ, w);
   	    h2rgen->Fill(genjet.Pt(), jet.Pt() / genjet.Pt(), w);
   	    prgen->Fill(genjet.Pt(), jet.Pt() / genjet.Pt(), w);
   	  }
   
   	  int flv = (tree->isMC ? tree->GenJet_partonFlavour[iGenJet] : 99);
   	  mvar["counts"] = 1;
   	  mvar["mpfchs1"] = mpf;
   	  mvar["ptchs"] = bal;
   	  mvar["mpf1"] = mpf1;
   	  mvar["mpfn"] = mpfn;
   	  mvar["mpfu"] = mpfu;
   	  mvar["rho"] = tree->fixedGridRhoFastjetAll;
   	  mvar["rjet"] = (ptZ!=0 ? jet.Pt() / ptZ : 0);
   	  mvar["gjet"] = (ptZ!=0 ? genjet.Pt() / ptZ : 0);
   	  mvar["rgen"] = (genjet.Pt()!=0 ? jet.Pt() / genjet.Pt() : 0);
   
   	  if (tree->isRun3) { // temporary patch
   	    tree->Jet_btagDeepB[iJet] = tree->Jet_btagDeepFlavB[iJet];
   	    tree->Jet_btagDeepC[iJet] = 0.5*(tree->Jet_btagDeepFlavCvB[iJet] +
   				       tree->Jet_btagDeepFlavCvL[iJet]);
   	    tree->Jet_qgl[iJet] = tree->Jet_btagDeepFlavQG[iJet];
   	  }
   	  bool isb = (tree->Jet_btagDeepB[iJet] > bthr);
   	  bool isc = (tree->Jet_btagDeepC[iJet] > cthr && !isb);
   	  bool isq = (tree->Jet_qgl[iJet]>=0.5 && tree->Jet_qgl[iJet] && !isb && !isc);
   	  bool isg = (tree->Jet_qgl[iJet]>=0 && tree->Jet_qgl[iJet]<0.5 && !isb && !isc);
   	  bool isn = (!isb && !isc && !isq && !isg);
   	  
   	  for (int ivar = 0; ivar != nvar; ++ivar) {
   	    for (int itag = 0; itag != ntag; ++itag) {
   	      for (int iflv = 0; iflv != nflv; ++iflv) {
   
   		string& svr = avar[ivar]; const char *cv = svr.c_str();
   		string& stg = atag[itag]; const char *ct = stg.c_str();
   		string& sfl = aflv[iflv]; const char *cf = sfl.c_str();
   
   		if (((sfl=="i") ||
   		     (sfl=="b" && abs(flv)==5) ||
   		     (sfl=="c" && abs(flv)==4) ||
   		     (sfl=="q" && abs(flv)<=3 && flv!=0) ||
   		     (sfl=="s" && abs(flv)==3) ||
   		     (sfl=="ud" && abs(flv)<=2 && flv!=0) ||
   		     (sfl=="g" && flv==21) ||
   		     (sfl=="n" && flv==0)) &&
   		    ((stg=="i") ||
   		     (stg=="b" && isb) ||
   		     (stg=="c" && isc) ||
   		     (stg=="q" && isq) ||
   		     (stg=="g" && isg) ||
   		     (stg=="n" && isn))) {
   
   		  double x = ptZ;
   		  if (svr=="rgen") x = genjet.Pt();
   		  if ((svr=="rjet" || svr=="gjet") && iGenJet==-1) x = 0;
   		  double var = mvar[svr];
   		  TH1* h = mp[svr][stg][sfl];
   		  if (!h) {
   		    cout << "Missing "<<svr<<stg<<sfl<<endl<<flush;
   		    assert(h);
   		  }
   
   		  if (svr=="counts")
   		    ((TH1D*)h)->Fill(x, w);
   		  else
   		    ((TProfile*)h)->Fill(x, var, w);
   		}
   	      } // for iflv
   	    } // for itag
   	  } // for ivar
   
   	  /*
   	  double genpt = genjet.Pt();
   	  double rgen = jet.Pt() / genjet.Pt();
   	  double gZ = genjet.Pt() / Z.Pt();
   	  if (abs(flv)==1) prgenud->Fill(genpt, rgen, w);
   	  if (abs(flv)==2) prgenud->Fill(genpt, rgen, w);
   	  if (abs(flv)==3) prgens->Fill(genpt, rgen, w);
   	  if (abs(flv)==4) prgenc->Fill(genpt, rgen, w);
   	  if (abs(flv)==5) prgenb->Fill(genpt, rgen, w);
   	  if (flv==21)     prgeng->Fill(genpt, rgen, w);
   	  if (flv==0)      prgeno->Fill(genpt, rgen, w);
   	  if (abs(flv)==1) pgjetud->Fill(ptZ, gZ, w);
   	  if (abs(flv)==2) pgjetud->Fill(ptZ, gZ, w);
   	  if (abs(flv)==3) pgjets->Fill(ptZ, gZ, w);
   	  if (abs(flv)==4) pgjetc->Fill(ptZ, gZ, w);
   	  if (abs(flv)==5) pgjetb->Fill(ptZ, gZ, w);
   	  if (flv==21)     pgjetg->Fill(ptZ, gZ, w);
   	  if (flv==0)      pgjeto->Fill(ptZ, gZ, w);
   	  pfud->Fill(ptZ, abs(flv)==1||abs(flv)==2 ? 1 : 0, w);
   	  pfs->Fill(ptZ, abs(flv)==3 ? 1 : 0, w);
   	  pfc->Fill(ptZ, abs(flv)==4 ? 1 : 0, w);
   	  pfb->Fill(ptZ, abs(flv)==5 ? 1 : 0, w);
   	  pfg->Fill(ptZ, flv==21 ? 1 : 0, w);
   	  pfo->Fill(ptZ, flv==0 ? 1 : 0, w);
             */
   	}
   	if (pass_basic_ext && jet2.Pt()>0) {
   	  if (iGenJet2!=-1 || !tree->isMC) {
   	    h2rjet2->Fill(ptZ, jet2.Pt() / ptZ, w);
   	    prjet2->Fill(ptZ, jet2.Pt() / ptZ, w);
   	  }
   	  if (iGenJet2!=-1) {
   	    h2gjet2->Fill(ptZ, genjet2.Pt() / ptZ, w);
   	    pgjet2->Fill(ptZ, genjet2.Pt() / ptZ, w);
   	    h2rgen2->Fill(genjet2.Pt(), jet2.Pt() / genjet2.Pt(), w);
   	    prgen2->Fill(genjet2.Pt(), jet2.Pt() / genjet2.Pt(), w);
   	  }
   	}
         }
   
         // Control plots for trigger 
         if (ptZ>0 && pass_basic_ext && pass_jeteta && pass_alpha100) {
   	//     optimal trigger edges: (20,30,(35),55,80,95,105,115,210)
   	//     old bin trigger edges  (20,30,60,85,*95*,105,130,230)
   	double pt = ptZ;
   	double mu = tree->Pileup_nTrueInt;
   	if (tree->isMC                             && pt>210)  hmusmc->Fill(mu, w);
   	int nmax = (tree->isMC ? 1 : 100);
   	for (int i=0; i!=nmax; ++i) {
   	  mu = gRandom->Gaus(tree->Pileup_nTrueInt,TruePUrms);
   	  double w1 = 0.01*w;
   	  if (tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ                    && pt>=210) hmus200->Fill(mu,w1);
   	} // for i in nmax
         } // control plots for triggers
   
         // Control plots for DiEle-jet 
         if (ptZ>0 && pass_basic_ext) {
   
   	h2_DiEle_j->Fill(ptZ, DiEle_j.Pt()/ptZ, w);
   	p_DiEle_j->Fill(ptZ, DiEle_j.Pt()/ptZ, w);
   
   	double footprint =
   	  (fabs(Z.DeltaPhi(DiEle_j0))<0.4 ? 1 : -1)*DiEle_j0.Pt()/ptZ;
   	h2_DiEle_j0->Fill(ptZ, footprint, w);
   	p_DiEle_j0->Fill(ptZ, footprint, w);
   
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==1) {
   	  h2_DiEle_j1->Fill(ptZ, footprint, w);
   	  p_DiEle_j1->Fill(ptZ, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal1->Fill(ptZ, bal, w);
   	    prmpf1->Fill(ptZ, mpf, w);
   	  }
   	}
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==6) {
   	  h2_DiEle_j6->Fill(ptZ, footprint, w);
   	  p_DiEle_j6->Fill(ptZ, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal6->Fill(ptZ, bal, w);
   	    prmpf6->Fill(ptZ, mpf, w);
   	  }
   	}
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==12) {
   	  h2_DiEle_j12->Fill(ptZ, footprint, w);
   	  p_DiEle_j12->Fill(ptZ, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal12->Fill(ptZ, bal, w);
   	    prmpf12->Fill(ptZ, mpf, w);
   	  }
   	}
   
   	if (pass_jeteta && pass_alpha100) {
   	  prbal->Fill(ptZ, bal, w);
   	  prmpf->Fill(ptZ, mpf, w);
   	  prbal0->Fill(ptZ, bal, w);
   	  prmpf0->Fill(ptZ, mpf, w);
   	  
   	  // PF composition plots
   	  h2pteta->Fill(ptZ, tree->Jet_eta[iJet], w);
   	  pabseta->Fill(ptZ, fabs(tree->Jet_eta[iJet]), w);
   
   	  // 1D composition and response
   	  pdb->Fill(ptZ, bal, w);
   	  pmpf->Fill(ptZ, mpf, w);
   	  pchf->Fill(ptZ, tree->Jet_chHEF[iJet], w);
   	  pnhf->Fill(ptZ, tree->Jet_neHEF[iJet], w);
   	  pnef->Fill(ptZ, tree->Jet_neEmEF[iJet], w);
   	  pcef->Fill(ptZ, tree->Jet_chEmEF[iJet], w);
   	  pmuf->Fill(ptZ, tree->Jet_muEF[iJet], w);
   	  //if (tree->isRun3) tree->Jet_chFPV0EF[iJet] = 0;
   	  //ppuf->Fill(ptZ, tree->Jet_chFPV0EF[iJet], w);
   	  
   	  // 2D composition and response
   	  if (ptZ>230) {
   	    double eta = tree->Jet_eta[iJet];
   	    double phi = tree->Jet_phi[iJet];
   	    p2db->Fill(eta, phi, bal, w);
   	    p2mpf->Fill(eta, phi, mpf, w);
   	    p2chf->Fill(eta, phi, tree->Jet_chHEF[iJet], w);
   	    p2nhf->Fill(eta, phi, tree->Jet_neHEF[iJet], w);
   	    p2nef->Fill(eta, phi, tree->Jet_neEmEF[iJet], w);
   	    p2cef->Fill(eta, phi, tree->Jet_chEmEF[iJet], w);
   	    p2muf->Fill(eta, phi, tree->Jet_muEF[iJet], w);
   	    //p2puf->Fill(eta, phi, tree->Jet_chFPV0EF[iJet], w);
   	  }
   
   	  if (tree->isMC) {
   	    if (ptZ>=105 && ptZ<230)
   	      hmus->Fill(tree->Pileup_nTrueInt, w);
   	    h2mus->Fill(ptZ, tree->Pileup_nTrueInt, w);
   	  }
   	  else {
   	    for (int i=0; i!=100; ++i) {
   	      double mu = gRandom->Gaus(tree->Pileup_nTrueInt,TruePUrms);
   	      if (ptZ>=105 && ptZ<230)
   		hmus->Fill(mu, 0.01*w);
   	      h2mus->Fill(ptZ, mu, 0.01*w);
   	    } // for i in 100
   	  } // is MC
   	  //if (ptZ>=130 && ptZ<175) {
   	  //if ((tree->is16 && ptZ>175) ||
   	  //  (tree->is17 && ptZ>230) ||
   	  //  (tree->is18 && ptZ>130)) {
   	  if (ptZ>230 && iGam!=-1) {
   	    pgainvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_seedGain[iGam], w);
   	    if (tree->Photon_eCorr) // safety for 2016
   	      pcorrvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_eCorr[iGam], w);
   	    perrvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_energyErr[iGam], w);
   	    DiEle_evsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_hoe[iGam], w);
   	    pr9vsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_r9[iGam], w);
   	  }
   	  if (ptZ>230) {
   	    pmuvsmu->Fill(tree->Pileup_nTrueInt, tree->Pileup_nTrueInt, w);
   	    prhovsmu->Fill(tree->Pileup_nTrueInt, tree->fixedGridRhoFastjetAll, w);
   	    pnpvgoodvsmu->Fill(tree->Pileup_nTrueInt, tree->PV_npvsGood, w);
   	    pnpvallvsmu->Fill(tree->Pileup_nTrueInt, tree->PV_npvs, w);
   	  } // high pT range
   	  
   	  if (iGam!=-1) {
   	    pgain1vspt->Fill(ptZ, tree->Photon_seedGain[iGam]==1 ? 1 : 0, w);
   	    pgain6vspt->Fill(ptZ, tree->Photon_seedGain[iGam]==6 ? 1 : 0, w);
   	    pgain12vspt->Fill(ptZ, tree->Photon_seedGain[iGam]==12 ? 1 : 0, w);
   	    pgainvspt->Fill(ptZ, tree->Photon_seedGain[iGam], w);
   	    if (tree->Photon_eCorr) // safety for 2016
   	      pcorrvspt->Fill(ptZ, tree->Photon_eCorr[iGam], w);
   	    perrvspt->Fill(ptZ, tree->Photon_energyErr[iGam], w);
   	    h2hoevspt->Fill(ptZ, tree->Photon_hoe[iGam], w);
   	    DiEle_evspt->Fill(ptZ, tree->Photon_hoe[iGam], w);
   	    h2r9vspt->Fill(ptZ, tree->Photon_r9[iGam], w);
   	    pr9vspt->Fill(ptZ, tree->Photon_r9[iGam], w);
   	  }
   	  pmuvspt->Fill(ptZ, tree->Pileup_nTrueInt, w);
   	  prhovspt->Fill(ptZ, tree->fixedGridRhoFastjetAll, w);
   	  pnpvgoodvspt->Fill(ptZ, tree->PV_npvsGood, w);
   	  pnpvallvspt->Fill(ptZ, tree->PV_npvs, w);
   	} // barrel
         } // basic_ext cuts
         
         // Specific event selection for alpha and eta bins
         for (unsigned int ialpha = 0; ialpha != alphas.size(); ++ialpha) {
   	for (unsigned int ieta = 0; ieta != etas.size(); ++ieta) { 
   	for (int ips = 0; ips != tree->nPSWeight+1; ++ips) { 
   
   	  double alpha = alphas[ialpha];
   	  double ymin = etas[ieta].first;
   	  double ymax = etas[ieta].second;
   	  double wps = (ips==0 ? 1. : tree->PSWeight[ips-1]);
   	  
   	  // Specific event selection
   	  bool pass_alpha = (pt2 < alpha*ptZ || pt2 < pt2min);
   	  bool pass_eta = (abseta >= ymin && abseta < ymax);
   	  bool pass = (pass_basic_ext && pass_alpha && pass_eta);
   	  
   	  if (pass) {
   	    
   	    // Retrieve pointers to correct set of histograms/profiles
   	    int ia = int(100*alpha);
   	    int iy = 100*int(10*ymin) + int(10*ymax);
   	    
   	    // Get reference instead of pointer so can use . and not ->
   	    BasicHistosDiEleJet *pmh = mBasicHistosDiEleJet[iy][ia][ips]; assert(pmh);
   	    BasicHistosDiEleJet& mh = (*pmh); assert(mh.hn);
   	    
   	    // Fill histograms (h*) and profiles (p*)
   	    //assert(fabs(mpf1+mpfn+mpfu-mpf)<1e-4);
   	    mh.hn->Fill(ptZ);
   	    mh.hxsec->Fill(ptZ, w*wps);
   	    mh.prpt->Fill(ptZ, ptZ, w*wps);
   	    mh.prbal->Fill(ptZ, bal, w*wps);
   	    mh.prdb->Fill(ptZ, mpf1, w*wps);
   	    mh.prmpf->Fill(ptZ, mpf, w*wps);
   	    mh.prmpf1->Fill(ptZ, mpf1, w*wps);
   	    mh.prmpfn->Fill(ptZ, mpfn, w*wps);
   	    mh.prmpfu->Fill(ptZ, mpfu, w*wps);
   	    mh.prho->Fill(ptZ, tree->fixedGridRhoFastjetAll, w);
   	    mh.pdjes->Fill(ptZ, djes, w);
   	    mh.pjes->Fill(ptZ, jes, w);
   	    mh.pres->Fill(ptZ, res, w);
   	  } // pass
   	} // for ips in PSWeight
   	} // for ieta in etas
         } // for ialpha in alphas
   
         if (doGamjet && hg) {
   
   	MoreHistosDiEleJet *h = hg;
   
   	// Specific event selection
   	bool pass_alpha = (pt2 < 1.00*ptZ || pt2 < pt2min);
   	bool pass_eta = (fabs(tree->Jet_eta[iJet]) < 1.3);
   	bool pass = (pass_basic_ext && pass_alpha && pass_eta);
   
   	if (pass) {
   	
   	  double jsf = (smearJets ? Jet_CF[iJet] : 1);
   	  double ptjet = tree->Jet_pt[iJet];
   	  double ptavp = 0.5*(ptZ + ptjet); // Do better later, now pT,ave (not pT,avp)
   	  h->hpt13->Fill(ptZ, w);
   	  h->hpt13a->Fill(ptavp, w);
   	  h->hpt13j->Fill(ptjet, w);
   	  
   	  h->pptg->Fill(ptZ, ptZ, w);
   	  h->pptj->Fill(ptZ, ptjet, w);
   	  
   	  h->pres->Fill(ptZ, res, w);
   	  h->pjsf->Fill(ptZ, jsf, w);
   	  h->pm0->Fill(ptZ, mpf, w);
   	  h->pm2->Fill(ptZ, mpf1, w);
   	  h->pmn->Fill(ptZ, mpfn, w);
   	  h->pmu->Fill(ptZ, mpfu, w);
   	  
   	  h->pm0x->Fill(ptZ, mpfx, w);
   	  h->pm2x->Fill(ptZ, mpf1x, w);
   	  
   	  // Extras for FSR studies
   	  h->pmnu->Fill(ptZ, mpfnu, w);
   	  h->pmnx->Fill(ptZ, mpfnx, w);
   	  h->pmux->Fill(ptZ, mpfux, w);
   	  h->pmnux->Fill(ptZ, mpfnux, w);
   	  
   	  // Composition
   	  h->prho->Fill(ptZ, tree->fixedGridRhoFastjetAll, w);
   	  h->pchf->Fill(ptZ, tree->Jet_chHEF[iJet], w);
   	  h->pnhf->Fill(ptZ, tree->Jet_neHEF[iJet], w);
   	  h->pnef->Fill(ptZ, tree->Jet_neEmEF[iJet], w);
   	  
   	  // Alternative pT bins
   	  h->presa->Fill(ptavp, res, w);
   	  h->pm0a->Fill(ptavp, mpf, w);
   	  h->pm2a->Fill(ptavp, mpf1, w);
   	  h->pmna->Fill(ptavp, mpfn, w);
   	  h->pmua->Fill(ptavp, mpfu, w);
   	  //
   	  h->presj->Fill(ptjet, res, w);
   	  h->pm0j->Fill(ptjet, mpf, w);
   	  h->pm2j->Fill(ptjet, mpf1, w);
   	  h->pmnj->Fill(ptjet, mpfn, w);
   	  h->pmuj->Fill(ptjet, mpfu, w);
   	}
         } // doGamjet
         
         if (doGamjet2 && hg2) {
   
   	MoreHistosDiEleJet2 *h = hg2;
   
   	// Specific event selection
   	bool pass_alpha = (pt2 < 1.00*ptZ || pt2 < pt2min);
   	//bool pass_eta = (abseta >= ymin && abseta < ymax);
   	bool pass = (pass_basic_ext && pass_alpha);// && pass_eta);
   
   	if (pass) {
   	
   	  //double res = Jet_RES[iprobe] / Jet_RES[itag];
   	  double jsf = (smearJets ? Jet_CF[iJet] : 1);
   	  
   	  double abseta = fabs(tree->Jet_eta[iJet]);
   	  h->h2pteta->Fill(abseta, ptZ, w);
   	  
   	  h->p2res->Fill(abseta, ptZ, res, w);
   	  h->p2jsf->Fill(abseta, ptZ, jsf, w);
   	  h->p2m0->Fill(abseta, ptZ, mpf, w);
   	  h->p2m2->Fill(abseta, ptZ, mpf1, w);
   	  h->p2mn->Fill(abseta, ptZ, mpfn, w);
   	  h->p2mu->Fill(abseta, ptZ, mpfu, w);
   	  
   	  h->p2m0x->Fill(abseta, ptZ, mpfx, w);
   	  h->p2m2x->Fill(abseta, ptZ, mpf1x, w);
   	  
   	  // Extras for FSR studies
   	  h->p2mnu->Fill(abseta, ptZ, mpfnu, w);
   	  h->p2mnx->Fill(abseta, ptZ, mpfnx, w);
   	  h->p2mux->Fill(abseta, ptZ, mpfux, w);
   	  h->p2mnux->Fill(abseta, ptZ, mpfnux, w);
   	}
         } // doGamjet2
         
       } // for jentry in nentries
     }
     cout << endl << "Finished loop, writing file." << endl << flush;
     cout << "Processed " << _nevents << " events\n";
     cout << "Skipped " << _nbadevents_json << " events due to JSON ("
      << (100.*_nbadevents_json/_nevents) << "%) \n";
     cout << "Skipped " << _nbadevents_trigger << " events due to trigger ("
       	 << (100.*_nbadevents_trigger/_ntot) << "%) \n";
     cout << "Skipped " << _nbadevents_veto << " events due to veto ("
       	 << (100.*_nbadevents_veto/_nevents) << "%) \n";
   
     // Add extra plot for jet response vs DiEle pT
     if (tree->isMC) {
       fout->cd("control");
       TH1D *hrgenvgen = prgen->ProjectionX("hrgenvgen");
       TH1D *hrgenvZ = prjet->ProjectionX("hrgenvZ");
       hrgenvZ->Divide(pgjet);
       TH1D *hrgen2vgen = prgen2->ProjectionX("hrgen2vgen");
       TH1D *hrgen2vZ = prjet2->ProjectionX("hrgen2vZ");
       hrgen2vZ->Divide(pgjet2);
       curdir->cd();
     }
     fout->Write();
     cout << "File written." << endl << flush;
     fout->Close();
     cout << "File closed." << endl << flush;
     return 0;
}
   
