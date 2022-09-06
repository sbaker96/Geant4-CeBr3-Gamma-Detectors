//============================
//Event Action Header
//============================

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "TrackerHit.hh"

#include "globals.hh"

namespace CeBr3
{

class EventAction : public G4UserEventAction
{

	public:
		EventAction();
		~EventAction() override;

		void BeginOfEventAction(const G4Event* event) override;
		void EndOfEventAction(const G4Event* event) override;

	private:
		TrackerHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
	//	void PrintEventStatistics(G4double Edep) const;
		G4int fHCID = -1;

};


}

#endif
