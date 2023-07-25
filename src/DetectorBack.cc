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

void DetectorBack::ConstructDetBack()
{
	//Parameters
	G4double sInRad = (pmt->GetWidth())/2;
	G4double sOutRad = (pmt->GetWidth())/2 + shieldThickness;
	G4double s_hz = (pmt->GetLength())/2;

	G4double s_x = 0*mm;
	G4double s_y = 0*mm;
	G4double s_z = s_hz;

	//Sheilding Solid
	G4Tubs* sSolid = new G4Tubs("sSolid", sInRad, sOutRad, s_hz, 0*deg, 360*deg);

	//Sheilding Logical Volume
	G4LogicalVolume* sLog = new G4LogicalVolume(sSolid, shieldMat, "sLog");

	//Construct DetectorBack Assembly
	
	detBack = new G4AssemblyVolume;

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

	Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
	Ma = G4Transform3D(Ra, Ta);

	detBack->AddPlacedAssembly(pmt->GetPMT(), Ma);
	detBack->AddPlacedVolume(sLog, Ma);

}

G4double DetectorBack::CalculateLength()
{
	G4double length = pmt->GetLength();
	
	return length;
}

G4double DetectorBack::CalculateWidth()
{
	G4double width = pmt->GetWidth() + 2*shieldThickness;

	return width;
}

}
