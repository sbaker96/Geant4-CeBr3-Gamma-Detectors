//==============================
//Tracker Hit Implementation
//==============================

#include "TrackerHit.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

namespace CeBr3
{

G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator = nullptr;

//==========================

TrackerHit::TrackerHit()
{}

//=========================

TrackerHit::~TrackerHit()
{}

//==========================

G4bool TrackerHit::operator==(const TrackerHit& right) const 
{
     	return ( this == &right ) ? true : false; 
}

//=========================

void TrackerHit::Print()
{
/*
	G4cout
	<< " Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy")
	<< G4endl;
*/	
}

}

