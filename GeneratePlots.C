#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

using std::set;

using namespace std;
///////////////////////

//Function Prototypes//
void plotRaw(int num);
void plotFolded(int num);
void plotExp(int num);

void plot2DRaw(int numA, int numB);
void plot2DFolded(int numA, int numB);

void plotPhotopeaks(int num);
void writePhotopeaks(int num);

string numAppend(string txt, int num);
string genSimOutFile(string folder, string base, int num);

//////////////////////

//Constants//

string srcName = "output.root";
string outName = "Plots.root";
string histName = "Edep_";

string simDirName = "simOutputPhotopeaks";
string simFileName = "detector_";

const int nofDetectors = 2;
const int maxEnergy = 4000; // in keV
const int binWidth = 4;

/////////////////////

//Main Function//

int GeneratePlots()
{
	//Create Output File
	TFile* outFile = new TFile(outName.c_str(), "RECREATE");

	outFile->Close();

	//Generate raw and folded plots for each detector
	for(int i = 0; i < nofDetectors; i++)
	{
		plotRaw(i);
		plotFolded(i);
//		plotExp(i);
//		plotPhotopeaks(i);
	
//		writePhotopeaks(i);
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
	auto inFile = TFile::Open(srcName.c_str());
	
	//Create reader for the output ntuple
	TTreeReader reader("Edep by Gamma", inFile);

	//Get name of branch to look at
	string branchName = numAppend(histName, num);

	//Create readerValue for Edep
	TTreeReaderValue<float> Edep(reader, branchName.c_str());

	//Create Histogram
	int nofBins = maxEnergy;

	string writeName = numAppend("Raw_", num);

	TH1F* outHist = new TH1F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins);

	//Loop through values to fill histogram
	while(reader.Next())
	{
		float edep = *Edep;

		edep *= 1000; //Convert Units

		outHist->Fill(edep);

	}

        outHist->SetTitle("Raw Plot");
        outHist->GetXaxis()->SetTitle("E_{#gamma} [keV]");
        outHist->GetXaxis()->CenterTitle();
        outHist->GetYaxis()->SetTitle("Counts/4 keV");
        outHist->GetYaxis()->CenterTitle();



	//Set Histogram Options
	outHist->SetOption("HIST");

	outHist->SetLineColor(1);

	//Write Histogram to output file
	TFile* outFile = new TFile(outName.c_str(), "UPDATE");

	outHist->Write();

	outFile->Close();


}

void plotFolded(int num)
{
	//Start Message 
	cout << "Plotting Folded Data for Detector " << num << endl;

	TFile* inFile = new TFile(outName.c_str());

	TH1F* srcHist = static_cast<TH1F*>(inFile->Get(numAppend("Raw_", num).c_str()));
	
	//Create Histogram
        int nofBins = maxEnergy;
	
	string writeName = numAppend("Fold_", num);

        TH1F* outHist = new TH1F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins);

	//Create Functions
        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(1.20*(106.0)/sqrt(x))", 0 , 1000*nofBins);
/*
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
*/
	int maxFilledBin = srcHist->FindLastBinAbove();
	for(int i = 2; i <= maxFilledBin; i++)
	{
		int counts  = srcHist->GetBinContent(i);
		if(counts != 0)
		{
			for(int j = 0; j < counts; j++)
			{
			double currentBin = srcHist->GetBinCenter(i);
			double bin = currentBin + gRandom->Gaus(1, stDev->Eval(currentBin));
			//std::cout << currentBin << " | " << bin << endl;
			outHist->Fill(bin);
			}
		}
	}

	//Normalize Histogram
  	double factor = 1.0;

//	outHist->Scale(factor/outHist->GetMaximum());	//Sets the highest bin to one

        outHist->SetTitle("Folded Plot");
        outHist->GetXaxis()->SetTitle("E_{#gamma} [keV]");
        outHist->GetXaxis()->CenterTitle();
        outHist->GetYaxis()->SetTitle("Counts/4 keV");
        outHist->GetYaxis()->CenterTitle();



	//Set Histogram Option
	outHist->SetOption("HIST");

	outHist->GetXaxis()->SetTitle("Energy [keV]");
	outHist->GetXaxis()->CenterTitle();
	outHist->GetYaxis()->SetTitle("Counts/4 keV");
	outHist->GetYaxis()->CenterTitle();

        outHist->SetLineColor(1);

	outHist->Rebin(binWidth);
	
	//Write Histogram to output file
	TFile* outFile = new TFile(outName.c_str(), "UPDATE");
	
	outHist->Write();

	outFile->Close();
}

void plotExp(int num)

{
	//Start Message 
	cout << "Plotting Folded Data with Exponential Background for Detector " << num << endl;

	TFile* inFile = new TFile(outName.c_str());

	TH1F* srcHist = static_cast<TH1F*>(inFile->Get(numAppend("Raw_", num).c_str()));
	
	int maxFilledBin = srcHist->FindLastBinAbove();
	
	//Create Histogram
        int nofBins = maxEnergy;
	
	string writeName = numAppend("Exp_", num);

        TH1F* outHist = new TH1F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins);

	TF1* back = new TF1("back", "[0]*exp(-[1]*x)", 0, 1000*nofBins);

	back->SetParameter(0, 6437.944008871607);
	back->SetParameter(1, 0.006219787025252926);

	auto integral = back->Integral(0, 1000*nofBins);

	outHist->FillRandom("back", integral);

	//Create Functions
        TF1* stDev = new TF1("Standard Deviation", "(x/235.5)*(106.1738108093346/sqrt(x))", 0 , 1000*nofBins);

	auto g = new TF1("g", "gausn(0)");

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


	//Scale Back Histogram
