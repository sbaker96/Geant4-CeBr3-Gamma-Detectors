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
	
	//Create List of TrackIDs corresponding to Decay Gammas

	set<G4int> decayGammaIDs;

	for(G4int i = 0; i < nofHits; i++)
	{
		G4int currentSrc = (*trackerHC)[i]->GetDecayGammaSourceID();
		if(currentSrc != -1)
		{
		decayGammaIDs.insert(currentSrc);
		}
		
	}
	//-----
	
	
	for(auto itr = decayGammaIDs.begin(); itr != decayGammaIDs.end(); itr++)
	{
	
		G4int currentGammaID = *itr;
		
		G4double totalEdep = 0.0;
		
		for(G4int i = 0; i < nofHits; i++)
		{
		
			if((*trackerHC)[i]->GetDecayGammaSourceID() == currentGammaID)
			{ totalEdep += (*trackerHC)[i]->GetEdep(); }
		
		}

	        auto analysisManager = G4AnalysisManager::Instance();
//		G4cout << " Total Edep: " << totalEdep << G4endl;
        	analysisManager->FillH1(0, totalEdep);





	
	}
}

}
