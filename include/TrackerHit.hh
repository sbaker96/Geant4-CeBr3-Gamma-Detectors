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
		void SetDetectorNumber(G4int dNum)	{fDetectorNumber = dNum; };
		void SetIsGamma(G4bool g)		{fIsGamma = g; };
		void SetDecayGammaSourceID(G4int src)	{fDecayGammaSourceID = src; };
		void SetOriginalEng(G4double ogE)	{fOriginalEng = ogE;	};
		void SetEdep(G4double de)		{fEdep = de; };

		void SetPosition(G4ThreeVector pos)	{fPos = pos; };

		//Get Methods
		G4int GetDetectorNumber() const		{return fDetectorNumber; };
		G4bool GetIsGamma() const 		{return fIsGamma; };
		G4int GetDecayGammaSourceID() const	{return fDecayGammaSourceID; };
		G4double GetOriginalEng() const		{return fOriginalEng;	};
		G4double GetEdep() const		{return fEdep; };

		G4ThreeVector GetPosition() const	{return fPos; };

	private:
		G4int fDetectorNumber = -1;
		G4bool fIsGamma = 0;
		G4int fDecayGammaSourceID;
		G4double fOriginalEng;
		G4double fEdep = 0.;

		G4ThreeVector fPos;
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
