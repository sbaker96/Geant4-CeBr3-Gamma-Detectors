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

	//Create N-Tuple
	
	analysisManager->CreateNtuple("Edep by Detectors", "Edep by Detectors");
	
	analysisManager->CreateNtupleFColumn("Edep_0");

	analysisManager->CreateNtupleFColumn("Edep_1");

	analysisManager->CreateNtupleIColumn("EventID");

	analysisManager->FinishNtuple();

	analysisManager->SetNtupleMerging(true);
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
