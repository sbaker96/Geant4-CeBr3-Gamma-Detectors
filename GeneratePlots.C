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

//void plot2DRaw(int numA, int numB);

const char* numAppend(const char* txt, int num);

//////////////////////

//Constants//

const char* srcName = "output.root";
const char* outName = "Plots.root";
const char* histName = "Edep_";

const int nofDetectors = 1;
const int maxEnergy = 5000; // in keV

/////////////////////

//Main Function//

int GeneratePlots()
{
	TFile* outFile = new TFile(outName, "RECREATE");

	outFile->Close();
	
	const char* writeName = numAppend("Flush", 0);
	
	for(int i = 0; i < nofDetectors; i++)
	{
		plotRaw(i);
//		plotFolded(i);
//		plotFoldedNoZero(i);
//		plotFoldedNormal(i);
		plotFoldedNormalNoZero(i);
	}
	return 0;

}

void plotRaw(int num)
{
	/*
        TFile f(srcName);

	auto edep = numAppend(histName, num);
	
	TH1D* src = (TH1D*)f.Get(edep);

        int lastFilledBin = src->FindLastBinAbove();

        int nofBins = 1.5*lastFilledBin;

        const char* writeName = numAppend("Raw_", num);

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

	for(int i = 0; i < nofBins; i++)
	{
		
		int currentBinContent = src->GetBinContent(i);

			for(int k = 0; k < currentBinContent; k++)
			{
				outHist->Fill(i);
			}
	}

        outHist->SetOption("HIST");

        outHist->SetLineColor(1);
	
        TFile* outFile = new TFile(outName, "UPDATE");

        outHist->Write();

        outFile->Close();
*/
	auto inFile = TFile::Open(srcName);
	
	TTreeReader reader("Edep by Detectors", inFile);

	TTreeReaderValue<float> Edep(reader, numAppend(histName, num));

	int nofBins = maxEnergy;
			    
	const char* writeName = numAppend("Raw_", num);

	TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

	while(reader.Next())
	{
		float edep = *Edep;

		edep *= 1000; //Convert Units

		cout << edep << endl;
	
		outHist->Fill(edep);

	}

	outHist->SetOption("HIST");

	outHist->SetLineColor(1);

	TFile* outFile = new TFile(outName, "UPDATE");

	outHist->Write();

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

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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
                outHist->FillRandom("g", currentBinContent);
                }

        }


        outHist->SetOption("HIST");

        outHist->SetLineColor(1);

	TFile* outFile = new TFile(outName, "UPDATE");
	
	outHist->Write();

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

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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
                outHist->FillRandom("g", currentBinContent);
                }

        }

        for(int i = 0; i < 20; i++)
        {
        outHist->SetBinContent(i, 0);
        }


        outHist->SetOption("HIST");

        outHist->SetLineColor(1);

        TFile* outFile = new TFile(outName, "UPDATE");

        outHist->Write();

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

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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
                outHist->FillRandom("g", currentBinContent);
                }

        }

        double factor = 1.0;

        outHist->Scale(factor/outHist->GetMaximum());

        outHist->SetOption("HIST");

        outHist->SetLineColor(1);

        TFile* outFile = new TFile(outName, "UPDATE");

        outHist->Write();

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

       	TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

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
                outHist->FillRandom("g", currentBinContent);
                }

        }

        for(int i = 0; i < 20; i++)
        {
        outHist->SetBinContent(i, 0);
        }


        double factor = 1.0;

        outHist->Scale(factor/outHist->GetMaximum());

        outHist->SetOption("HIST");

        outHist->SetLineColor(1);

        TFile* outFile = new TFile(outName, "UPDATE");

        outHist->Write();

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
