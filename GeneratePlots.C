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

const int nofDetectors = 2;
const int maxEnergy = 3000; // in keV

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
	plot2DRaw(0, 1);
	plot2DFolded(0, 1);

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
	
	const char* writeName = numAppend("Fold_", num);

        TH1F* outHist = new TH1F(writeName, writeName, nofBins, 0, nofBins);

	//Create Functions
        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);

	auto g = new TF1("g", "gausn(0)");

	//Loop through values to fill histogram
	while(reader.Next())
        {
                float edep = *Edep;

                edep *= 1000; //Convert Units
	
		//Set Parameters of gaussian
		if( edep != 0)
		{	
		g->SetParameter(0, 1);
		g->SetParameter(1, edep);
		g->SetParameter(2, stDev->Eval(edep));

                outHist->FillRandom("g", 1);
		}

        }

	//Normalize Histogram
  	double factor = 1.0;

	outHist->Scale(factor/outHist->GetMaximum());

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
	cout << "Plotting 2D Raw" << endl;

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
	cout << "Plotting 2D Folded" << endl;

	//Open output from Simulation
	auto inFile = TFile::Open(srcName);

	//Create reader for the output ntuple
        TTreeReader reader("Edep by Event", inFile);

	//Create readerValues for Edep
        TTreeReaderValue<float> EdepA(reader, numAppend(histName, numA));
        
	TTreeReaderValue<float> EdepB(reader, numAppend(histName, numB));
	
	//Create Histogram
        int nofBins = maxEnergy;

        const char* writeName = numAppend(numAppend("CompareFold_", numA), numB);

	TH2F* outHist = new TH2F(writeName, writeName, nofBins, 0, nofBins, nofBins, 0, nofBins);
	
	//Create Functions
	TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(100/sqrt(x))", 0 , 1000*nofBins);
		
	TF2* g = new TF2("g", "[0]*exp(-(0.5*((x-[1])/[2])**2+(0.5*((y-[3])/[4])**2)))/(sqrt(2*pi)*[2]*[4])", 0, 20, 0, 20); //!!!

        while(reader.Next())
        {

                float edepA = *EdepA*1000;

                float edepB = *EdepB*1000;

                if( edepA > 0 && edepB > 0)
                {
                g->SetParameter(0, 1);
                g->SetParameter(1, edepA);
                g->SetParameter(2, stDev->Eval(edepA));
                g->SetParameter(3, edepB);
                g->SetParameter(4, stDev->Eval(edepB));

//              cout << "A: " << edepA << " | B: " << edepB << endl;

                outHist->FillRandom("g", 1);

                }

        }


	//Set Histogram Options
	outHist->SetOption("COLZ");

	//Normalize Histogram
	double factor = 1.0;

        outHist->Scale(factor/outHist->GetMaximum());

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
