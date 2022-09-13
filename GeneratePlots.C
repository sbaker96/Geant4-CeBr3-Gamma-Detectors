#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

//Function Prototypes//
void plotRaw(const char* edep);
void plotFolded(const char* edep);
void plotFoldedNoZero(const char* edep);
void plotFoldedNormal(const char* edep);
void plotFoldedNormalNoZero(const char* edep);
//////////////////////

//Constants//

const char* srcName = "output.root";
const char* outName = "Plots.root";
const char* histName = "Edep_";

int nofDetectors = 2;

/////////////////////

int GeneratePlots()
{

//	for(int num = 0; num < nofDetectors; num++);
	{
		int num = 0;
		stringstream ss;
	
		ss << histName << num;
	
		const char* edep = ss.str().c_str();
	
		plotRaw(edep);
		plotFolded(edep);
		plotFoldedNoZero(edep);
		plotFoldedNormal(edep);
		plotFoldedNormalNoZero(edep);
//	}
	return 0;

}

void plotRaw(const char* edep)
{
        TFile f(srcName);
        
	TH1D* raw = (TH1D*)f.Get(edep);

        int lastFilledBin = raw->FindLastBinAbove();

        raw->GetXaxis()->SetRange(0, 1.5*lastFilledBin);

        raw->SetOption("HIST");

        raw->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open(outName, "RECREATE") );

        out->WriteObject(raw, "Raw");

}

void plotFolded(const char* edep)
{
        TFile f(srcName);

        TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        TH1F* folded = new TH1F("Folded", "Folded", nofBins, 0, nofBins);

        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

        for(int i = 0; i < nofBins; i++)
        {
                auto g = new TF1("g", "gausn(0)");
                g->SetParameter(0, 1);
                g->SetParameter(1, i);
                g->SetParameter(2, stDev->Eval(i));

                int currentBinContent = src->GetBinContent(i);

                if(currentBinContent > 0)
                {
                folded->FillRandom("g", currentBinContent);
                }

        }


        folded->SetOption("HIST");

        folded->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open(outName, "UPDATE") );

        out->WriteObject(folded, "Folded");

}

void plotFoldedNoZero(const char* edep)
{
       TFile f(srcName);

        TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        TH1F* folded = new TH1F("Folded", "Folded", nofBins, 0, nofBins);

        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

        for(int i = 0; i < nofBins; i++)
        {
                auto g = new TF1("g", "gausn(0)");
                g->SetParameter(0, 1);
                g->SetParameter(1, i);
                g->SetParameter(2, stDev->Eval(i));

                int currentBinContent = src->GetBinContent(i);

                if(currentBinContent > 0)
                {
                folded->FillRandom("g", currentBinContent);
                }

        }

        for(int i = 0; i < 20; i++)
        {
        folded->SetBinContent(i, 0);
        }


        folded->SetOption("HIST");

        folded->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open(outName, "UPDATE") );

        out->WriteObject(folded, "Folded-NoZero");



}

void plotFoldedNormal(const char* edep)
{

        TFile f(srcName);

        TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        TH1F* folded = new TH1F("Folded", "Folded", nofBins, 0, nofBins);

        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

        for(int i = 0; i < nofBins; i++)
        {
                auto g = new TF1("g", "gausn(0)");
                g->SetParameter(0, 1);
                g->SetParameter(1, i);
                g->SetParameter(2, stDev->Eval(i));

                int currentBinContent = src->GetBinContent(i);

                if(currentBinContent > 0)
                {
                folded->FillRandom("g", currentBinContent);
                }

        }

        double factor = 1.0;

        folded->Scale(factor/folded->GetMaximum());

        folded->SetOption("HIST");

        folded->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open(outName, "UPDATE") );

        out->WriteObject(folded, "Folded-Normal");


}

void plotFoldedNormalNoZero(const char* edep)
{
       TFile f(srcName);

        TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        TH1F* folded = new TH1F("Folded", "Folded", nofBins, 0, nofBins);

        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

        for(int i = 0; i < nofBins; i++)
        {
                auto g = new TF1("g", "gausn(0)");
                g->SetParameter(0, 1);
                g->SetParameter(1, i);
                g->SetParameter(2, stDev->Eval(i));

                int currentBinContent = src->GetBinContent(i);

                if(currentBinContent > 0)
                {
                folded->FillRandom("g", currentBinContent);
                }

        }

        for(int i = 0; i < 20; i++)
        {
        folded->SetBinContent(i, 0);
        }


        double factor = 1.0;

        folded->Scale(factor/folded->GetMaximum());

        folded->SetOption("HIST");

        folded->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open(outName, "UPDATE") );

        out->WriteObject(folded, "FoldedNormal-NoZero");



}
