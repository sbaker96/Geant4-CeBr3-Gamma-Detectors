//==================================
// Event Action Implementation
// =================================

#include "EventAction.hh"

#include "TrackerHit.hh"
#include "TrackerSD.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

using std::set;

namespace CeBr3
{

EventAction::EventAction()
{}

EventAction::~EventAction()
{}

TrackerHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{

	auto hitsCollection = static_cast<TrackerHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

	if (! hitsCollection ) {
		G4ExceptionDescription msg;
		msg << "Cannot Access hitsCollection ID " << hcID;
		G4Exception("EventAction::GetHitsCollection()",
			"Test", FatalException, msg);
	}

	return hitsCollection;

}

void EventAction::BeginOfEventAction(const G4Event* event)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{

	if (fHCID == -1)
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("TrackerHitsCollection");	

	auto trackerHC = GetHitsCollection(fHCID, event);

	const G4int nofDetectors = 1;

    	auto analysisManager = G4AnalysisManager::Instance();

	G4int nofHits = trackerHC->entries();
	
	////////////////
	//Fill Ntuples//
	////////////////

	set<G4int>validIDs[nofDetectors+1]; //validIds[n] corresponds to the valid ids for detector n,
					    //except for validIds[nofDetectors] which is the full set of
					    //valid ids.

	//Loop through hits
	
	for(G4int i = 0; i < nofHits; i++)
	{
		G4int currentSrc = (*trackerHC)[i]->GetDecayGammaSourceID();
                if(currentSrc != -1)
		{
			if((*trackerHC)[i]->GetIsGamma())
			{
				validIDs[nofDetectors].insert(currentSrc);
				validIDs[(*trackerHC)[i]->GetDetectorNumber()].insert(currentSrc);	
			}
		}
	}

	//Edep by Gamma
	
	//Loop through Ids
	for(auto itr = validIDs[nofDetectors].begin(); itr != validIDs[nofDetectors].end(); itr++)
       	{
		G4int currentID = *itr;

		//Loop through detectors
		for(G4int n = 0; n < nofDetectors; n++)
		{
		
			if(validIDs[n].count(currentID))
			{
				G4double totalEdep = 0.0;

				//Loop through hits
				for(G4int i = 0; i < nofHits; i++)
				{
				
					//Add energy if hit matches current detector and decay ID
               		                if((*trackerHC)[i]->GetDecayGammaSourceID() == currentID
                                       	        && (*trackerHC)[i]->GetDetectorNumber() == n)
                               		{ totalEdep += (*trackerHC)[i]->GetEdep(); }

				}
		
				//Add total to ntuple
       		                analysisManager->FillNtupleFColumn(0, n, totalEdep);
			
			}

			else
			{ analysisManager->FillNtupleFColumn(0, n, 0.0); }
		
		}

        	//Add eventID to ntuple
       	        G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

               	analysisManager->FillNtupleIColumn(0, nofDetectors, eventID);
        
		//Complete ntuple row
       	        analysisManager->AddNtupleRow(0);

	
	}

	//Edep by Event

	//Loop through each detector//
	for(G4int n = 0; n < nofDetectors; n++)
	{
		G4double totalEdep = 0.0;	//Total Energy of Event

		//Loop through Ids
                for(auto itr = validIDs[n].begin(); itr != validIDs[n].end(); itr++)
		{
			G4int currentID = *itr;
		
			//Loop Through Hits
			for(G4int i = 0; i < nofHits; i++)
			{
				if((*trackerHC)[i]->GetDecayGammaSourceID() == currentID && (*trackerHC)[i]->GetDetectorNumber() == n)
				{ totalEdep += (*trackerHC)[i]->GetEdep(); }
			}
		
		}

		analysisManager->FillNtupleFColumn(1, n, totalEdep);
	}

	analysisManager->AddNtupleRow(1);


	//Get Just Full Energy Peaks
	
	//Loop through Ids
	for(auto itr = validIDs[nofDetectors].begin(); itr != validIDs[nofDetectors].end(); itr++)
       	{
		G4int currentID = *itr;

		//Loop through detectors
		for(G4int n = 0; n < nofDetectors; n++)
		{
		
			if(validIDs[n].count(currentID))
			{
				G4double totalEdep = 0.0;
				G4double ogE = 0.0;

				//Loop through hits
				for(G4int i = 0; i < nofHits; i++)
				{
				
					//Add energy if hit matches current detector and decay ID
               		                if((*trackerHC)[i]->GetDecayGammaSourceID() == currentID
                                       	        && (*trackerHC)[i]->GetDetectorNumber() == n)
                               		{ 
						totalEdep += (*trackerHC)[i]->GetEdep();
					       	ogE = (*trackerHC)[i]->GetOriginalEng();
					//	std::cout << ogE << std::endl;
					}

				}
		
				//Add total to ntuple
       		                //analysisManager->FillNtupleFColumn(0, n, totalEdep);
			
				if(ogE == totalEdep)
				{ 
					analysisManager->FillNtupleFColumn(2, n, totalEdep);
			//		std::cout << "Full Energy Peak" << std::endl; 
				}
			}

			else
			{ analysisManager->FillNtupleFColumn(2, n, 0.0); }
		
		}

        	//Add eventID to ntuple
       	        G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

               	analysisManager->FillNtupleIColumn(2, nofDetectors, eventID);
        
		//Complete ntuple row
       	        analysisManager->AddNtupleRow(2);

	
	}


}

}
