//=====================================
//Action Initialization Implementation
//=====================================

#include "ActionInitialization.hh"
#include "ActionGenerator.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"

namespace CeBr3
{

ActionInitialization::ActionInitialization()
{}
///////////////////

ActionInitialization::~ActionInitialization()
{}
///////////////////

void ActionInitialization::BuildForMaster() const
{

SetUserAction(new RunAction);

}

///////////////////

void ActionInitialization::Build() const
{
        SetUserAction(new ActionGenerator);
	SetUserAction(new RunAction);
	SetUserAction(new EventAction);
	SetUserAction(new TrackingAction);
	SetUserAction(new SteppingAction);
}

}

