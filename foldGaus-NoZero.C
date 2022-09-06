//===========================
// Folding Histograms Macro
//===========================



{
	gROOT->SetStyle("Plain");

        TFile f("test.root");

        TCanvas* c3 = new TCanvas("c3", "", 20, 20, 1000, 1000);

	TH1D* src = (TH1D*)f.Get("Edep");

	int lastFilledBin = src->FindLastBinAbove();

	int nofBins = lastFilledBin*1.5;

	TH1F* h1 = new TH1F("h1", "h1", nofBins, 0, nofBins);

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
		h1->FillRandom("g", currentBinContent);
		}

	}

	for(int i = 0; i < 20; i++)
	{
	
	h1->SetBinContent(i, 0);

	}

	double factor = 1.0;

	h1->Scale(factor/h1->GetMaximum());
	
	h1->Draw("HIST");

	std::unique_ptr<TFile> out( TFile::Open("Folded-NoZero.root", "RECREATE") );

        out->WriteObject(h1, "h1");


}
