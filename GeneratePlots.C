#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

///////////////////////

//Function Prototypes//
void plotRaw(int num);
void plotFolded(int num);
void plotFoldedNoZero(int num);
void plotFoldedNormal(int num);
void plotFoldedNormalNoZero(int num);

const char* numAppend(const char* txt, int num);

//////////////////////

//Constants//

const char* srcName = "output.root";
const char* outName = "Plots.root";
const char* histName = "Edep_";

int nofDetectors = 2;

/////////////////////

//Main Function//

int GeneratePlots()
{
	TFile* outFile = new TFile(outName, "RECREATE");

	outFile->Close();
	
	for(int i = 0; i < nofDetectors; i++)
	{
		plotRaw(i);
		plotFolded(i);
		plotFoldedNoZero(i);
		plotFoldedNormal(i);
		plotFoldedNormalNoZero(i);
	}
	return 0;

}

void plotRaw(int num)
{
        TFile f(srcName);

	auto edep = numAppend(histName, num);
	
	TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        const char* writeName = numAppend("Raw_", num);

        TH1F* folded = new TH1F(writeName, writeName, nofBins, 0, nofBins);

        folded->SetOption("HIST");

        folded->SetLineColor(1);
	
        TFile* outFile = new TFile(outName, "UPDATE");

        folded->Write();

        outFile->Close();

}

void plotFolded(int num)
{
        TFile f(srcName);

	auto edep = numAppend(histName, num);

	TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

	const char* writeName = numAppend("Fold_", num);

        TH1F* folded = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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

	TFile* outFile = new TFile(outName, "UPDATE");
	
	folded->Write();

	outFile->Close();
}

void plotFoldedNoZero(int num)
{
       TFile f(srcName);

	auto edep = numAppend(histName, num);
        
	TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

	const char* writeName = numAppend("FoldNZ_", num);

        TH1F* folded = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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

        TFile* outFile = new TFile(outName, "UPDATE");

        folded->Write();

        outFile->Close();
}

void plotFoldedNormal(int num)
{

        TFile f(srcName);
	
	auto edep = numAppend(histName, num);

        TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;
	
	const char* writeName = numAppend("FoldNorm_", num);

        TH1F* folded = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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

        TFile* outFile = new TFile(outName, "UPDATE");

        folded->Write();

        outFile->Close();
}

void plotFoldedNormalNoZero(int num)
{
	TFile f(srcName);
	
	auto edep = numAppend(histName, num);
       	
	TH1D* src = (TH1D*)f.Get(edep);

       	int lastFilledBin = src->FindLastBinAbove();

       	int nofBins = 1.5*lastFilledBin;
	
	const char* writeName = numAppend("FoldNormNZ_", num);

       	TH1F* folded = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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

        TFile* outFile = new TFile(outName, "UPDATE");

        folded->Write();

        outFile->Close();
}


const char* numAppend(const char* txt, int num)
{

	stringstream ss;

        ss << txt  << num;
	
	const char* output = ss.str().c_str();

//	std::cout << output << std::endl;

	return output;

}
