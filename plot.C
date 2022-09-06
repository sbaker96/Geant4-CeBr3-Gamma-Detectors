//==============================
// Histogram Plotting Macro
//==============================

{
	gROOT->Reset();
	gROOT->SetStyle("Plain");

	TFile f("test.root");

	TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);

	TH1D* hist1 = (TH1D*)f.Get("Edep");
	
	int lastFilledBin = hist1->FindLastBinAbove();

	hist1->GetXaxis()->SetRange(0, 1.5*lastFilledBin);

	c1->SetLogy();
	hist1->Draw("HIST");

        std::unique_ptr<TFile> out( TFile::Open("Raw.root", "RECREATE") );

        out->WriteObject(hist1, "h1");



}



