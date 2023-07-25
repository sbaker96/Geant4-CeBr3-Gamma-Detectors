//============================
//DetectorFront Implementation
//============================

#include "DetectorFront.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"

namespace CeBr3
{

DetectorFront::DetectorFront()
{;}

DetectorFront::~DetectorFront()
{;}

void DetectorFront::ConstructDetFront()
{
	//Parameters
	G4RotationMatrix* nullRot =  new G4RotationMatrix();

	G4double c_hz = crystal->GetLength()/2;
	G4double c_outRad = crystal->GetWidth()/2;

	G4double c_x = 0*mm;
	G4double c_y = 0*mm;
	G4double c_z = c_hz + gapFront + sThickness;

	//Generate Reflector
	
	G4double rInRad = c_outRad;
	G4double rLength = rThickness + 2*c_hz;
	
	G4LogicalVolume* rLog = GenerateCan(rThickness, rInRad, rLength, rMat, "Reflector");

	G4double r_x = c_x;
	G4double r_y = c_y;
	G4double r_z = c_z - (rThickness/2);

	//Generate Shell
	
	G4double sInRad = c_outRad + gapSide;
	G4double sLength = sThickness + gapFront + 2*c_hz;

	G4LogicalVolume* sLog = GenerateCan(sThickness, sInRad, sLength, sMat, "Shell");

	G4double s_x = c_x;
	G4double s_y = c_y;
	G4double s_z = c_z - (sThickness + gapFront)/2;

	//Generate Assembly
	
	detFront = new G4AssemblyVolume();
	
	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

        //Add Crystal
        Ta.setX(c_x); Ta.setY(c_y); Ta.setZ(c_z);
        Ma = G4Transform3D(Ra, Ta);
        detFront->AddPlacedVolume(crystal->GetCrystal(), Ma);

        //Add Reflector
        Ta.setX(r_x); Ta.setY(r_y); Ta.setZ(r_z);
        Ma = G4Transform3D(Ra, Ta);
        detFront->AddPlacedVolume(rLog, Ma);

        //Add Shell
        Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
        Ma = G4Transform3D(Ra, Ta);
        detFront->AddPlacedVolume(sLog, Ma);
	
	
}

G4double DetectorFront::CalculateLength()
{
	G4double length = crystal->GetLength() + gapFront + sThickness;
	return length;
}

G4double DetectorFront::CalculateWidth()
{
	G4double width = crystal->GetWidth() + gapSide + sThickness;
	return width;
}

G4LogicalVolume* DetectorFront::GenerateCan(G4double thick, G4double inRad,
                                G4double length, G4Material* mat, G4String name)
{
	G4RotationMatrix* nullRot = new G4RotationMatrix();
	
	//Base Parameters
	G4double bInRad = 0.0*mm;
	G4double bOutRad = inRad + thick;
	G4double b_hz = length/2;

	//Base Solid
	G4Tubs* baseSolid = new G4Tubs("baseSolid", bInRad, bOutRad, b_hz, 0*deg, 360*deg);
	
	//Hole Parameters
	G4double hInRad = 0.0*mm;
	G4double hOutRad = inRad;
	G4double h_hz = length/2;

	//Hole Solid
	G4Tubs* holeSolid = new G4Tubs("holeSolid", hInRad, hOutRad, h_hz, 0*deg, 360*deg);

	//Hole Positioning
	G4ThreeVector holeTrans = G4ThreeVector(0*mm, 0*mm, 1*thick);

	//Build Solid
	G4SubtractionSolid* canSolid = new G4SubtractionSolid("canSolid", baseSolid, holeSolid, nullRot, holeTrans);

	//Build Logical Volume
	G4LogicalVolume* canLog = new G4LogicalVolume(canSolid, mat, name);

	return canLog;

	
}


}
