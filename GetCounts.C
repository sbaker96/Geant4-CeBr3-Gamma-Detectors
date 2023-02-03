#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>

using namespace std;

using std::vector;

////////////////////

//Constants//

vector<double> energies {
121.7817,
244.6975,
344.2785,
411.1163,
443.9606,
778.904,
867.38,
964.057,
1085.837,
1112.076,
1408.006

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
