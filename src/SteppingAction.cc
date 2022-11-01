//================================
// Stepping Action Implementation
//================================


#include "SteppingAction.hh"

#include "TrackInformation.hh"

#include "G4ios.hh"

#include "G4Track.hh" 
#include "G4Step.hh" 
#include "G4StepPoint.hh"
#include "G4SteppingManager.hh"
#include "G4TrackingManager.hh"
#include "G4TrackVector.hh"

namespace CeBr3
{

SteppingAction::SteppingAction()
	: G4UserSteppingAction()
{;}

SteppingAction::~SteppingAction()
{;}


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	//Get tracks of secondary particles	
       	G4TrackVector* secTracks = fpSteppingManager->GetfSecondary();

	size_t nofSecTracks = (*secTracks).size();
	
	//Get track of current step
	G4Track* aTrack = aStep->GetTrack();

       	if(nofSecTracks > 0) //Check that there are secondary tracks
       	{
		//Get Decay Gamma Source ID of current track
        	TrackInformation* trackInfo = (TrackInformation*)aTrack->GetUserInformation();

		G4int srcID = trackInfo->GetDecayGammaSourceID();
               
		//Propagate the Decay Gamma Source ID to secondaries
		for(size_t i = 0; i < nofSecTracks; i++)
               	{
			if(!(*secTracks)[i]->GetUserInformation())	//Check for existance of track infos to prevent duplicates
			{
	                TrackInformation* secInfo = new TrackInformation();
			secInfo->SetDecayGammaSourceID(srcID);
			(*secTracks)[i]->SetUserInformation(secInfo);
               		}
		}
        }

//Debug
	G4cout << aStep->GetTrack()->GetPosition() << G4endl;
}

}
