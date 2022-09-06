//================================
//TrackInformation Implementation
//================================


#include "TrackInformation.hh"

namespace CeBr3
{

G4ThreadLocal G4Allocator<TrackInformation> * aTrackInformationAllocator = 0;

TrackInformation::TrackInformation()
	: G4VUserTrackInformation()
{;}

TrackInformation::TrackInformation(const G4Track* aTrack)
        : G4VUserTrackInformation()
{;}


TrackInformation::TrackInformation(G4int src)
	: G4VUserTrackInformation()
{

	fDecayGammaSourceID = src;
	
}


TrackInformation::~TrackInformation()
{;}


}
