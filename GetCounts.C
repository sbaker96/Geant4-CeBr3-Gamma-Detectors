#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>

using namespace std;

using std::vector;

////////////////////

//Constants//

vector<double> energies {
	846.77,
	1037.843,
	1360.212,
	1771.357,
	2015.215,
	2034.755,
	2598.5,
	3202.029,
	3253.503,
	3273.079,
	3451.232,

};
const char* fileName = "Plots.root";
const char* plotName = "Raw_0";
///////////////////


//Main Function

int GetCounts()
{
       auto inFile = TFile::Open(fileName);
       TH1F* srcHist = static_cast<TH1F*>(inFile->Get(plotName));

	cout << setw(14) << "Energy";
	cout << " | Counts" << endl;
	for(auto i : energies)
	{
		int bin = (i + 1);
		int counts = srcHist->GetBinContent(bin);
		cout << setw(10) << i;
	        cout << " keV | ";
	        cout << counts << endl;
	}
	
	cout << "Just Counts:" << endl;

	for(auto i : energies)
	{
		int bin = (i + 1);
		int counts = srcHist->GetBinContent(bin);
		cout << counts << endl;
	}

return 0;

}
