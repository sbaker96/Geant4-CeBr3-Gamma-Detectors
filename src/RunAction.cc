//=============================
// Run Action Implementation
// ============================

#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

namespace CeBr3

{

RunAction::RunAction()
{

	G4RunManager::GetRunManager()->SetPrintProgress(10000);

	auto analysisManager = G4AnalysisManager::Instance();

	analysisManager->SetVerboseLevel(1);

	//Create Histograms
	
	analysisManager->CreateH1("Edep_0", "Edep_0", 5000, 0., 5000*keV);

	analysisManager->CreateH1("Edep_1", "Edep_1", 5000, 0., 5000*keV);
/*
	analysisManager->CreateH2("Compare_0_1", "Compare_0_1", 5000, 0., 5000*keV, 5000, 0., 5000*keV);
*/

	//Create N-Tuple
	
	analysisManager->CreateNTuple("Edep by Detectors", "Edep by Detectors");
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{

	auto analysisManager = G4AnalysisManager::Instance();

	G4String fileName = "output.root";

	analysisManager->OpenFile(fileName);

}

void RunAction::EndOfRunAction(const G4Run*)
{

	auto analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();

}


}
