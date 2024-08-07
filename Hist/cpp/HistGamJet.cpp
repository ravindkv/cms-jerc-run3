#include "HistGamJet.h"
   
int HistGamJet::Run(TString oName, SkimTree *tree, ObjectScale *objS, TFile *fout){
    TStopwatch fulltime, laptime;
     fulltime.Start();
     TDatime bgn;
     int nlap(0);
   
     int _ntot(0), _nevents(0), _nbadevents_json(0), _nbadevents_trigger(0);
     int _nbadevents_veto(0);
     
     bool isPrint = false;
   
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
   
     string dir = (isMC ? "MC" : "DATA");
     
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
     //double vht_gam[] = {0, 25, 50, 100, 200, 300, 500, 700, 1000, 1500, 2000, 6500};
     double vht_gam[] = {0, 40, 70, 100, 200, 400, 600, 6500}; // G-4Jets
     const int nht_gam = sizeof(vht_gam)/sizeof(vht_gam[0])-1;
     int nMG_gam(0);
     double wMG_gam(0);
     if (isMG && !isQCD) {
       hxsec = new TH1D("hxsec",";H_{T} (GeV);pb",nht_gam,vht_gam);
       hnevt = new TH1D("hnevt",";H_{T} (GeV);N_{evt}",nht_gam,vht_gam);
       hsumw = new TH1D("hsumw",";H_{T} (GeV);Sum(weights)",nht_gam,vht_gam);
       hLHE_HT = new TH1D("hLHE_HT",";H_{T} (GeV);N_{evt} (unweighted)",
   		       nht_gam,vht_gam);
       hHT = new TH1D("hHT",";H_{T} (GeV);N_{evt} (weighted)",2485,15,2500);
   
       // Reference number of events, retrieved manuallay with
       // TChain c("Events"); c.AddFile("<path to files>/*.root"); c.GetEntries();
       // Also re-calculated this code before event loop when needed
       //int vnevt[nht] = {0, 0, 11197186, 23002929, 17512439, 16405924, 14359110,
       //		      13473185, 4365993, 2944561, 1836165};
       int vnevt[7] = {1, 6862, 7213, 5825, 6575, 3185, 2815}; // 2022EEP8 (local)
       double vsumw[7] = {0, 5.277e+08, 3.126e+08, 2.698e+08, 7.937e+07, 4.976e+06, 1.596e+06}; // 2022EEP8 (local)
       for (int i = 0; i != nht_gam; ++i) {
         hnevt->SetBinContent(i+1, vnevt[i]);
         nMG_gam += vnevt[i];
         hsumw->SetBinContent(i+1, vsumw[i]);
         wMG_gam += vsumw[i];
       }
       cout << "Loaded (local) MadGraph event numbers ("
   	 << nMG_gam << ", sumw=" << wMG_gam << ")" << endl << flush;
       
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
       double vxsec[nht_gam] = {0, 1.524e+04, 8.111e+03, 7.327e+03, 1.541e+03,
   			     1.676e+02, 5.439e+01}; // xsec in pb
       cout << Form("double vxsec[%d] = {",nht_gam);
       for (int i = 0; i != nht_gam; ++i) {
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
     const double *vht_qcd = (&vht_qcd3[0]);
     const int nht_qcd = (nht_qcd3);
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
        const int *vnevt = (&vnevt3[0]);
        const int *vnwgt = (&vnwgt3[0]);
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
        const double *vxsec = (&vxsec3[0]);
        for (int i = 0; i != nht_qcd; ++i) {
          hxsec->SetBinContent(i+1, vxsec[i]);
        }
      } // isMG && isQCD
   
     const double *vht = (isMG ? (isQCD ? &vht_qcd[0] : &vht_gam[0]) : 0);
     const int nht = (isMG ? (isQCD ? nht_qcd : nht_gam) : 0);
     int nMG = (isMG ? (isQCD ? nMG_qcd : nMG_gam) : 0);
     const int wMG = (isMG ? (isQCD ? wMG_qcd : wMG_gam) : 0);
     
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
     string cuts[nc] = {"pass_trig", "pass_ngam", "pass_njet", "pass_gameta",
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
     TH1D *hmus20 = new TH1D("hmus20","",100,0,100);
     TH1D *hmus30 = new TH1D("hmus30","",100,0,100);
     TH1D *hmus50 = new TH1D("hmus50","",100,0,100);
     TH1D *hmus75 = new TH1D("hmus75","",100,0,100);
     TH1D *hmus90 = new TH1D("hmus90","",100,0,100);
     TH1D *hmus100 = new TH1D("hmus100","",100,0,100);
     TH1D *hmus110 = new TH1D("hmus110","",100,0,100);
     TH1D *hmus200 = new TH1D("hmus200","",100,0,100);
   
     // 2D plots for mu vs photon pT
     TH2D *h2mus = new TH2D("h2mus","",nx,vx,100,0,100);
   
     // Plots of npvgood, npvall vs mu
     TProfile *pmuvsmu = new TProfile("pmuvsmu","",100,0,100);
     TProfile *prhovsmu = new TProfile("prhovsmu","",100,0,100);
     TProfile *pnpvgoodvsmu = new TProfile("pnpvgoodvsmu","",100,0,100);
     TProfile *pnpvallvsmu = new TProfile("pnpvallvsmu","",100,0,100);
     // Plots of photon corr, err, hoe, r9, vs mu 
     TProfile *pgainvsmu = new TProfile("pgainvsmu","",100,0,100);
     TProfile *pcorrvsmu = new TProfile("pcorrvsmu","",100,0,100);
     TProfile *perrvsmu = new TProfile("perrvsmu","",100,0,100);
     TProfile *phoevsmu = new TProfile("phoevsmu","",100,0,100);
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
     TProfile *phoevspt = new TProfile("phoevspt","",nx,vx);
     TH2D *h2r9vspt = new TH2D("h2r9vspt","",nx,vx,150,0.90,1.05);
     TProfile *pr9vspt = new TProfile("pr9vspt","",nx,vx);
   
     // 2D plots for jet response
     TH2D *h2bal = new TH2D("h2bal","",nx,vx,200,0,4);
     TH2D *h2mpf = new TH2D("h2mpf","",nx,vx,300,-2,4);
     TH2D *h2balc = new TH2D("h2balc","",nx,vx,200,0,4);
     TH2D *h2mpfc = new TH2D("h2mpfc","",nx,vx,300,-2,4);
     TH2D *h2balc2 = new TH2D("h2balc2","",nx,vx,200,0,4);
     TH2D *h2mpfc2 = new TH2D("h2mpfc2","",nx,vx,300,-2,4);
     
     // 2D and profile for photon-jet pT
     TH2D *h2phoj = new TH2D("h2phoj","",nx,vx,240,-0.1,0.5);
     TProfile *pphoj = new TProfile("pphoj","",nx,vx);
   
     // Extras without zero suppression and for gain paths
     TH2D *h2phoj0 = new TH2D("h2phoj0","",nx,vx,140,-0.2,0.5);
     TProfile *pphoj0 = new TProfile("pphoj0","",nx,vx);
     TH2D *h2phoj1 = new TH2D("h2phoj1","",nx,vx,140,-0.2,0.5);
     TProfile *pphoj1 = new TProfile("pphoj1","",nx,vx);
     TH2D *h2phoj6 = new TH2D("h2phoj6","",nx,vx,140,-0.2,0.5);
     TProfile *pphoj6 = new TProfile("pphoj6","",nx,vx);
     TH2D *h2phoj12 = new TH2D("h2phoj12","",nx,vx,140,-0.2,0.5);
     TProfile *pphoj12 = new TProfile("pphoj12","",nx,vx);
   
     // Plots for photon properties (more in MC)
     TH2D *h2gametaphi = new TH2D("h2gametaphi","",30,-1.305,+1.305,
   			       72,-TMath::Pi(),TMath::Pi());
     //TH2D *h2gametaphi2 = new TH2D("h2gametaphi2","",150,-1.305,+1.305,
     // Match barrel edge to 1.305 with 3.132, even though EC edge should be 3.139
     TH2D *h2gametaphi2 = new TH2D("h2gametaphi2","",360,-3.132,+3.132,
   				360,-TMath::Pi(),TMath::Pi());
     //TH2D *h2gametaphi3 = new TH2D("h2gametaphi3","",150,-1.305,+1.305,
     //				720,-TMath::Pi(),TMath::Pi());
     //TH2D *h2gametaphi4 = new TH2D("h2gametaphi4","",150,-1.305,+1.305,
     //				1440,-TMath::Pi(),TMath::Pi());
     TH2D *h2ngam = new TH2D("h2ngam","",nx,vx,5,0,5);
     TH1D *hgen = new TH1D("hgen","",nx,vx);
     TH1D *hgam = new TH1D("hgam","",nx,vx);
     TH1D *hgamtrg = new TH1D("hgamtrg","",nx,vx);
     TProfile *peffgr = new TProfile("peffgr","",nx,vx);
     TProfile *peffid = new TProfile("peffid","",nx,vx);
     TProfile *pfake = new TProfile("pfake","",nx,vx);
     TProfile *pfakeqcd = new TProfile("pfakeqcd","",nx,vx); // for QCD bkg
     TProfile *pfakeqcd2 = new TProfile("pfakeqcd2","",nx,vx); // for QCD bkg
     TH2D *h2rgam = new TH2D("h2rgam","",nx,vx,350,0.80,1.15);
     TH2D *h2rgamqcd = new TH2D("h2rgamqcd","",nx,vx,350,0.80,1.15); // for QCD bkg
     TProfile *prgam = new TProfile("prgam","",nx,vx);
     TProfile *prgamqcd = new TProfile("prgamqcd","",nx,vx); // for QCD bkg
     TProfile *prgamqcd2 = new TProfile("prgamqcd2","",nx,vx); // for QCD bkg
     TProfile *prgamorigqcd = new TProfile("prgamorigqcd","",nx,vx); // fpr QCD bkg
     TProfile *prgamorigqcd2 = new TProfile("prgamorigqcd2","",nx,vx); // QCD
     TH2D *h2cgam = new TH2D("h2cgam","",nx,vx,100,0.90,1.10);
     TProfile *pcgam = new TProfile("pcgam","",nx,vx);
   
    //--------------------------
    // Photon scale and smearing
    //--------------------------
    TH1D *hPhoScaleSF = new TH1D("hPhoScaleSF", "hPhoScaleSF", 100, 0.8, 1.2);
    TH1D *hPhoScaleSF_Up = new TH1D("hPhoScaleSF_Up", "hPhoScaleSF_Up", 100, 0.8, 1.2);
    TH1D *hPhoScaleSF_Down = new TH1D("hPhoScaleSF_Down", "hPhoScaleSF_Down", 100, 0.8, 1.2);
    TH1D *hPhoSmearSF = new TH1D("hPhoSmearSF", "hPhoSmearSF", 100, 0.8, 1.2);
    TH1D *hPhoSmearSF_Up = new TH1D("hPhoSmearSF_Up", "hPhoSmearSF_Up", 100, 0.8, 1.2);
    TH1D *hPhoSmearSF_Down = new TH1D("hPhoSmearSF_Down", "hPhoSmearSF_Down", 100, 0.8, 1.2);
    //TProfile
    TProfile *tPhoScaleSF = new TProfile("tPhoScaleSF", "tPhoScaleSF", nx, vx);
    // in eta bins
    // Gain = 1
        TProfile *tPhoScaleSF_Gain1 = new TProfile("tPhoScaleSF_Gain1", "tPhoScaleSF_Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin1Gain1= new TProfile("tPhoScaleSF_etaBin1Gain1", "tPhoScaleSF_etaBin1Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin2Gain1= new TProfile("tPhoScaleSF_etaBin2Gain1", "tPhoScaleSF_etaBin2Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin3Gain1= new TProfile("tPhoScaleSF_etaBin3Gain1", "tPhoScaleSF_etaBin3Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin4Gain1= new TProfile("tPhoScaleSF_etaBin4Gain1", "tPhoScaleSF_etaBin4Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin5Gain1= new TProfile("tPhoScaleSF_etaBin5Gain1", "tPhoScaleSF_etaBin5Gain1", nx, vx);
        TProfile *tPhoScaleSF_etaBin6Gain1= new TProfile("tPhoScaleSF_etaBin6Gain1", "tPhoScaleSF_etaBin6Gain1", nx, vx);
    // Gain = 6
        TProfile *tPhoScaleSF_Gain6 = new TProfile("tPhoScaleSF_Gain6", "tPhoScaleSF_Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin1Gain6= new TProfile("tPhoScaleSF_etaBin1Gain6", "tPhoScaleSF_etaBin1Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin2Gain6= new TProfile("tPhoScaleSF_etaBin2Gain6", "tPhoScaleSF_etaBin2Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin3Gain6= new TProfile("tPhoScaleSF_etaBin3Gain6", "tPhoScaleSF_etaBin3Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin4Gain6= new TProfile("tPhoScaleSF_etaBin4Gain6", "tPhoScaleSF_etaBin4Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin5Gain6= new TProfile("tPhoScaleSF_etaBin5Gain6", "tPhoScaleSF_etaBin5Gain6", nx, vx);
        TProfile *tPhoScaleSF_etaBin6Gain6= new TProfile("tPhoScaleSF_etaBin6Gain6", "tPhoScaleSF_etaBin6Gain6", nx, vx);
    // Gain = 12
        TProfile *tPhoScaleSF_Gain12 = new TProfile("tPhoScaleSF_Gain12", "tPhoScaleSF_Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin1Gain12= new TProfile("tPhoScaleSF_etaBin1Gain12", "tPhoScaleSF_etaBin1Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin2Gain12= new TProfile("tPhoScaleSF_etaBin2Gain12", "tPhoScaleSF_etaBin2Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin3Gain12= new TProfile("tPhoScaleSF_etaBin3Gain12", "tPhoScaleSF_etaBin3Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin4Gain12= new TProfile("tPhoScaleSF_etaBin4Gain12", "tPhoScaleSF_etaBin4Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin5Gain12= new TProfile("tPhoScaleSF_etaBin5Gain12", "tPhoScaleSF_etaBin5Gain12", nx, vx);
        TProfile *tPhoScaleSF_etaBin6Gain12= new TProfile("tPhoScaleSF_etaBin6Gain12", "tPhoScaleSF_etaBin6Gain12", nx, vx);
    //----
    TProfile *tPhoScaleSF_Up = new TProfile("tPhoScaleSF_Up", "tPhoScaleSF_Up", nx, vx);
    TProfile *tPhoScaleSF_Down = new TProfile("tPhoScaleSF_Down", "tPhoScaleSF_Down", nx, vx);
    TProfile *tPhoSmearSF = new TProfile("tPhoSmearSF", "tPhoSmearSF", nx, vx);
    // in eta bins
    // Gain = 1
        TProfile *tPhoSmearSF_Gain1 = new TProfile("tPhoSmearSF_Gain1", "tPhoSmearSF_Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin1Gain1= new TProfile("tPhoSmearSF_etaBin1Gain1", "tPhoSmearSF_etaBin1Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin2Gain1= new TProfile("tPhoSmearSF_etaBin2Gain1", "tPhoSmearSF_etaBin2Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin3Gain1= new TProfile("tPhoSmearSF_etaBin3Gain1", "tPhoSmearSF_etaBin3Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin4Gain1= new TProfile("tPhoSmearSF_etaBin4Gain1", "tPhoSmearSF_etaBin4Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin5Gain1= new TProfile("tPhoSmearSF_etaBin5Gain1", "tPhoSmearSF_etaBin5Gain1", nx, vx);
        TProfile *tPhoSmearSF_etaBin6Gain1= new TProfile("tPhoSmearSF_etaBin6Gain1", "tPhoSmearSF_etaBin6Gain1", nx, vx);
    // Gain = 6
        TProfile *tPhoSmearSF_Gain6 = new TProfile("tPhoSmearSF_Gain6", "tPhoSmearSF_Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin1Gain6= new TProfile("tPhoSmearSF_etaBin1Gain6", "tPhoSmearSF_etaBin1Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin2Gain6= new TProfile("tPhoSmearSF_etaBin2Gain6", "tPhoSmearSF_etaBin2Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin3Gain6= new TProfile("tPhoSmearSF_etaBin3Gain6", "tPhoSmearSF_etaBin3Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin4Gain6= new TProfile("tPhoSmearSF_etaBin4Gain6", "tPhoSmearSF_etaBin4Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin5Gain6= new TProfile("tPhoSmearSF_etaBin5Gain6", "tPhoSmearSF_etaBin5Gain6", nx, vx);
        TProfile *tPhoSmearSF_etaBin6Gain6= new TProfile("tPhoSmearSF_etaBin6Gain6", "tPhoSmearSF_etaBin6Gain6", nx, vx);
    // Gain = 12
        TProfile *tPhoSmearSF_Gain12 = new TProfile("tPhoSmearSF_Gain12", "tPhoSmearSF_Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin1Gain12= new TProfile("tPhoSmearSF_etaBin1Gain12", "tPhoSmearSF_etaBin1Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin2Gain12= new TProfile("tPhoSmearSF_etaBin2Gain12", "tPhoSmearSF_etaBin2Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin3Gain12= new TProfile("tPhoSmearSF_etaBin3Gain12", "tPhoSmearSF_etaBin3Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin4Gain12= new TProfile("tPhoSmearSF_etaBin4Gain12", "tPhoSmearSF_etaBin4Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin5Gain12= new TProfile("tPhoSmearSF_etaBin5Gain12", "tPhoSmearSF_etaBin5Gain12", nx, vx);
        TProfile *tPhoSmearSF_etaBin6Gain12= new TProfile("tPhoSmearSF_etaBin6Gain12", "tPhoSmearSF_etaBin6Gain12", nx, vx);
    TProfile *tPhoSmearSF_Up = new TProfile("tPhoSmearSF_Up", "tPhoSmearSF_Up", nx, vx);
    TProfile *tPhoSmearSF_Down = new TProfile("tPhoSmearSF_Down", "tPhoSmearSF_Down", nx, vx);

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
   
     // Plots for photon response in data
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
     
     // Plots for photon trigger efficiencies
     // TBD: need to create these more systematically with a loop
     TH1D *hgam0_data = new TH1D("hgam0_data","",197,15,1000);
     TH1D *hgam0_mc = new TH1D("hgam0_mc","",197,15,1000);
     TH1D *hgam0 = new TH1D("hgam0","",197,15,1000);
     TH1D *hgam20 = new TH1D("hgam20","",197,15,1000);
     TH1D *hgam22 = new TH1D("hgam22","",197,15,1000);
     TH1D *hgam30 = new TH1D("hgam30","",197,15,1000);
     TH1D *hgam33 = new TH1D("hgam33","",197,15,1000);
     TH1D *hgam36 = new TH1D("hgam36","",197,15,1000);
     TH1D *hgam50 = new TH1D("hgam50","",197,15,1000);
     TH1D *hgam75 = new TH1D("hgam75","",197,15,1000);
     TH1D *hgam90 = new TH1D("hgam90","",197,15,1000);
     TH1D *hgam120 = new TH1D("hgam120","",197,15,1000);
     TH1D *hgam150 = new TH1D("hgam150","",197,15,1000);
     TH1D *hgam175 = new TH1D("hgam175","",197,15,1000);
     TH1D *hgam200 = new TH1D("hgam200","",197,15,1000);
     TH1D *hgam300 = new TH1D("hgam300","",197,15,1000);
     TH1D *hgam30t = new TH1D("hgam30t","",197,15,1000);
     TH1D *hgam100t = new TH1D("hgam100t","",197,15,1000);
     TH1D *hgam110t = new TH1D("hgam110t","",197,15,1000);
     TH1D *hgam120t = new TH1D("hgam120t","",197,15,1000);
     TH1D *hgam22m = new TH1D("hgam22m","",197,15,1000);
     TH1D *hgam30m = new TH1D("hgam30m","",197,15,1000);
     TH1D *hgam36m = new TH1D("hgam36m","",197,15,1000);
     TH1D *hgam50m = new TH1D("hgam50m","",197,15,1000);
     TH1D *hgam75m = new TH1D("hgam75m","",197,15,1000);
     TH1D *hgam90m = new TH1D("hgam90m","",197,15,1000);
     TH1D *hgam120m = new TH1D("hgam120m","",197,15,1000);
     TH1D *hgam165m = new TH1D("hgam165m","",197,15,1000);
     TH1D *hgam165h = new TH1D("hgam165h","",197,15,1000);
     TH1D *hgam100h = new TH1D("hgam100h","",197,15,1000);
     TH1D *hgam20l = new TH1D("hgam20l","",197,15,1000);
     TH1D *hgam30l = new TH1D("hgam30l","",197,15,1000);
     TH1D *hgam40l = new TH1D("hgam40l","",197,15,1000);
     TH1D *hgam50l = new TH1D("hgam50l","",197,15,1000);
     TH1D *hgam60l = new TH1D("hgam60l","",197,15,1000);
     TH1D *hgamtrig = new TH1D("hgamtrig","",197,15,1000);
     TH1D *hgamtrig_data = new TH1D("hgamtrig_data","",197,15,1000);
     TH1D *hgamtrig_mc = new TH1D("hgamtrig_mc","",197,15,1000);
   
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
     MoreHistosGamJet *hg(0);
     if (doGamjet) {
   
       // L3Res (gamma+jet) pT binning adapted and extended
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
   
       MoreHistosGamJet *h = new MoreHistosGamJet();
       hg = h;
   
       // Counting of events, and JEC L2L3Res+JERSF for undoing
       h->hpt13 = new TH1D("hpt13",";p_{T,#gamma};N_{events}",npt,vpt);
       h->hpt13a = new TH1D("hpt13a",";p_{T,avg};N_{events}",npt,vpt);
       h->hpt13j = new TH1D("hpt13j",";p_{T,jet};N_{events}",npt,vpt);
       
       h->pptg = new TProfile("pptg",";p_{T,#gamma};p_{T,#gamma}",npt,vpt);
       h->pptj = new TProfile("pptj",";p_{T,#gamma};p_{T,jet}",npt,vpt);
   
      // MPF decomposition for HDM method
       h->pres = new TProfile("pres",";p_{T,#gamma} (GeV);JES",npt,vpt);
       h->pjsf = new TProfile("pjsf",";p_{T,#gamma} (GeV);JERSF",npt,vpt);
       h->pm0 = new TProfile("pm0",";p_{T,#gamma} (GeV);MPF0",npt,vpt);
       h->pm2 = new TProfile("pm2",";p_{T,#gamma} (GeV);MPF2",npt,vpt);
       h->pmn = new TProfile("pmn",";p_{T,#gamma} (GeV);MPFn",npt,vpt);
       h->pmu = new TProfile("pmu",";p_{T,#gamma} (GeV);MPFu",npt,vpt);
       
       h->pm0x = new TProfile("pm0x",";p_{T,#gamma} (GeV);MPF0X (MPFX)", npt, vpt, "S");
       h->pm2x = new TProfile("pm2x",";;p_{T,#gamma} (GeV);MPF2X (DBX)", npt, vpt, "S");
   
       // Extra for FSR studies
       h->pmnu = new TProfile("pmnu",";p_{T,#gamma} (GeV);MPFnu", npt, vpt);
       h->pmnx = new TProfile("pmnx",";p_{T,#gamma} (GeV);MPFNX", npt, vpt, "S");
       h->pmux = new TProfile("pmux",";p_{T,#gamma} (GeV);MPFUX", npt, vpt, "S");
       h->pmnux = new TProfile("pmnux",";p_{T,#gamma} (GeV;MPFNUX", npt, vpt, "S");
       
       // Composition
       h->prho = new TProfile("prho",";p_{T,#gamma};#rho (GeV)",npt,vpt);
       h->pchf = new TProfile("pchf",";p_{T,#gamma};CHF",npt,vpt);
       h->pnhf = new TProfile("pnhf",";p_{T,#gamma};NHF",npt,vpt);
       h->pnef = new TProfile("pnef",";p_{T,#gamma};NEF",npt,vpt);
   
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
     MoreHistosGamJet2 *hg2(0);
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
   
       MoreHistosGamJet2 *h = new MoreHistosGamJet2();
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
     unsigned int nps = (isMC ? tree->nPSWeightMax+1 : 1);
     map<int, map<int, map<int, BasicHistosGamJet*> > > mBasicHistosGamJet;
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
         BasicHistosGamJet *pmh = new BasicHistosGamJet();
         BasicHistosGamJet& mh = (*pmh);
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
         mBasicHistosGamJet[iy][ia][ips] = pmh;
       } // for ips in PSWeight
       } // for ieta in etas
     } // for ialpha in alphas
     
     curdir->cd();
     
     TLorentzVector gam, gami, lhe, gengam, phoj, phoj0, phoj0off, jet, jet2, jetn;
     TLorentzVector gamorig; // for QCD bkg
     TLorentzVector met, met1, metn, metu, metnu, rawmet, corrmet, rawgam;
     TLorentzVector jeti, corrjets, rawjet, rawjets, rcjet, rcjets, rcoffsets;
     TLorentzVector geni, genjet, genjet2;
     TLorentzVector fox; // for isQCD
     TLorentzVector gamx; // for MPFX
     
     //Long64_t nentries = tree->GetEntriesFast();
     Long64_t nentries = tree->GetEntries(); // Long startup time
     cout << "\nStarting loop over " << oName << " with "
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
       cout << "}; // " << oName << endl << flush;
       //
       cout << Form("double vsumw[%d] = ",hsumw->GetNbinsX());
       for (int i = 1; i != hsumw->GetNbinsX()+1; ++i) {
         cout<<Form("%s%1.4g",(i==1 ? "{" : ", "),hsumw->GetBinContent(i));
       }
       cout << "}; // " << oName << endl << flush;
     } // isMC && nentries!=nMG
     
     //int skip = 21700000; // 2018A first events without 110EB
     //int skip = 55342793; // 2018A first events with 92 photon
     //int skip = 265126992; // 2018A first events with 191 photons, 23 jets
     //int skip = 14648973; // 2017C bad HDM
   
     for (Long64_t jentry=0; jentry<nentries;jentry++) {
   
       // Skip events, typically for debugging purposes
       //if (jentry<skip) continue;
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
       Long64_t ientry = tree->loadEntry(jentry);
       if (ientry < 0) break; 
       tree->fChain->GetTree()->GetEntry(ientry);
   	   if (!(tree->HLT_Photon200 ||
   	     tree->HLT_Photon175 || 
   	     tree->HLT_Photon150 || 
   	     tree->HLT_Photon120 || 
   	     tree->HLT_Photon90 || 
   	     tree->HLT_Photon75 || 
   	     tree->HLT_Photon50 || 
   	     tree->HLT_Photon33 || 
   	     //tree->HLT_Photon20 ||
   	     //tree->HLT_Photon120EB_TightID_TightIso ||
   	     tree->HLT_Photon110EB_TightID_TightIso ||
   	     //tree->HLT_Photon100EB_TightID_TightIso ||
   	     tree->HLT_Photon100EBHE10 ||
   	     tree->HLT_Photon30EB_TightID_TightIso ||
   	     tree->HLT_Photon90_R9Id90_HE10_IsoM ||
   	     tree->HLT_Photon75_R9Id90_HE10_IsoM ||
   	     tree->HLT_Photon50_R9Id90_HE10_IsoM ||
   	     tree->HLT_Photon30_HoverELoose ||
   	     tree->HLT_Photon20_HoverELoose)){
   	    ++_nbadevents_trigger;
   	    continue;
        }
   
       // if (Cut(ientry) < 0) continue;
   
       // Safety check for rho being NaN
       if (!(tree->fixedGridRhoFastjetAll>=0 && tree->fixedGridRhoFastjetAll<150))
         tree->fixedGridRhoFastjetAll = 34; // average conditions
       
       // Sanity check PS weights
       if (!isMC) { tree->nPSWeight = 0; }
       assert(tree->nPSWeight<=tree->nPSWeightMax);
   
       // Does the run/LS pass the latest JSON selection?
       if (isData && objS->loadedLumiJson[tree->run][tree->luminosityBlock]==0) {
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
       
       // Select leading photon. Use tight cut-based ID and PF relative isolation
       // Temporary: select photon based on LHE photon match
       int iGamGen(-1), iGam(-1), nGam(0);
       int iGamOrig(-1); // for QCD bkg
       gengam.SetPtEtaPhiM(0,0,0,0);
       gam.SetPtEtaPhiM(0,0,0,0);
       rawgam.SetPtEtaPhiM(0,0,0,0);
       phoj.SetPtEtaPhiM(0,0,0,0);
       phoj0.SetPtEtaPhiM(0,0,0,0);
   
       // Gen-photon
       if (isMC && tree->nGenIsolatedPhoton>0) {
         gengam.SetPtEtaPhiM(tree->GenIsolatedPhoton_pt[0],tree->GenIsolatedPhoton_eta[0],
   			  tree->GenIsolatedPhoton_phi[0],tree->GenIsolatedPhoton_mass[0]);
       }
        //https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammSFandSSRun3#Scale_And_Smearings_Correctionli 
       // Select tight photons and photon matching gen photon
       for (int i = 0; i != tree->nPhoton; ++i) {
   
         tree->Photon_mass[i] = 0;
         gami.SetPtEtaPhiM(tree->Photon_pt[i],  tree->Photon_eta[i],
   			tree->Photon_phi[i], tree->Photon_mass[i]);
         
         // Photon matching gen photon
         if (gengam.Pt()>0 && gengam.DeltaR(gami)<0.2 && iGamGen==-1) {
   	iGamGen = i;
         } 
         
         // Leading tight photon(s)
         // R9>0.94 to avoid bias wrt R9Id90 triggers and from photon conversions
        if (tree->Photon_pt[i]>15 && tree->Photon_cutBased[i]==3 && tree->Photon_hoe[i]<0.02148 && tree->Photon_r9[i]>0.96) {
            ++nGam;
   	        if (iGam==-1) {
   	            iGam = i;
   	            gam = gami;
   	        }
         } // tight photon
       } // for i in tree->nPhoton
   
       // Correct photon for gain1 and MPF for "footprint" (photon vs PFgamma)
       rawgam = gam;
       if (iGam!=-1 && tree->Photon_seedGain[iGam]==1 && !isMC) {
         //gam *= 1./1.01;
         // minitools/drawGainVsPt.C (add R_6/12+R_1/6, take MPF+statTowardsDB)
         gam *= 1./1.017; // MPF=1.74+/-0.07%, DB=1.41+/-0.16%
       }
      
       // Photon-jet: uncorrected jet minus (uncorr.) photon minus L1RC
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
         phoj.SetPtEtaPhiM(tree->Jet_pt[idx], tree->Jet_eta[idx], tree->Jet_phi[idx], tree->Jet_mass[idx]);
         phoj *= (1-tree->Jet_rawFactor[idx]);
         if (rawgam.DeltaR(phoj)<0.4) { // does not always hold in Run3
   	//phoj.Pt() >= rawgam.Pt()) { // not always true in Run3 (esp. 2022C)
   	// 2022 data is missing proper Puppi photon protection for jets
   	// (but MET ok?)
   	//double r22 = max(0.15,min(1.0,(rawgam.Pt()-20.)/180.));
   	phoj -= rawgam; // NanoV12
         }
         else {
   	if (cntErrDR<10) {
   	  cout << endl << "entry " << jentry << ", rawgam.DeltaR(phoj) = "
   	       << rawgam.DeltaR(phoj) << endl << flush;
   	  cout << "Skip event " << tree->event << " in LS " << tree->luminosityBlock
   	       << " in  run " << tree->run << " in File: " << _filename << endl;
   	  ++cntErrDR;
   	  if (cntErrDR==10) {
   	    cout << "Stop reporting rawgam.DeltaR, silently skip." << endl;
   	  }
   	}
   	phoj.SetPtEtaPhiM(0,0,0,0);
   	continue;
         }
         phoj0 = phoj;
   
         // Calculate L1RC correction
         /*
         double corrl1rc(1.); 
         phoj *= corrl1rc;
   
         // Calculate L1RC correction without "zero suppression"
         double refpt = 30; // phoj.Pt~0 leads to negative offset cutoff
         double corrl1rc0(1.); 
         double off0 = (corrl1rc0 - 1) * refpt; // corr*ptref = (ptref-off)
         phoj0off.SetPtEtaPhiM(off0,phoj0.Eta(),phoj0.Phi(),0.);
         phoj0 -= phoj0off;
       }
       */
   
       // For QCD background, emulate a photon+jet event by replacing
       // one of the leading jets with genjet taking the place of the photon
       int iFox(-1);
       fox.SetPtEtaPhiM(0,0,0,0);
       //if (isQCD && iGam==-1 && tree->nJet>=2) {
       if (isQCD && tree->nJet>=2) {
         // Save original good photon, if one was found
         iGamOrig = iGam;
         gamorig = gam;
   
         iFox = (jentry%2); // "random" selection from two leading jets
         // tree->Jet_genJetIdx would be great, but only there for UL18 nAOD? Maybe there
         int k = tree->Jet_genJetIdx[iFox];
         if (k>=0 && k<tree->nGenJet) {
   	gam.SetPtEtaPhiM(tree->GenJet_pt[k], tree->GenJet_eta[k], tree->GenJet_phi[k],
   			 tree->GenJet_mass[k]);
   	// NB: should remove UE clustered into gam. In Minsuk's rho_ZB_new.pdf
   	// QCD_CP5 has about 3.5 GeV/A of UE offset at generator level
   	double area = tree->Jet_area[iFox];
   	gam *= (gam.Pt()>0 ? 1 - 3.5*area/gam.Pt() : 1.);
   	rawgam = gam;
   
   	fox.SetPtEtaPhiM(tree->Jet_pt[iFox], tree->Jet_eta[iFox], tree->Jet_phi[iFox],
   			 tree->Jet_mass[iFox]);
   	fox *= (1-tree->Jet_rawFactor[iFox]);
   	// Calculate L1RC correction
   	double corrl1rc(1.); // 
   	fox *= corrl1rc;
   	// NB2: should also remove UE clustered into fox. In Minsuk's plot
   	// QCD_CP5 has about 2.5 GeV/A of UE offset at FullSim level
   	fox *= (fox.Pt()>0 ? 1 - 2.5*area/fox.Pt() : 1.);
   
   	// NB3: For consistency with gamma+jet, phoj should still have reco UE
   	phoj.SetPtEtaPhiM(2.5*area,0,0,0);
   	phoj0 = phoj;
   	nGam = 1;
         }
       } // isQCD
     
       // Event weights (1 for MadGraph)
       //bool isMC = (run==1);
       assert((isMC && tree->run==1) || (!isMC && tree->run!=1));
       double w = (isMC ? tree->genWeight : 1);
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
       //if (doPtHatFilter && isMC) {
       //if ( isMG && 2.*tree->Pileup_pthatmax>LHE_HT) continue;
       //if (!isMG && tree->Pileup_pthatmax>Generator_binvar) continue;
       //}
       
       // Pileup
       double TruePUrms(0);
       if (!isMC) tree->Pileup_nTrueInt = objS->getTruePU(tree->run,tree->luminosityBlock,&TruePUrms);
       double ptgam = gam.Pt();
   
       // Trigger selection. Take care to match pT bin edges
       // {15, 20, 25, 30, 35, 40, 50, 60, 70, 85, 105, 130, 175, 230,
       //  300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750};
       // NB: Photon90 threshold could be 95, Photon175 coud be 185, if bins split?
       double pt = ptgam; // shorthand to make trigger selection more readable
       int itrg(0); // choose trigger for PU reweighing as side effect (hack...)
       bool pass_trig = ((tree->HLT_Photon200                 && pt>=230            && (itrg=200)) ||
   	 (tree->HLT_Photon110EB_TightID_TightIso && pt>=110&&pt<230 && (itrg=110)) ||
   	 (tree->HLT_Photon90_R9Id90_HE10_IsoM && pt>=90  && pt<110  && (itrg=90))  ||
   	 (tree->HLT_Photon75_R9Id90_HE10_IsoM && pt>=75  && pt<90   && (itrg=75))  ||
   	 (tree->HLT_Photon50_R9Id90_HE10_IsoM && pt>=50  && pt<75   && (itrg=50))  ||
          //(tree->HLT_Photon50_R9Id90_HE10_IsoM && pt>=50  && pt<130  && (itrg=50))  ||
   	 (tree->HLT_Photon30EB_TightID_TightIso  && pt>=30&&pt<50   && (itrg=30))  ||
   	 (tree->HLT_Photon20_HoverELoose      && pt>=20  && pt<30   && (itrg=20))
   	 //|| (true && (itrg=1))// trigger bypass for EGamma on photonTrigs.C
   	 );
     
       // Select trigger pT bins by hand for QCD. Error prone...
       if (isQCD && !pass_trig) {
         pass_trig = ((pt>=230            && (itrg=200)) ||
   	   (pt>=110&&pt<230 && (itrg=110)) ||
   	   (pt>=90  && pt<110  && (itrg=90))  ||
   	   (pt>=75  && pt<90   && (itrg=75))  ||
   	   (pt>=50  && pt<75   && (itrg=50))  ||
   	   (pt>=30&&pt<50   && (itrg=30))  ||
   	   (pt>=20  && pt<30   && (itrg=20)));
       } // isQCD
   
       assert(itrg>0 || !pass_trig);
   
       // Select leading jets. Just exclude photon, don't apply JetID yet
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
         
         // Check that jet is not photon and pTcorr>15 GeV
         if (tree->Jet_pt[i]>15 && (iGam==-1 || i != tree->Photon_jetIdx[iGam]) &&
   	  (!isQCD || i != iFox)) {
   	
   	//++nJets;
   	jeti.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
    	if (gam.DeltaR(jeti)<0.2) continue; // should not happen, but does?
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
   	double corrl1rc(1.); // 
   	rcjet = corrl1rc * rawjet;
   	
   	// Corrected type-I chsMET calculation
   	corrjets += jeti;
   	rawjets += rawjet;
   	rcjets += rcjet;
   	rcoffsets += (rawjet - rcjet);
         } // non-photon jet
       } // for i in nJet
       
       // Select genjet matching leading and subleading reco jet
       int iGenJet(-1), iGenJet2(-1);
       genjet.SetPtEtaPhiM(0,0,0,0);
       genjet2.SetPtEtaPhiM(0,0,0,0);
       if (isMC) {
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
       } // isMC
   
       // Set MET vectors
       rawmet.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
       if (isQCD && iFox!=-1) rawmet += fox - gam; // fox=rawjet-PU, gam=genjet
       else rawmet += rawgam - gam; // replace PF photon with Reco photon
       met1 = -jet -gam;
       metn = -jetn;
       //corrmet = rawmet +rawjets -corrjets -rcoffsets;
       corrmet = rawmet +rcjets -corrjets; // same as above
       // Unclustered MET from rawMET by taking out all the hard stuff
       // metu = rawmet +gam +rawjets -rcoffsets;
       // metu = rawmet +gam +rcjets;
       // Or equally well, from corrMET (modulo rounding errors)
       metu = corrmet +gam +corrjets;
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
       if (ptgam>0) {
         bal = ptjet / ptgam;
         mpf = 1 + met.Vect().Dot(gam.Vect()) / (ptgam*ptgam);
         mpf1 = 1 + met1.Vect().Dot(gam.Vect()) / (ptgam*ptgam);
         mpfn = metn.Vect().Dot(gam.Vect()) / (ptgam*ptgam);
         mpfu = metu.Vect().Dot(gam.Vect()) / (ptgam*ptgam);
         mpfnu = metnu.Vect().Dot(gam.Vect()) / (ptgam*ptgam);
         //
         gamx.SetPtEtaPhiM(gam.Pt(),gam.Eta(),gam.Phi()+0.5*TMath::Pi(),0.);
         mpfx = 1 + met.Vect().Dot(gamx.Vect()) / (ptgam*ptgam);
         mpf1x = 1 + met1.Vect().Dot(gamx.Vect()) / (ptgam*ptgam);
         mpfnx = metn.Vect().Dot(gamx.Vect()) / (ptgam*ptgam);
         mpfux = metu.Vect().Dot(gamx.Vect()) / (ptgam*ptgam);
         mpfnux = metnu.Vect().Dot(gamx.Vect()) / (ptgam*ptgam);
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
       bool pass_filt = eventPick->passFilter(tree); 
       
       // Photon control plots
       h2ngam->Fill(ptgam, nGam, w);
       if (gengam.Pt()>0 && fabs(gengam.Eta()) < 1.3) {
         hgen->Fill(gengam.Pt(), w);
         peffgr->Fill(gengam.Pt(), iGamGen!=-1 ? 1 : 0, w);
         peffid->Fill(gengam.Pt(), iGam==iGamGen ? 1 : 0, w);
       }
       if (ptgam>0 && fabs(gam.Eta()) < 1.3 && pass_filt) {
         hgam->Fill(ptgam, w);
         if (isMC) pfake->Fill(ptgam, iGam!=iGamGen ? 1 : 0, w);
         if (isQCD) {
   	bool hasorig = (iGamOrig!=-1 && gam.DeltaR(gamorig)<0.2);
   	bool inwindow = (fabs(gamorig.Pt() / ptgam - 0.9) < 0.2); // [0.8,1.1]
   	pfakeqcd->Fill(ptgam, hasorig ? 1 : 0, w);
   	pfakeqcd2->Fill(ptgam, hasorig && inwindow ? 1 : 0, w);
   	if (hasorig) {
   	  h2rgamqcd->Fill(ptgam, gamorig.Pt() / ptgam, w);
   	  prgamqcd->Fill(ptgam, gamorig.Pt() / ptgam, w);
   	  if (inwindow) prgamqcd2->Fill(ptgam, gamorig.Pt() / ptgam, w);
   	  prgamorigqcd->Fill(gamorig.Pt(), ptgam / gamorig.Pt(), w);
   	  if (inwindow) prgamorigqcd2->Fill(gamorig.Pt(), ptgam / gamorig.Pt(), w);
   	} // hasorig
         }
         if (iGam==iGamGen && gengam.Pt()>0) {
   	h2rgam->Fill(gengam.Pt(), ptgam / gengam.Pt(), w);
   	prgam->Fill(gengam.Pt(), ptgam / gengam.Pt(), w);
   	h2cgam->Fill(ptgam, gengam.Pt() / ptgam, w);
   	pcgam->Fill(ptgam, gengam.Pt() / ptgam, w);
         }
   
         // Plots for photon trigger efficiencies
         if (isMC)  hgam0_mc->Fill(ptgam, w);
         if (!isMC) hgam0_data->Fill(ptgam, w);
         
         hgam0 ->Fill(ptgam, w);
         // Backup high pT
         if (tree->HLT_Photon300_NoHE)  hgam300->Fill(ptgam, w);
         if (tree->HLT_Photon165_HE10)  hgam165h->Fill(ptgam, w);
         if (tree->HLT_Photon100EBHE10) hgam100h->Fill(ptgam, w);
         // Main unprescaled trigger in 2018
         if (tree->HLT_Photon200) hgam200->Fill(ptgam, w);
         // Run 1 style prescaled triggers
         if (tree->HLT_Photon175) hgam175->Fill(ptgam, w);
         if (tree->HLT_Photon150) hgam150->Fill(ptgam, w);
         if (tree->HLT_Photon120) hgam120->Fill(ptgam, w);
         if (tree->HLT_Photon90)  hgam90 ->Fill(ptgam, w);
         if (tree->HLT_Photon75)  hgam75 ->Fill(ptgam, w);
         if (tree->HLT_Photon50)  hgam50 ->Fill(ptgam, w);
         if (tree->HLT_Photon36)  hgam36 ->Fill(ptgam, w);
         if (tree->HLT_Photon33)  hgam33 ->Fill(ptgam, w);
         if (tree->HLT_Photon30)  hgam30 ->Fill(ptgam, w);
         if (tree->HLT_Photon22)  hgam22 ->Fill(ptgam, w);
         if (tree->HLT_Photon20)  hgam20 ->Fill(ptgam, w);
         // 105-230 GeV intermediate range with tight triggers
         if (tree->HLT_Photon120EB_TightID_TightIso) hgam120t->Fill(ptgam, w);
         if (tree->HLT_Photon110EB_TightID_TightIso) hgam110t->Fill(ptgam, w);
         if (tree->HLT_Photon100EB_TightID_TightIso) hgam100t->Fill(ptgam, w);
         if (tree->HLT_Photon30EB_TightID_TightIso) hgam30t->Fill(ptgam, w);
         // 60-105 GeV with medium triggers. NB: conflicting ID?
         if (tree->HLT_Photon165_R9Id90_HE10_IsoM) hgam165m->Fill(ptgam, w);
         if (tree->HLT_Photon120_R9Id90_HE10_IsoM) hgam120m->Fill(ptgam, w);
         if (tree->HLT_Photon90_R9Id90_HE10_IsoM)  hgam90m ->Fill(ptgam, w);
         if (tree->HLT_Photon75_R9Id90_HE10_IsoM)  hgam75m ->Fill(ptgam, w);
         if (tree->HLT_Photon50_R9Id90_HE10_IsoM)  hgam50m ->Fill(ptgam, w);
         if (tree->HLT_Photon36_R9Id90_HE10_IsoM)  hgam36m ->Fill(ptgam, w);
         if (tree->HLT_Photon30_R9Id90_HE10_IsoM)  hgam30m ->Fill(ptgam, w);
         if (tree->HLT_Photon22_R9Id90_HE10_IsoM)  hgam22m ->Fill(ptgam, w);
         // 20-60 GeV with loose triggers
         if (tree->HLT_Photon60_HoverELoose) hgam60l->Fill(ptgam, w);
         if (tree->HLT_Photon50_HoverELoose) hgam50l->Fill(ptgam, w);
         if (tree->HLT_Photon40_HoverELoose) hgam40l->Fill(ptgam, w);
         if (tree->HLT_Photon30_HoverELoose) hgam30l->Fill(ptgam, w);
         if (tree->HLT_Photon20_HoverELoose) hgam20l->Fill(ptgam, w);
       } // barrel photon
   
         // Summary of combined trigger efficiencies
         if (ptgam>0 && fabs(gam.Eta())<1.3 && pass_trig && pass_filt) {
   	if (isMC)  hgamtrig_mc->Fill(ptgam, w);
   	if (!isMC) hgamtrig_data->Fill(ptgam, w);
   	hgamtrig->Fill(ptgam, w); // 5 GeV bins to match hgam[trgX]
   	hgamtrg->Fill(ptgam, w); // wider binning to higher pT (=hgam)
         }
         //if (ptgam>=105 && fabs(gam.Eta())<1.3 && pass_trig) {
         if (ptgam>=110 && pass_trig && pass_filt) {
   	//HLT_Photon110EB_TightID_TightIso) {
   	h2gametaphi->Fill(gam.Eta(), gam.Phi(), w);
   	h2gametaphi2->Fill(gam.Eta(), gam.Phi(), w);
   	//h2gametaphi3->Fill(gam.Eta(), gam.Phi(), w);
   	//h2gametaphi4->Fill(gam.Eta(), gam.Phi(), w);
         }
         
         bool pass_ngam = (nGam>=1);
         bool pass_njet = (nJets>=1);
         bool pass_gameta = (fabs(gam.Eta()) < 1.3);
         bool pass_dphi = (fabs(gam.DeltaPhi(jet)) > 2.7); // pi-0.44 as in KIT Z+j
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
       
         bool pass_leak = (phoj.Pt()<0.06*ptgam);// 
         bool pass_basic = (pass_trig && pass_filt && pass_ngam && pass_njet &&
   			 pass_gameta && pass_dphi && pass_jetid && pass_veto &&
   			 pass_leak); // add pass_gameta v19 / 202111122 !
         
         // Control plots for jet response
         bool pass_bal = (fabs(1-bal)<0.7);
         bool pass_mpf = (fabs(1-mpf)<0.7);
         bool pass_jeteta = (abseta < 1.3);
         bool pass_alpha100 = (pt2 < ptgam || pt2 < pt2min);      
         bool pass_basic_ext = (pass_basic && pass_bal && pass_mpf);
         bool pass_all = (pass_basic_ext && pass_jeteta && pass_alpha100);
         bool pass_gen = (iGenJet!=-1);
   
         //const int nc = 18;
         bool cut[nc] = {pass_trig, pass_ngam, pass_njet, pass_gameta,
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
         if (pass_trig && pass_ngam && pass_njet && pass_gameta) {
   	hdphi->Fill(gam.DeltaPhi(jet), w);
   	hdr->Fill(gam.DeltaR(jet), w);
         }
   
         // Time controls for JES and PF composition
         if (pass_all) {
   	if (itrg==30 && ptgam>30) {
   	  pr30n->Fill(tree->run, w); 
   	  pr30b->Fill(tree->run, bal, w); 
   	  pr30m->Fill(tree->run, mpf, w);
   	  pr30chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  pr30nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  pr30nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
   	if (itrg==110 && ptgam>110) {
   	  pr110n->Fill(tree->run, w);
   	  pr110b->Fill(tree->run, bal, w); 
   	  pr110m->Fill(tree->run, mpf, w);
   	  pr110chf->Fill(tree->run, tree->Jet_chHEF[iJet], w);
   	  pr110nhf->Fill(tree->run, tree->Jet_neHEF[iJet], w);
   	  pr110nef->Fill(tree->run, tree->Jet_neEmEF[iJet], w);
   	}
   	if (itrg==200 && ptgam>230) {
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
   	cout << Form("pass_ngam = %d, pass_njet = %d, pass_gameta = %d "
   		     "pass_dphi = %d, pass_jetid = %d\n"
   		     "pass_veto = %d, pass_leak = %d, pass_basic = %d "
   		     "pass_bal = %d, pass_mpf = %d, \n"
   		     "pass_jeteta = %d, pass_alpha100 = %d, "
   		     "pass_basic_ext = %d, "
   		     "pass_gen = %d,\n"
   		     "pass_trig = %d, pass_filt = %d",
   		     pass_ngam, pass_njet, pass_gameta,
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
   	h2bal->Fill(ptgam, bal, w);
   	h2mpf->Fill(ptgam, mpf, w);
   	if (pass_mpf) h2balc->Fill(ptgam, bal, w);
   	if (pass_bal) h2mpfc->Fill(ptgam, mpf, w);
   	if (pass_mpf && pass_bal) h2balc2->Fill(ptgam, bal, w);
   	if (pass_mpf && pass_bal) h2mpfc2->Fill(ptgam, mpf, w);
   	if (pass_basic_ext) {
   
   	  if (pass_gen || !isMC) {
   	    h2rjet->Fill(ptgam, jet.Pt() / ptgam, w);
   	    prjet->Fill(ptgam, jet.Pt() / ptgam, w);
   	  }
   	  if (pass_gen) {
   	    h2gjet->Fill(ptgam, genjet.Pt() / ptgam, w);
   	    pgjet->Fill(ptgam, genjet.Pt() / ptgam, w);
   	    h2rgen->Fill(genjet.Pt(), jet.Pt() / genjet.Pt(), w);
   	    prgen->Fill(genjet.Pt(), jet.Pt() / genjet.Pt(), w);
   	  }
   
   	  int flv = (isMC ? tree->GenJet_partonFlavour[iGenJet] : 99);
   	  mvar["counts"] = 1;
   	  mvar["mpfchs1"] = mpf;
   	  mvar["ptchs"] = bal;
   	  mvar["mpf1"] = mpf1;
   	  mvar["mpfn"] = mpfn;
   	  mvar["mpfu"] = mpfu;
   	  mvar["rho"] = tree->fixedGridRhoFastjetAll;
   	  mvar["rjet"] = (ptgam!=0 ? jet.Pt() / ptgam : 0);
   	  mvar["gjet"] = (ptgam!=0 ? genjet.Pt() / ptgam : 0);
   	  mvar["rgen"] = (genjet.Pt()!=0 ? jet.Pt() / genjet.Pt() : 0);
   
   	  tree->Jet_btagDeepB[iJet] = tree->Jet_btagDeepFlavB[iJet];
   	  tree->Jet_btagDeepC[iJet] = 0.5*(tree->Jet_btagDeepFlavCvB[iJet] +
   	  		       tree->Jet_btagDeepFlavCvL[iJet]);
   	  tree->Jet_qgl[iJet] = tree->Jet_btagDeepFlavQG[iJet];
   	  bool isb = (tree->Jet_btagDeepB[iJet] > objS->bThresh);
   	  bool isc = (tree->Jet_btagDeepC[iJet] > objS->cThresh && !isb);
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
   
   		  double x = ptgam;
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
   	  double ggam = genjet.Pt() / gam.Pt();
   	  if (abs(flv)==1) prgenud->Fill(genpt, rgen, w);
   	  if (abs(flv)==2) prgenud->Fill(genpt, rgen, w);
   	  if (abs(flv)==3) prgens->Fill(genpt, rgen, w);
   	  if (abs(flv)==4) prgenc->Fill(genpt, rgen, w);
   	  if (abs(flv)==5) prgenb->Fill(genpt, rgen, w);
   	  if (flv==21)     prgeng->Fill(genpt, rgen, w);
   	  if (flv==0)      prgeno->Fill(genpt, rgen, w);
   	  if (abs(flv)==1) pgjetud->Fill(ptgam, ggam, w);
   	  if (abs(flv)==2) pgjetud->Fill(ptgam, ggam, w);
   	  if (abs(flv)==3) pgjets->Fill(ptgam, ggam, w);
   	  if (abs(flv)==4) pgjetc->Fill(ptgam, ggam, w);
   	  if (abs(flv)==5) pgjetb->Fill(ptgam, ggam, w);
   	  if (flv==21)     pgjetg->Fill(ptgam, ggam, w);
   	  if (flv==0)      pgjeto->Fill(ptgam, ggam, w);
   	  pfud->Fill(ptgam, abs(flv)==1||abs(flv)==2 ? 1 : 0, w);
   	  pfs->Fill(ptgam, abs(flv)==3 ? 1 : 0, w);
   	  pfc->Fill(ptgam, abs(flv)==4 ? 1 : 0, w);
   	  pfb->Fill(ptgam, abs(flv)==5 ? 1 : 0, w);
   	  pfg->Fill(ptgam, flv==21 ? 1 : 0, w);
   	  pfo->Fill(ptgam, flv==0 ? 1 : 0, w);
             */
   	}
   	if (pass_basic_ext && jet2.Pt()>0) {
   	  if (iGenJet2!=-1 || !isMC) {
   	    h2rjet2->Fill(ptgam, jet2.Pt() / ptgam, w);
   	    prjet2->Fill(ptgam, jet2.Pt() / ptgam, w);
   	  }
   	  if (iGenJet2!=-1) {
   	    h2gjet2->Fill(ptgam, genjet2.Pt() / ptgam, w);
   	    pgjet2->Fill(ptgam, genjet2.Pt() / ptgam, w);
   	    h2rgen2->Fill(genjet2.Pt(), jet2.Pt() / genjet2.Pt(), w);
   	    prgen2->Fill(genjet2.Pt(), jet2.Pt() / genjet2.Pt(), w);
   	  }
   	}
         }
   
         // Control plots for trigger 
         if (ptgam>0 && pass_basic_ext && pass_jeteta && pass_alpha100) {
   	//     optimal trigger edges: (20,30,(35),55,80,95,105,115,210)
   	//     old bin trigger edges  (20,30,60,85,*95*,105,130,230)
   	double pt = ptgam;
   	double mu = tree->Pileup_nTrueInt;
   	if (isMC                             && pt>210)  hmusmc->Fill(mu, w);
   	int nmax = (isMC ? 1 : 100);
   	for (int i=0; i!=nmax; ++i) {
   	  mu = gRandom->Gaus(tree->Pileup_nTrueInt,TruePUrms);
   	  double w1 = 0.01*w;
   	  if (tree->HLT_Photon20_HoverELoose         && pt>=20)  hmus20->Fill(mu,w1);
   	  if (tree->HLT_Photon30_HoverELoose         && pt>=30)  hmus30->Fill(mu,w1);
   	  if (tree->HLT_Photon50_R9Id90_HE10_IsoM    && pt>=55)  hmus50->Fill(mu,w1);
   	  if (tree->HLT_Photon75_R9Id90_HE10_IsoM    && pt>=80)  hmus75->Fill(mu,w1);
   	  if (tree->HLT_Photon90_R9Id90_HE10_IsoM    && pt>=95)  hmus90->Fill(mu,w1);
   	  if (tree->HLT_Photon100EB_TightID_TightIso && pt>=105) hmus100->Fill(mu,w1);
   	  if (tree->HLT_Photon110EB_TightID_TightIso && pt>=115) hmus110->Fill(mu,w1);
   	  if (tree->HLT_Photon200                    && pt>=210) hmus200->Fill(mu,w1);
   	} // for i in nmax
         } // control plots for triggers
   
         // Control plots for photon-jet 
         if (ptgam>0 && pass_basic_ext) {
   
   	h2phoj->Fill(ptgam, phoj.Pt()/ptgam, w);
   	pphoj->Fill(ptgam, phoj.Pt()/ptgam, w);
   
   	double footprint =
   	  (fabs(gam.DeltaPhi(phoj0))<0.4 ? 1 : -1)*phoj0.Pt()/ptgam;
   	h2phoj0->Fill(ptgam, footprint, w);
   	pphoj0->Fill(ptgam, footprint, w);
   
    double phoScaleSF        = 1.0;
    double phoScaleSF_Up     = 1.0;
    double phoScaleSF_Down   = 1.0;
    double phoSmearSF        = 1.0;
    double phoSmearSF_Up     = 1.0;
    double phoSmearSF_Down   = 1.0;
   	if (iGam!=-1){ 
        if (isData){
            phoScaleSF  = objS->loadedPhoSsRef->evaluate({"total_correction", 
                        tree->Photon_seedGain[iGam], 
                        static_cast<Float_t>(tree->run), 
                        tree->Photon_eta[iGam], 
                        tree->Photon_r9[iGam],
                        tree->Photon_pt[iGam]});
        }
        if (isMC){
            /*
            phoScaleSF  = objS->loadedPhoSsRef->evaluate({"total_uncertainty",
                        tree->Photon_seedGain[iGam], 
                        static_cast<Float_t>(tree->run), 
                        tree->Photon_eta[iGam], 
                        tree->Photon_r9[iGam],
                        tree->Photon_pt[iGam]});
            phoScaleSF_Up     = (1+phoScaleSF);
            phoScaleSF_Down   = (1-phoScaleSF);
            */
            double rho  = objS->loadedPhoSsRef->evaluate({"rho", 
                        tree->Photon_eta[iGam], 
                        tree->Photon_r9[iGam]});
            /*
            double err_rho  = objS->loadedPhoSsRef->evaluate({"err_rho", 
                        tree->Photon_eta[iGam], 
                        tree->Photon_r9[iGam]});
            */
            phoSmearSF       = gRandom->Gaus(1., rho);
            //phoSmearSF_Up    = gRandom->Gaus(1., rho+err_rho);
            //phoSmearSF_Down  = gRandom->Gaus(1., rho-err_rho);
        }
        hPhoScaleSF        ->Fill(phoScaleSF     );
        hPhoScaleSF_Up     ->Fill(phoScaleSF_Up  );
        hPhoScaleSF_Down   ->Fill(phoScaleSF_Down);
        hPhoSmearSF        ->Fill(phoSmearSF     );
        hPhoSmearSF_Up     ->Fill(phoSmearSF_Up  );
        hPhoSmearSF_Down   ->Fill(phoSmearSF_Down);
        
        tPhoScaleSF        ->Fill(tree->Photon_pt[iGam], phoScaleSF     );
        if(tree->Photon_seedGain[iGam]==1)
            tPhoScaleSF_Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF     );
        if(tree->Photon_seedGain[iGam]==6)
            tPhoScaleSF_Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF     );
        if(tree->Photon_seedGain[iGam]==12)
            tPhoScaleSF_Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF     );
             
        //in eta bins
        //Gain =1
        if(tree->Photon_seedGain[iGam]==1 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoScaleSF_etaBin1Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==1 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoScaleSF_etaBin2Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==1 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoScaleSF_etaBin3Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==1 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoScaleSF_etaBin4Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==1 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoScaleSF_etaBin5Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==1 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoScaleSF_etaBin6Gain1        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        //Gain =6
        if(tree->Photon_seedGain[iGam]==6 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoScaleSF_etaBin1Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==6 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoScaleSF_etaBin2Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==6 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoScaleSF_etaBin3Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==6 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoScaleSF_etaBin4Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==6 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoScaleSF_etaBin5Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==6 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoScaleSF_etaBin6Gain6        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        //Gain =12
        if(tree->Photon_seedGain[iGam]==12 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoScaleSF_etaBin1Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==12 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoScaleSF_etaBin2Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==12 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoScaleSF_etaBin3Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==12 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoScaleSF_etaBin4Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==12 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoScaleSF_etaBin5Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        if(tree->Photon_seedGain[iGam]==12 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoScaleSF_etaBin6Gain12        ->Fill(tree->Photon_pt[iGam], phoScaleSF);
        //---
        tPhoScaleSF_Up     ->Fill(tree->Photon_pt[iGam], phoScaleSF_Up  );
        tPhoScaleSF_Down   ->Fill(tree->Photon_pt[iGam], phoScaleSF_Down);
        tPhoSmearSF        ->Fill(tree->Photon_pt[iGam], phoSmearSF     );
        tPhoSmearSF_Up     ->Fill(tree->Photon_pt[iGam], phoSmearSF_Up  );
        tPhoSmearSF_Down   ->Fill(tree->Photon_pt[iGam], phoSmearSF_Down);
        if(tree->Photon_seedGain[iGam]==1)
            tPhoSmearSF_Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF     );
        if(tree->Photon_seedGain[iGam]==6)
            tPhoSmearSF_Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF     );
        if(tree->Photon_seedGain[iGam]==12)
            tPhoSmearSF_Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF     );
        //in eta bins
        //Gain =1
        if(tree->Photon_seedGain[iGam]==1 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoSmearSF_etaBin1Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==1 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoSmearSF_etaBin2Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==1 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoSmearSF_etaBin3Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==1 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoSmearSF_etaBin4Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==1 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoSmearSF_etaBin5Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==1 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoSmearSF_etaBin6Gain1        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        //Gain =6
        if(tree->Photon_seedGain[iGam]==6 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoSmearSF_etaBin1Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==6 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoSmearSF_etaBin2Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==6 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoSmearSF_etaBin3Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==6 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoSmearSF_etaBin4Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==6 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoSmearSF_etaBin5Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==6 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoSmearSF_etaBin6Gain6        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        //Gain =12
        if(tree->Photon_seedGain[iGam]==12 && -1.442 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.2) 
            tPhoSmearSF_etaBin1Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==12 && -1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= -1.0) 
            tPhoSmearSF_etaBin2Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==12 && -1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 0.0) 
            tPhoSmearSF_etaBin3Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==12 && 0.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.0) 
            tPhoSmearSF_etaBin4Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==12 && 1.0 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.2) 
            tPhoSmearSF_etaBin5Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        if(tree->Photon_seedGain[iGam]==12 && 1.2 < tree->Photon_eta[iGam] && tree->Photon_eta[iGam] <= 1.442) 
            tPhoSmearSF_etaBin6Gain12        ->Fill(tree->Photon_pt[iGam], phoSmearSF);
        //---
    }
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==1) {
   	  h2phoj1->Fill(ptgam, footprint, w);
   	  pphoj1->Fill(ptgam, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal1->Fill(ptgam, bal, w);
   	    prmpf1->Fill(ptgam, mpf, w);
   	  }
   	}
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==6) {
   	  h2phoj6->Fill(ptgam, footprint, w);
   	  pphoj6->Fill(ptgam, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal6->Fill(ptgam, bal, w);
   	    prmpf6->Fill(ptgam, mpf, w);
   	  }
   	}
   	if (iGam!=-1 && tree->Photon_seedGain[iGam]==12) {
   	  h2phoj12->Fill(ptgam, footprint, w);
   	  pphoj12->Fill(ptgam, footprint, w);
   	  if (pass_jeteta && pass_alpha100) {
   	    prbal12->Fill(ptgam, bal, w);
   	    prmpf12->Fill(ptgam, mpf, w);
   	  }
   	}
   
   	if (pass_jeteta && pass_alpha100) {
   	  prbal->Fill(ptgam, bal, w);
   	  prmpf->Fill(ptgam, mpf, w);
   	  prbal0->Fill(ptgam, bal, w);
   	  prmpf0->Fill(ptgam, mpf, w);
   	  
   	  // PF composition plots
   	  h2pteta->Fill(ptgam, tree->Jet_eta[iJet], w);
   	  pabseta->Fill(ptgam, fabs(tree->Jet_eta[iJet]), w);
   
   	  // 1D composition and response
   	  pdb->Fill(ptgam, bal, w);
   	  pmpf->Fill(ptgam, mpf, w);
   	  pchf->Fill(ptgam, tree->Jet_chHEF[iJet], w);
   	  pnhf->Fill(ptgam, tree->Jet_neHEF[iJet], w);
   	  pnef->Fill(ptgam, tree->Jet_neEmEF[iJet], w);
   	  pcef->Fill(ptgam, tree->Jet_chEmEF[iJet], w);
   	  pmuf->Fill(ptgam, tree->Jet_muEF[iJet], w);
   	  
   	  // 2D composition and response
   	  if (ptgam>230) {
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
   
   	  if (isMC) {
   	    if (ptgam>=105 && ptgam<230)
   	      hmus->Fill(tree->Pileup_nTrueInt, w);
   	    h2mus->Fill(ptgam, tree->Pileup_nTrueInt, w);
   	  }
   	  else {
   	    for (int i=0; i!=100; ++i) {
   	      double mu = gRandom->Gaus(tree->Pileup_nTrueInt,TruePUrms);
   	      if (ptgam>=105 && ptgam<230)
   		hmus->Fill(mu, 0.01*w);
   	      h2mus->Fill(ptgam, mu, 0.01*w);
   	    } // for i in 100
   	  } // is MC
   	  //if (ptgam>=130 && ptgam<175) {
   	  if (ptgam>230 && iGam!=-1) {
   	    pgainvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_seedGain[iGam], w);
   	    if (tree->Photon_eCorr) // safety for 2016
   	      pcorrvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_eCorr[iGam], w);
   	    perrvsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_energyErr[iGam], w);
   	    phoevsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_hoe[iGam], w);
   	    pr9vsmu->Fill(tree->Pileup_nTrueInt, tree->Photon_r9[iGam], w);
   	  }
   	  if (ptgam>230) {
   	    pmuvsmu->Fill(tree->Pileup_nTrueInt, tree->Pileup_nTrueInt, w);
   	    prhovsmu->Fill(tree->Pileup_nTrueInt, tree->fixedGridRhoFastjetAll, w);
   	    pnpvgoodvsmu->Fill(tree->Pileup_nTrueInt, tree->PV_npvsGood, w);
   	    pnpvallvsmu->Fill(tree->Pileup_nTrueInt, tree->PV_npvs, w);
   	  } // high pT range
   	  
   	  if (iGam!=-1) {
   	    pgain1vspt->Fill(ptgam, tree->Photon_seedGain[iGam]==1 ? 1 : 0, w);
   	    pgain6vspt->Fill(ptgam, tree->Photon_seedGain[iGam]==6 ? 1 : 0, w);
   	    pgain12vspt->Fill(ptgam, tree->Photon_seedGain[iGam]==12 ? 1 : 0, w);
   	    pgainvspt->Fill(ptgam, tree->Photon_seedGain[iGam], w);
   	    if (tree->Photon_eCorr) // safety for 2016
   	      pcorrvspt->Fill(ptgam, tree->Photon_eCorr[iGam], w);
   	    perrvspt->Fill(ptgam, tree->Photon_energyErr[iGam], w);
   	    h2hoevspt->Fill(ptgam, tree->Photon_hoe[iGam], w);
   	    phoevspt->Fill(ptgam, tree->Photon_hoe[iGam], w);
   	    h2r9vspt->Fill(ptgam, tree->Photon_r9[iGam], w);
   	    pr9vspt->Fill(ptgam, tree->Photon_r9[iGam], w);
   	  }
   	  pmuvspt->Fill(ptgam, tree->Pileup_nTrueInt, w);
   	  prhovspt->Fill(ptgam, tree->fixedGridRhoFastjetAll, w);
   	  pnpvgoodvspt->Fill(ptgam, tree->PV_npvsGood, w);
   	  pnpvallvspt->Fill(ptgam, tree->PV_npvs, w);
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
   	  bool pass_alpha = (pt2 < alpha*ptgam || pt2 < pt2min);
   	  bool pass_eta = (abseta >= ymin && abseta < ymax);
   	  bool pass = (pass_basic_ext && pass_alpha && pass_eta);
   	  
   	  if (pass) {
   	    
   	    // Retrieve pointers to correct set of histograms/profiles
   	    int ia = int(100*alpha);
   	    int iy = 100*int(10*ymin) + int(10*ymax);
   	    
   	    // Get reference instead of pointer so can use . and not ->
   	    BasicHistosGamJet *pmh = mBasicHistosGamJet[iy][ia][ips]; assert(pmh);
   	    BasicHistosGamJet& mh = (*pmh); assert(mh.hn);
   	    
   	    // Fill histograms (h*) and profiles (p*)
   	    //assert(fabs(mpf1+mpfn+mpfu-mpf)<1e-4);
   	    mh.hn->Fill(ptgam);
   	    mh.hxsec->Fill(ptgam, w*wps);
   	    mh.prpt->Fill(ptgam, ptgam, w*wps);
   	    mh.prbal->Fill(ptgam, bal, w*wps);
   	    mh.prdb->Fill(ptgam, mpf1, w*wps);
   	    mh.prmpf->Fill(ptgam, mpf, w*wps);
   	    mh.prmpf1->Fill(ptgam, mpf1, w*wps);
   	    mh.prmpfn->Fill(ptgam, mpfn, w*wps);
   	    mh.prmpfu->Fill(ptgam, mpfu, w*wps);
   	    mh.prho->Fill(ptgam, tree->fixedGridRhoFastjetAll, w);
   	    mh.pdjes->Fill(ptgam, djes, w);
   	    mh.pjes->Fill(ptgam, jes, w);
   	    mh.pres->Fill(ptgam, res, w);
   	  } // pass
   	} // for ips in PSWeight
   	} // for ieta in etas
         } // for ialpha in alphas
   
         if (doGamjet && hg) {
   
   	MoreHistosGamJet *h = hg;
   
   	// Specific event selection
   	bool pass_alpha = (pt2 < 1.00*ptgam || pt2 < pt2min);
   	bool pass_eta = (fabs(tree->Jet_eta[iJet]) < 1.3);
   	bool pass = (pass_basic_ext && pass_alpha && pass_eta);
   
   	if (pass) {
   	
   	  double jsf = (smearJets ? Jet_CF[iJet] : 1);
   	  double ptjet = tree->Jet_pt[iJet];
   	  double ptavp = 0.5*(ptgam + ptjet); // Do better later, now pT,ave (not pT,avp)
   	  h->hpt13->Fill(ptgam, w);
   	  h->hpt13a->Fill(ptavp, w);
   	  h->hpt13j->Fill(ptjet, w);
   	  
   	  h->pptg->Fill(ptgam, ptgam, w);
   	  h->pptj->Fill(ptgam, ptjet, w);
   	  
   	  h->pres->Fill(ptgam, res, w);
   	  h->pjsf->Fill(ptgam, jsf, w);
   	  h->pm0->Fill(ptgam, mpf, w);
   	  h->pm2->Fill(ptgam, mpf1, w);
   	  h->pmn->Fill(ptgam, mpfn, w);
   	  h->pmu->Fill(ptgam, mpfu, w);
   	  
   	  h->pm0x->Fill(ptgam, mpfx, w);
   	  h->pm2x->Fill(ptgam, mpf1x, w);
   	  
   	  // Extras for FSR studies
   	  h->pmnu->Fill(ptgam, mpfnu, w);
   	  h->pmnx->Fill(ptgam, mpfnx, w);
   	  h->pmux->Fill(ptgam, mpfux, w);
   	  h->pmnux->Fill(ptgam, mpfnux, w);
   	  
   	  // Composition
   	  h->prho->Fill(ptgam, tree->fixedGridRhoFastjetAll, w);
   	  h->pchf->Fill(ptgam, tree->Jet_chHEF[iJet], w);
   	  h->pnhf->Fill(ptgam, tree->Jet_neHEF[iJet], w);
   	  h->pnef->Fill(ptgam, tree->Jet_neEmEF[iJet], w);
   	  
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
   
   	MoreHistosGamJet2 *h = hg2;
   
   	// Specific event selection
   	bool pass_alpha = (pt2 < 1.00*ptgam || pt2 < pt2min);
   	//bool pass_eta = (abseta >= ymin && abseta < ymax);
   	bool pass = (pass_basic_ext && pass_alpha);// && pass_eta);
   
   	if (pass) {
   	
   	  //double res = Jet_RES[iprobe] / Jet_RES[itag];
   	  double jsf = (smearJets ? Jet_CF[iJet] : 1);
   	  
   	  double abseta = fabs(tree->Jet_eta[iJet]);
   	  h->h2pteta->Fill(abseta, ptgam, w);
   	  
   	  h->p2res->Fill(abseta, ptgam, res, w);
   	  h->p2jsf->Fill(abseta, ptgam, jsf, w);
   	  h->p2m0->Fill(abseta, ptgam, mpf, w);
   	  h->p2m2->Fill(abseta, ptgam, mpf1, w);
   	  h->p2mn->Fill(abseta, ptgam, mpfn, w);
   	  h->p2mu->Fill(abseta, ptgam, mpfu, w);
   	  
   	  h->p2m0x->Fill(abseta, ptgam, mpfx, w);
   	  h->p2m2x->Fill(abseta, ptgam, mpf1x, w);
   	  
   	  // Extras for FSR studies
   	  h->p2mnu->Fill(abseta, ptgam, mpfnu, w);
   	  h->p2mnx->Fill(abseta, ptgam, mpfnx, w);
   	  h->p2mux->Fill(abseta, ptgam, mpfux, w);
   	  h->p2mnux->Fill(abseta, ptgam, mpfnux, w);
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
   
     // Add extra plot for jet response vs photon pT
     if (isMC) {
       fout->cd("control");
       TH1D *hrgenvgen = prgen->ProjectionX("hrgenvgen");
       TH1D *hrgenvgam = prjet->ProjectionX("hrgenvgam");
       hrgenvgam->Divide(pgjet);
       TH1D *hrgen2vgen = prgen2->ProjectionX("hrgen2vgen");
       TH1D *hrgen2vgam = prjet2->ProjectionX("hrgen2vgam");
       hrgen2vgam->Divide(pgjet2);
       curdir->cd();
     }
     fout->Write();
     cout << "File written." << endl << flush;
     fout->Close();
     cout << "File closed." << endl << flush;
     return 0;
}
   
