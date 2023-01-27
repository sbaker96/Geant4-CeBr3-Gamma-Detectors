#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

using std::set;

using namespace std;
///////////////////////

//Function Prototypes//
void plotRaw(int num);
void plotFolded(int num);

void plot2DRaw(int numA, int numB);
void plot2DFolded(int numA, int numB);

const char* numAppend(const char* txt, int num);

//////////////////////

//Constants//

const char* srcName = "output.root";
const char* outName = "Plots.root";
const char* histName = "Edep_";

const int nofDetectors = 1;
const int maxEnergy = 4000; // in keV

/////////////////////

//Main Function//

int GeneratePlots()
{
	//Create Output File
	TFile* outFile = new TFile(outName, "RECREATE");

	outFile->Close();

	//Clear writeName
	const char* writeName = numAppend("Flush", 0);
	
	//Generate raw and folded plots for each detector
	for(int i = 0; i < nofDetectors; i++)
	{
		plotRaw(i);
		plotFolded(i);
	}

	//Generate raw and folded 2D plots
	for(int j = 0; j < nofDetectors; j++)
	{
/*
		for(int k = 0; k < j; k++)
		{
			plot2DRaw(k, j);
			plot2DFolded(k, j);
		}
*/
	}

	return 0;

}

void plotRaw(int num)
{
	//Start Message
	cout << "Plotting Raw Data for Detector " << num << endl;
	
	//Open output from simulation
	auto inFile = TFile::Open(srcName);
	
	//Create reader for the output ntuple
	TTreeReader reader("Edep by Gamma", inFile);

	//Get name of branch to look at
	const char* branchName = numAppend(histName, num);

	//Create readerValue for Edep
	TTreeReaderValue<float> Edep(reader, branchName);

	//Create Histogram
	int nofBins = maxEnergy;

	const char* writeName = numAppend("Raw_", num);

	TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

	//Loop through values to fill histogram
	while(reader.Next())
	{
		float edep = *Edep;

		edep *= 1000; //Convert Units

		outHist->Fill(edep);

	}

	//Set Histogram Options
	outHist->SetOption("HIST");

	outHist->SetLineColor(1);

	//Write Histogram to output file
	TFile* outFile = new TFile(outName, "UPDATE");

	outHist->Write();

	outFile->Close();


}

void plotFolded(int num)
{
	//Start Message 
	cout << "Plotting Folded Data for Detector " << num << endl;

	TFile* inFile = new TFile(outName);

	TH1F* srcHist = static_cast<TH1F*>(inFile->Get(numAppend("Raw_", num)));
	
	//Create Histogram
        int nofBins = maxEnergy;
	
	const char* writeName = numAppend("Fold_", num);

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

	//Create Functions
        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

	auto g = new TF1("g", "gausn(0)");

	int maxFilledBin = srcHist->FindLastBinAbove();

	for(int i = 2; i <= maxFilledBin; i++)
	{
		int counts  = srcHist->GetBinContent(i);
		
		if(counts != 0) 
		{
		g->SetParameter(0, 1);
                g->SetParameter(1, i);
                g->SetParameter(2, stDev->Eval(i));

                outHist->FillRandom("g", counts);
		}
	}


	//Normalize Histogram
  	double factor = 1.0;

	outHist->Scale(factor/outHist->GetMaximum());	//Sets the highest bin to one

	//Set Histogram Option
	outHist->SetOption("HIST");

        outHist->SetLineColor(1);

	//Write Histogram to output file
	TFile* outFile = new TFile(outName, "UPDATE");
	
	outHist->Write();

	outFile->Close();
}

void plot2DRaw(int numA, int numB)
{
	//Start Message
	cout << "Plotting 2D Raw for Detctors "<< numA << " and " << numB << endl;

	//Open output from Simulation
	auto inFile = TFile::Open(srcName);

	//Create reader for the output ntuple
        TTreeReader reader("Edep by Event", inFile);

	//Create readerValues for Edep
        TTreeReaderValue<float> EdepA(reader, numAppend(histName, numA));
        
	TTreeReaderValue<float> EdepB(reader, numAppend(histName, numB));
	
	//Create Histogram
        int nofBins = maxEnergy;

        const char* writeName = numAppend(numAppend("Compare_", numA), numB);

	TH2F* outHist = new TH2F(writeName, writeName, nofBins, 0, nofBins, nofBins, 0, nofBins);
	
	while(reader.Next())
	{
	
		float edepA = *EdepA*1000;

		float edepB = *EdepB*1000;


		outHist->Fill(edepA, edepB);
	
	}

	//Set Histogram Options
	outHist->SetOption("COLZ");

	//Write Histogram to output file
        TFile* outFile = new TFile(outName, "UPDATE");

        outHist->Write();

        outFile->Close();


}

void plot2DFolded(int numA, int numB)
{

	//Start Message
	cout << "Plotting 2D Folded for Detctors "<< numA << " and " << numB << endl;

	TFile* inFile = new TFile(outName);

        TH1F* srcHist = static_cast<TH1F*>(inFile->Get(numAppend(numAppend("Compare_", numA), numB)));

	//Create Histogram
        int nofBins = maxEnergy;

        const char* writeName = numAppend(numAppend("CompareFold_", numA), numB);

	TH2F* outHist = new TH2F(writeName, writeName, nofBins, 0, nofBins, nofBins, 0, nofBins);
	
	//Create Functions
	TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);
		
	TF2* g = new TF2("g", "xygaus(0)");

	//Get the maximum filled bins
        int maxFilledBinX = srcHist->FindLastBinAbove(1);

	int maxFilledBinY = srcHist->FindLastBinAbove(2);

	//Loop through x bins
        for(int i = 2; i <= maxFilledBinX; i++)
        {
		//Loop through y bins
		for(int k = 2; k <= maxFilledBinY; k++)
		{
			//Get number of counts
			int counts  = srcHist->GetBinContent(i, k);
			
                	if(counts != 0)
                	{
			cout << "X: " << i << " Y: " << k << endl;

               		g->SetParameter(0, 1);
                	g->SetParameter(1, i);
                	g->SetParameter(2, stDev->Eval(i));
			g->SetParameter(3, k);
			g->SetParameter(4, stDev->Eval(k));

			cout << "Filling Histogram... " << endl;
                	outHist->FillRandom("g", counts);
			cout << "Filled" << endl << endl;
                	}
		}
        }


	//Set Histogram Options
	outHist->SetOption("COLZ");
	
	//Write Histogram to output file
        TFile* outFile = new TFile(outName, "UPDATE");
        
	outHist->Write();

        outFile->Close();


}

//Function to append a number to a constant character array
const char* numAppend(const char* txt, int num)
{

	stringstream ss;

        ss << txt << num;
	
	const char* output = ss.str().c_str();

//	std::cout << output << std::endl;

	return output;

}
