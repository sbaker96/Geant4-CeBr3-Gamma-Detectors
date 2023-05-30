//================================
//TrackerSD Implementation
//================================

#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

#include "TrackInformation.hh"
#include "G4Track.hh"

namespace CeBr3
{

TrackerSD::TrackerSD(const G4String& name,
		const G4String& hitsCollectionName)
	: G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);
}

//=================

TrackerSD::~TrackerSD()
{}

//================

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
	//Create Hits Collection
	fHitsCollection
		= new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

	//Add Collection in hce
	G4int hcID
		= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection(hcID, fHitsCollection);

}

//=================

G4bool TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

	//Get energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();
	
	//Get copy number of detector
	G4int copyNumber = aStep->GetTrack()->GetTouchable()->GetCopyNumber();
	
	//print copyNumber
	//std::cout << copyNumber << std::endl;

	//Get detector number from copy number
	
       	//Parameters
	G4int n = 1;	//Number of physical volumes before detectors are placed (including world)
	G4int p = 3;	//Number of components of each detector

	G4int dNum = (copyNumber - n)/p; //This turns the copy number to detector number.
					 //For this equation to work.

        //Get particle type
        G4String type = aStep->GetTrack()->GetDefinition()->GetParticleType();

	//Get track info (Class defined by TrackInformation.hh)
        TrackInformation* trackInfo = (TrackInformation*)aStep->GetTrack()->GetUserInformation();
        
	G4int srcID = trackInfo->GetDecayGammaSourceID();
	G4double ogE = trackInfo->GetOriginalEng();

	//---------------------//
	//Debug Parameters 
		
	//Get Position
	G4ThreeVector pos = aStep->GetTrack()->GetPosition();
	
	//--------------------//
	

	//If track does not originate from decay gamma, discard it.
	if ( srcID  == -1) return false;

	//Create New Hit
	TrackerHit* newHit = new TrackerHit();
	
	newHit->SetDecayGammaSourceID(srcID);

	newHit->SetEdep(edep);

	newHit->SetOriginalEng(ogE);
	
	newHit->SetDetectorNumber(dNum);

	if( type == "gamma")
	{
		newHit->SetIsGamma(true);
	}

	//--------------------//
	//Set Debug Parameters
	
	newHit->SetPosition(pos);

	//--------------------//

	fHitsCollection->insert(newHit);

	return true;

}

//==================

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
/*	G4int nofHits = fHitsCollection->entries();
	G4cout << G4endl
		<< "----->Hits Collection: in this event they are " << nofHits
		<< "hits in the tracker: " << G4endl;
	for ( G4int i=0; i<nofHits; i++) (*fHitsCollection)[i]->Print();
*/
}
//======================
}

