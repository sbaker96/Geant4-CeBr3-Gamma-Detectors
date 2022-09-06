//=============================
// TrackerSD Header
// ===========================

#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "TrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

namespace CeBr3
{
class TrackerSD : public G4VSensitiveDetector
{
	public:
		TrackerSD(const G4String& name,
			const G4String& hitsCollectionName);
		~TrackerSD() override;

		//Base Class Methods
		void Initialize(G4HCofThisEvent* hitCollection) override;
		G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
		void EndOfEvent(G4HCofThisEvent* hitCollection) override;

	private:
		TrackerHitsCollection* fHitsCollection = nullptr;
};

}

#endif
