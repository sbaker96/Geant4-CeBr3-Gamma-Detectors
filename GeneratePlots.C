
#include <iostream>
#include <ostream>

//Function Prototypes//
void plotRaw();
void plotFolded();
void plotFoldedNoZero();
void plotFoldedNormal();
void plotFoldedNormalNoZero();
//////////////////////


int GeneratePlots()
{

	plotRaw();
	plotFolded();
	plotFoldedNoZero();
	plotFoldedNormal();
	plotFoldedNormalNoZero();

	return 0;

}

void plotRaw()
{
        TFile f("test.root");

        TH1D* raw = (TH1D*)f.Get("Edep");

        int lastFilledBin = raw->FindLastBinAbove();

        raw->GetXaxis()->SetRange(0, 1.5*lastFilledBin);

        raw->SetOption("HIST");

        raw->SetLineColor(1);

        std::unique_ptr<TFile> out( TFile::Open("Plots.root", "RECREATE") );

        out->WriteObject(raw, "Raw");

}

void plotFolded()
{
        TFile f("test.root");

        TH1D* src = (TH1D*)f.Get("Edep");

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

        std::unique_ptr<TFile> out( TFile::Open("Plots.root", "UPDATE") );

        out->WriteObject(folded, "Folded");

}

void plotFoldedNoZero()
{
       TFile f("test.root");

        TH1D* src = (TH1D*)f.Get("Edep");

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

        std::unique_ptr<TFile> out( TFile::Open("Plots.root", "UPDATE") );

        out->WriteObject(folded, "Folded-NoZero");



}

void plotFoldedNormal()
{

        TFile f("test.root");

        TH1D* src = (TH1D*)f.Get("Edep");

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

        std::unique_ptr<TFile> out( TFile::Open("Plots.root", "UPDATE") );

        out->WriteObject(folded, "Folded-Normal");


}

void plotFoldedNormalNoZero()
{
       TFile f("test.root");

        TH1D* src = (TH1D*)f.Get("Edep");

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

        std::unique_ptr<TFile> out( TFile::Open("Plots.root", "UPDATE") );

        out->WriteObject(folded, "FoldedNormal-NoZero");



}
