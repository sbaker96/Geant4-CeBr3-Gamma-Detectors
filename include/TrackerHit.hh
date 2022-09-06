//==============================
// TrackerHit Header
//=============================


#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"


namespace CeBr3
{

class TrackerHit : public G4VHit
{
	public:
		TrackerHit();
		TrackerHit(const TrackerHit&) = default;
		~TrackerHit() override;

		//operators
		TrackerHit& operator=(const TrackerHit&) = default;
		G4bool operator==(const TrackerHit&) const;

		inline void* operator new(size_t);
		inline void operator delete(void*);

		//Base Class Methods
		void Print() override;

		//Set Methods
//		void SetTrackID(G4int track)		{fTrackID = track; };
//		void SetParentID(G4int parent)		{fParentID = parent; };
		void SetDecayGammaSourceID(G4int src)	{fDecayGammaSourceID = src; };
//		void SetType(G4String type)		{fType = type; };
//		void SetCreationProcess(G4String proc)	{fCreationProcess = proc; };
		void SetEdep(G4double de)		{fEdep = de; };

		//Get Methods
//		G4int GetTrackID() const		{return fTrackID; };
//		G4int GetParentID() const		{return fParentID; };
		G4int GetDecayGammaSourceID() const	{return fDecayGammaSourceID; };
//		G4String GetCreationProcess() const	{return fCreationProcess; };
//		G4String GetType() const		{return fType; };
		G4double GetEdep() const		{return fEdep; };

	private:
//		G4int fTrackID;
//		G4int fParentID;
		G4int fDecayGammaSourceID;
//		G4String fCreationProcess;
//		G4String fType;
		G4double fEdep = 0.;

};

//=====================================

//typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

using TrackerHitsCollection = G4THitsCollection<TrackerHit>;

extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;



//====================================

inline void* TrackerHit::operator new(size_t)
{
	if(!TrackerHitAllocator) {
		TrackerHitAllocator = new G4Allocator<TrackerHit>; }
	void *hit;

	hit = (void *) TrackerHitAllocator->MallocSingle();
	return hit;
}

//===================================

inline void TrackerHit::operator delete(void *hit)
{
	if(!TrackerHitAllocator) {
                TrackerHitAllocator = new G4Allocator<TrackerHit>; }

	TrackerHitAllocator->FreeSingle((TrackerHit*) hit);
}

}

#endif
