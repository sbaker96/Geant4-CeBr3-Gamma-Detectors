//================================
// Tracking Action Implementation
//================================


#include "TrackingAction.hh"
#include "TrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"

#include "G4Gamma.hh"
#include "G4RadioactiveDecay.hh"

#include "G4VProcess.hh"

#include "G4ios.hh"

namespace CeBr3
{

TrackingAction::TrackingAction()
	:G4UserTrackingAction()
{;}

TrackingAction::~TrackingAction()

{;}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	//Get Parent ID
	G4int parentID = aTrack->GetParentID();
	
	//If track is a primary particle, create new track info
	if( parentID == 0 )
	{		
	TrackInformation* trackInfo = new TrackInformation();
	fpTrackingManager->SetUserTrackInformation(trackInfo);
	}

	//Get track info
	TrackInformation* trackInfo = (TrackInformation*)aTrack->GetUserInformation();
	
	//Setting Decay Gamma Source IDs//

	//For simulations starting with nuclei
        if(aTrack->GetDefinition() == G4Gamma::Definition() && aTrack->GetTrackID() > 1)
	{		
		if(aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
       		{
                G4int trackID = aTrack->GetTrackID();
		G4double eng = aTrack->GetTotalEnergy();
                trackInfo->SetDecayGammaSourceID(trackID);
		trackInfo->SetOriginalEng(eng);
       		}
	}

	//For simulations starting with gamma rays
	if(aTrack->GetDefinition() == G4Gamma::Definition() && aTrack->GetTrackID() == 1)
	{
	
		G4int trackID = aTrack->GetTrackID();
		trackInfo->SetDecayGammaSourceID(trackID);

	}

}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{;}

}
