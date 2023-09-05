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
	G4double glass_x = 0*mm;
	G4double glass_y = 0*mm;
	G4double glass_z = 0*mm;

	G4double cathodeOutRad = glassOutRad - thickness;
	G4double cathode_hz = cathodeThick/2;
	G4double cathode_x = glass_x;
	G4double cathode_y = glass_y;
	G4double cathode_z = glass_z - (glass_hz - thickness - cathode_hz);

	G4double gasOutRad = glassOutRad - thickness;
	G4double gas_hz = glass_hz - cathode_hz - thickness;
	G4double gas_x = glass_x;
	G4double gas_y = glass_y;
	G4double gas_z = glass_z + (cathode_hz);


	//Solid Construction
	G4Tubs* outerSolid = new G4Tubs("outerSolid", inRad, glassOutRad, glass_hz, 0*deg, 360*deg);
	G4Tubs* innerSolid = new G4Tubs("innerSolid", inRad, gasOutRad, gas_hz + cathode_hz, 0*deg, 360*deg);
	G4Tubs* cathodeSolid = new G4Tubs("cathodeSolid", inRad, cathodeOutRad, cathode_hz, 0*deg, 360*deg);
	G4Tubs* gasSolid = new G4Tubs("gasSolid", inRad, gasOutRad, gas_hz, 0*deg, 360*deg);


	G4SubtractionSolid* shellSolid = new G4SubtractionSolid("shellSolid", outerSolid, innerSolid);

	//Logical Volume Construction
	
	G4LogicalVolume* glassLog = new G4LogicalVolume(shellSolid, glassMat, "PMTGlass");
	G4LogicalVolume* cathodeLog = new G4LogicalVolume(cathodeSolid, cathodeMat, "PMTCathode");
	G4LogicalVolume* gasLog = new G4LogicalVolume(gasSolid, gasMat, "PMTGas"); 

	//Assembly Construction
	
	pmt = new G4AssemblyVolume();

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;
	G4Transform3D Ma;

	Ta.setX(glass_x); Ta.setY(glass_y); Ta.setZ(glass_z);
        Ma =  G4Transform3D(Ra, Ta);
	pmt->AddPlacedVolume(glassLog, Ma);
	
	Ta.setX(cathode_x); Ta.setY(cathode_y); Ta.setZ(cathode_z);
        Ma =  G4Transform3D(Ra, Ta);
	pmt->AddPlacedVolume(cathodeLog, Ma);
	
	Ta.setX(gas_x); Ta.setY(gas_y); Ta.setZ(gas_z);
        Ma =  G4Transform3D(Ra, Ta);
	pmt->AddPlacedVolume(gasLog, Ma);
	
}

	
}
