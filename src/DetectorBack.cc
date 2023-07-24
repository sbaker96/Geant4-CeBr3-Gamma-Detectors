//===========================
//DetectorBack Implementation
//===========================

#include "DetectorBack.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"

namespace CeBr3
{

DetectorBack::DetectorBack()
{;}

DetectorBack::~DetectorBack()
{;}

DetectorBack::ConstructDetectorBack()
{
	//Parameters
	G4double sInRad = (pmt->GetWidth())/2;
	G4double sOutRad = (pmt->GetWidth())/2 + shieldThickness();
	G4double s_hz = (pmt->GetLength())/2;

	//Sheilding Solid
	G4Tubs* sSolid = new G4Tubs("sSolid", sInRad, sOutRad, s_hz, 0*deg, 360*deg);

	//Sheilding Logical Volume
	G4LogicalVolume* sLog = new G4LogicalVolume(sSolid, shieldMat, "sLog");

	//Construct DetectorBack Assembly
	
	detBack = new G4AssemblyVolume*;

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

	detBack->AddPlacedAssembly(pmt->GetPMT(), Ma);
	detBack->AddPlacedVolume(sLog, Ma);

}

DetectorBack::CalculateLength()
{
	length = pmt->GetLength();
	
	return length;
}

DetectorBack::CalculateWidth()
{
	width = pmt->GetWidth() + 2*shieldThickness();

	return width;
}

}
