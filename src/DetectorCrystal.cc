//==============================
//DetectorCrystal Implementation
//==============================

#include "DetectorCrystal.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

namespace CeBr3
{

DetectorCrystal::DetectorCrystal()
{;}

DetectorCrystal::~DetectorCrystal()
{;}

void DetectorCrystal::ConstructCrystal()
{
	G4double inRad = 0.0*mm;
	G4double outRad = width/2;
	G4double hz = length/2;

	G4Tubs* solid = new G4Tubs("Solid", inRad, outRad, hz, 0*deg, 360*deg);

	crystal = new G4LogicalVolume(solid, mat, name);
}

}