//	int nofCounts = 10000000;
//	outHist->Scale(static_cast<double>(nofCounts)/static_cast<double>(integral + nofCounts));	

	outHist->Rebin(binWidth);

	//Set Histogram Option
	outHist->SetOption("HIST");

        outHist->SetLineColor(1);

	//Write Histogram to output file
	TFile* outFile = new TFile(outName.c_str(), "UPDATE");
	
	outHist->Write();

	outFile->Close();
}
void plot2DRaw(int numA, int numB)
{
	//Start Message
	cout << "Plotting 2D Raw for Detctors "<< numA << " and " << numB << endl;

	//Open output from Simulation
	auto inFile = TFile::Open(srcName.c_str());

	//Create reader for the output ntuple
        TTreeReader reader("Edep by Event", inFile);

	//Create readerValues for Edep
        TTreeReaderValue<float> EdepA(reader, numAppend(histName, numA).c_str());
        
	TTreeReaderValue<float> EdepB(reader, numAppend(histName, numB).c_str());
	
	//Create Histogram
        int nofBins = maxEnergy;

        string writeName = numAppend(numAppend("Compare_", numA).c_str(), numB);

	TH2F* outHist = new TH2F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins, nofBins, 0, nofBins);
	
	while(reader.Next())
	{
	
		float edepA = *EdepA*1000;

		float edepB = *EdepB*1000;


		outHist->Fill(edepA, edepB);
	
	}

	//Set Histogram Options
	outHist->SetOption("COLZ");

	//Write Histogram to output file
        TFile* outFile = new TFile(outName.c_str(), "UPDATE");

        outHist->Write();

        outFile->Close();


}

void plot2DFolded(int numA, int numB)
{

	//Start Message
	cout << "Plotting 2D Folded for Detctors "<< numA << " and " << numB << endl;

	TFile* inFile = new TFile(outName.c_str());

        TH1F* srcHist = static_cast<TH1F*>(inFile->Get(numAppend(numAppend("Compare_", numA).c_str(), numB).c_str()));

	//Create Histogram
        int nofBins = maxEnergy;

        string writeName = numAppend(numAppend("CompareFold_", numA), numB);

	TH2F* outHist = new TH2F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins, nofBins, 0, nofBins);
	
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
        TFile* outFile = new TFile(outName.c_str(), "UPDATE");
        
	outHist->Write();

        outFile->Close();


}

void plotPhotopeaks(int num)
{
	//Start Message
	cout << "Plotting Photopeaks for Detector " << num << endl;
	
	//Open output from simulation
	auto inFile = TFile::Open(srcName.c_str());
	
	//Create reader for the output ntuple
	TTreeReader reader("Full Photopeak", inFile);

	//Get name of branch to look at
	string branchName = numAppend(histName, num);

	//Create readerValue for Edep
	TTreeReaderValue<float> Edep(reader, branchName.c_str());

	//Create Histogram
	int nofBins = maxEnergy;

	string writeName = numAppend("Photopeak_", num);

	TH1F* outHist = new TH1F(writeName.c_str(), writeName.c_str(), nofBins, 0, nofBins);

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
	TFile* outFile = new TFile(outName.c_str(), "UPDATE");

	outHist->Write();

	outFile->Close();


}

void writePhotopeaks(int num)
{
	//Start Message
	cout << " Writing Photopeaks for Detector " << num << endl;
	
	//Open output from simulation
	auto inFile = TFile::Open(srcName.c_str());
	
	//Create reader for the output ntuple
	TTreeReader reader("Full Photopeak", inFile);

	//Get name of branch to look at
	string branchName = numAppend(histName.c_str(), num);

	//Create readerValue for Edep
	TTreeReaderValue<float> Edep(reader, branchName.c_str());

	//Loop through values to get gammas
	set<double> gammas;
	while(reader.Next())
	{
		float edep = std::round(*Edep * 10000)/10;

		gammas.insert(edep);
	}

	DIR *dir;

	mkdir(simDirName.c_str(), 0777);

	dir = opendir(simDirName.c_str());
	
	FILE *fpt;

	string outfile = genSimOutFile(simDirName, simFileName, num);

	fpt = fopen(outfile.c_str(), "w+");

	for(auto it = gammas.begin(); it != gammas.end(); ++it)
	{
		float currentGamma = *it; 
		
		reader.Restart();
		
		int counts = 0;
		
		while(reader.Next())
		{
			if(currentGamma == std::round(*Edep * 10000)/10)
				counts++;
		}

		fprintf(fpt, "%f, %d\n", currentGamma, counts);
		
	}
	
	fclose(fpt);

	closedir(dir);
	

}

//Function to append a number to a constant character array
string numAppend(string txt, int num)
{

	stringstream ss;

        ss << num;
	string nums = ss.str();

	string output = txt + nums;
	
	return output;

}

string genSimOutFile(string folder, string base, int num)
{

	string beg = "./";
	string sep = "/";
	string ext = ".dat";

	stringstream ss;

	ss << num;
	string nums = ss.str();

	string output = beg + folder + sep + base + nums + ext;
	
	return output;



	


}


