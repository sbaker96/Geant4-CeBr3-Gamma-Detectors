//===========================
//DetectorBack Implementation
//===========================

#include "DetectorBack.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
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

	//Shielding Solid
	G4Tubs* sSolid = new G4Tubs("sSolid", sInRad, sOutRad, s_hz, 0*deg, 360*deg);

	//Shielding Logical Volume
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

	//Add Support
	
	if(!detSup)		//If no support, end construction
		return;

	//Parameters
	G4double sup_hx = CalculateLength()*0.75;
	G4double sup_hy = detSup->GetHeight()/2;
	G4double sup_hz = CalculateLength()*0.75;

	G4double sup_rad = CalculateWidth()/2;

	G4Material* supMat = detSup->GetMat();

	G4Transform3D holeMa = detSup->GenerateHoleShift();
	G4Transform3D supMa = detSup->GenerateShift();

	//Support Base Solid
	G4Box* supBaseSolid = new G4Box("supBaseSolid", sup_hx, sup_hy, sup_hz);
	G4Tubs* supHoleSolid = new G4Tubs("supHoleSolid", 0*cm, sup_rad, 2*sup_hz, 0*rad, 2*CLHEP::pi*rad);

	G4SubtractionSolid* supSolid = new G4SubtractionSolid("supSolid", supBaseSolid, supHoleSolid, holeMa);

	//Support Logical Volume
	G4LogicalVolume* supLog = new G4LogicalVolume(supSolid, supMat, "supLog");

	//Place Support
	detBack->AddPlacedVolume(supLog, supMa);


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
