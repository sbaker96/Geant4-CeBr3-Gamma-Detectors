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
	
       	G4TrackVector* secTracks = fpSteppingManager->GetfSecondary();

	size_t nofSecTracks = (*secTracks).size();
	
	G4Track* aTrack = aStep->GetTrack();

	G4int copyNumber = aTrack->GetTouchable()->GetCopyNumber();

	G4String name = aTrack->GetTouchable()->GetVolume()->GetName();

	std::cout << "Copy Number: " << copyNumber << " | Volume Name: " << name  << std::endl;

       	if(nofSecTracks > 0)
       	{
/*
		G4Track* aTrack = aStep->GetTrack();

		G4int copyNumber = aTrack->GetTouchable()->GetCopyNumber();

		std::cout << "Copy Number: " << copyNumber << std::endl;
*/
        	TrackInformation* trackInfo = (TrackInformation*)aTrack->GetUserInformation();

      		G4int srcID = trackInfo->GetDecayGammaSourceID();
               	for(size_t i = 0; i < nofSecTracks; i++)
               	{
			if(!(*secTracks)[i]->GetUserInformation())
			{
	                TrackInformation* secInfo = new TrackInformation();
			secInfo->SetDecayGammaSourceID(srcID);
			(*secTracks)[i]->SetUserInformation(secInfo);
               		}
		}
        }



}

}
