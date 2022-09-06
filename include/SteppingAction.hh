//==============================
// Stepping Action Header
//==============================


#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "TrackInformation.hh"

namespace CeBr3
{

class SteppingAction : public G4UserSteppingAction
{

	public:
		SteppingAction();
		virtual ~SteppingAction();

		virtual void UserSteppingAction(const G4Step*);

	private:

};

}


#endif
