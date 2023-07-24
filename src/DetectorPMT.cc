//=============================
////Detector PMT Implementation
//=============================

#include "DetectorPMT.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"

namespace CeBr3
{

DetectorPMT::DetectorPMT()
{;}

DetectorPMT::~DetectorPMT()
{;}

void DetectorPMT::ConstructPMT()
{
	//Parameters
	G4double inRad = 0*mm;

	G4double glassOutRad = width/2;
	G4double glass_hz = length/2;

	G4double gasOutRad = glassOutRad - thickness;
	G4double gas_hz = glass_hz - thickness;

	//Solid Construction
	G4Tubs* outerSolid = new G4Tubs("outerSolid", inRad, glassOutRad, glass_hz, 0*deg, 360*deg);
	G4Tubs* innerSolid = new G4Tubs("innerSolid", inRad, gasOutRad, gas_hz, 0*deg, 360*deg);

	G4SubtractionSolid* shellSolid = new G4SubtractionSolid("shellSolid", outerSolid, innerSolid);

	//Logical Volume Construction
	
	G4LogicalVolume* glassLog = new G4LogicalVolume(shellSolid, glassMat, "PMT Glass");
	G4LogicalVolume* gasLog = new G4LogicalVolume(innerSolid, gasMat, "PMT Gas"); 

	//Assembly Construction
	
	pmt = new G4AssemblyVolume();

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma =  G4Transform3D(Ra, Ta);

	pmt->AddPlacedVolume(glassLog, Ma);
	pmt->AddPlacedVolume(gasLog, Ma);
	

}

	
}
