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


	G4int nofHits = trackerHC->entries();
	
	//Create Set  of TrackIDs corresponding to Decay Gammas

	set<G4int> decayGammaIDs; //Using a set prevents the need to keep track of duplicates

	//Loop through all Hits
	for(G4int i = 0; i < nofHits; i++)
	{
		G4int currentSrc = (*trackerHC)[i]->GetDecayGammaSourceID();
		if(currentSrc != -1)
		{
			//Check that the gamma itself deposited energy
			G4bool isGamma = (*trackerHC)[i]->GetIsGamma();
			if(isGamma)
			{
				decayGammaIDs.insert(currentSrc);
			}
		}
		
	}
	//-----
	
    	auto analysisManager = G4AnalysisManager::Instance();
	G4int nofDetectors = 2;	
	
	//Fill Ntuple
	
	//Loop through Ids
	for(auto itr = decayGammaIDs.begin(); itr != decayGammaIDs.end(); itr++)
	{
		G4int currentGammaID = *itr;
	
		//Loop through number of detectors
		for(G4int n = 0; n < nofDetectors; n++)
		{
		
			G4double totalEdep = 0.0;
			
			//Loop through hits
			for(G4int i = 0; i < nofHits; i++)
                        {

                                //Add energy if hit matches current detector and decay ID
				if((*trackerHC)[i]->GetDecayGammaSourceID() == currentGammaID
                                                && (*trackerHC)[i]->GetDetectorNumber() == n)
                                { totalEdep += (*trackerHC)[i]->GetEdep(); }

                        }

			//Add total to ntuple
			analysisManager->FillNtupleFColumn(n, totalEdep);

		}

		//Add eventID to ntuple
		G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

		analysisManager->FillNtupleIColumn(nofDetectors, eventID);

		//Complete ntuple row
		analysisManager->AddNtupleRow();
	}
}

}
