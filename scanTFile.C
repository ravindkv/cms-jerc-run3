#include <TFile.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <iostream>


//root -b -q -l "scanTFile.C(\"TFile.root\")";

void printInfo(TObject *obj) {
    if (TTree *tree = dynamic_cast<TTree*>(obj)) {
        std::cout<< setw(15) << "TTree: " << setw(35)<< tree->GetName() << setw(10) << tree->GetEntries() << std::endl;
    }
    else if (TH1F *h1f = dynamic_cast<TH1F*>(obj)) {
        std::cout<< setw(15) << "TH1F: " << setw(35)<< h1f->GetName() << setw(10) << h1f->GetEntries() << setw(10) << h1f->GetMean() << setw(10) << h1f->GetRMS() << std::endl;
    }
    else if (TH2D *h2d = dynamic_cast<TH2D*>(obj)) {
        std::cout<< setw(15) << "TH2D: " << setw(35)<< h2d->GetName() << setw(10) << h2d->GetEntries() << setw(10) << h2d->GetMean() << setw(10) << h2d->GetRMS() << std::endl;
    }
    else if (TProfile *prof = dynamic_cast<TProfile*>(obj)) {
        std::cout<< setw(15) << "TProfile: " << setw(35)<< prof->GetName() << setw(10) << prof->GetEntries() << setw(10) << prof->GetMean() << setw(10) << prof->GetRMS() << std::endl;
    }
    else if (TProfile2D *prof2d = dynamic_cast<TProfile2D*>(obj)) {
        std::cout<< setw(15) << "TProfile2D: " << setw(35)<< prof2d->GetName() << setw(10) << prof2d->GetEntries() << setw(10) << prof2d->GetMean() << setw(10) << prof2d->GetRMS() << std::endl;
    }
}

void scanDirectory(TDirectory *dir, const std::string &path = "") {
    std::string currentPath = path + dir->GetName() + "/";
    std::cout << "\nDirectory: " << currentPath << std::endl;

    TIter next(dir->GetListOfKeys());
    TKey *key;

    while ((key = (TKey*)next())) {
        TObject *obj = key->ReadObj();
        
        if (obj->InheritsFrom(TDirectory::Class())) {
            scanDirectory((TDirectory*)obj, currentPath); // Recursive call for subdirectories
        } else {
            printInfo(obj);
        }
    }
}

void scanTFile(const char *fileName) {
    TFile *file = TFile::Open(fileName);
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }
		cout<<"\n-----------: Scan all directories and print Entries, Mean, RMS :------------\n"<<endl;
    scanDirectory(file);

    file->Close();
    delete file;
}

