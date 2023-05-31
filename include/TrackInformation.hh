//==============================
//TrackInformation Header
//==============================

#ifndef TrackInformation_h
#define TrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "globals.hh"

#include "G4ParticleDefinition.hh" 
#include "G4Track.hh" 
#include "G4Allocator.hh"

namespace CeBr3
{

class TrackInformation : public G4VUserTrackInformation
{

	public:
		TrackInformation();
		TrackInformation(const G4Track* aTrack);
		TrackInformation(G4int srcID, G4double ogE);
		virtual ~TrackInformation();
		
		inline void *operator new(size_t);
		inline void operator delete(void *aTrackInfo);

		void SetDecayGammaSourceID(G4int srcID)	{ fDecayGammaSourceID = srcID; };
		void SetOriginalEng(G4double ogE)	{ fOriginalEng = ogE; };

		G4int GetDecayGammaSourceID()		{ return fDecayGammaSourceID; };
		G4double GetOriginalEng()		{ return fOriginalEng; };

	private:
		G4int fDecayGammaSourceID = -1; //TrackID of the Gamma Ray resulting from radioactive
						//decay that is the ancestor of this track.
		G4double fOriginalEng = 0.0;	//Original Energy of the first gamma ray
};

extern G4ThreadLocal
	G4Allocator<TrackInformation> * aTrackInformationAllocator;

inline void* TrackInformation::operator new(size_t)
{

	if(!aTrackInformationAllocator)
		aTrackInformationAllocator = new G4Allocator<TrackInformation>;
	return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void TrackInformation::operator delete(void *aTrackInfo)

{ aTrackInformationAllocator->FreeSingle((TrackInformation*)aTrackInfo);}


}
#endif
