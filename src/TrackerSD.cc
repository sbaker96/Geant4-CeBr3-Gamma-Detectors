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

	//energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();

        TrackInformation* trackInfo = (TrackInformation*)aStep->GetTrack()->GetUserInformation();

        G4int srcID = trackInfo->GetDecayGammaSourceID();

	if ( srcID  == -1) return false;

	TrackerHit* newHit = new TrackerHit();

//	newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
//	newHit->SetParentID(aStep->GetTrack()->GetParentID());
	
	newHit->SetDecayGammaSourceID(srcID);

//	newHit->SetType(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());

//	newHit->SetCreationProcess(aStep->GetTrack()->GetCreatorProcess()->GetProcessName());

	newHit->SetEdep(edep);

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

