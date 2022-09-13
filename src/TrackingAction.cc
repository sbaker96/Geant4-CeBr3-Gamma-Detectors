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
	G4int parentID = aTrack->GetParentID();
	
	if( parentID == 0 )
	{		
	TrackInformation* trackInfo = new TrackInformation();
	fpTrackingManager->SetUserTrackInformation(trackInfo);
	}

	TrackInformation* trackInfo = (TrackInformation*)aTrack->GetUserInformation();
	
        if(aTrack->GetDefinition() == G4Gamma::Definition() && aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
        {
                G4int trackID = aTrack->GetTrackID();
                trackInfo->SetDecayGammaSourceID(trackID);
        }

	G4int copyNumber = aTrack->GetTouchable()->GetCopyNumber();
//	std::cout << "Copy Number: " << copyNumber << std::endl;
	G4int dNum = (copyNumber - 1)/3;
	trackInfo->SetDetectorNumber(dNum);	
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{;}

}
