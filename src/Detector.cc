//===============================
//Detector Implementation
//===============================

#include "Detector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

namespace CeBr3
{

Detector::Detector()
{;}

Detector::~Detector()
{;}

void Detector::ConstructCrystal()
{
	G4double inRad = 0*mm;
	G4double outRad = width/2;
	G4double c_hz = length/2;

	G4Tubs* solid = new G4Tubs("Solid", inRad, outRad, c_hz, 0*deg, 360*deg);

	crystal = new G4LogicalVolume(solid, cMat, name);
}

void Detector::ConstructDetector()
{
	//Constants
	G4double inRad = 0*mm;
	G4double startAngle = 0*deg;
	G4double spanAngle = 360*deg;
	G4RotationMatrix* nullRot = new G4RotationMatrix;

	//Crystal Parameters
	G4double c_outRad = width/2;
	G4double c_hz = length/2;

	G4double c_x = 0*mm;
	G4double c_y = 0*mm;
	G4double c_z = 0*mm;

	//Reflector//
	
	//Create Base
        G4double rB_inRad = inRad;
        G4double rB_outRad = inRad+rThickness;
        G4double rB_hz = c_hz + rThickness/2;

        //Base Solid
        G4Tubs* rB = new G4Tubs("rB", rB_inRad, rB_outRad, rB_hz, startAngle, spanAngle);

        //Create Hole
        G4double rH_inRad = inRad;
        G4double rH_outRad = c_outRad;
        G4double rH_hz = rB_hz;

        //Hole Solid
        G4Tubs* rH = new G4Tubs("rH", rH_inRad, rH_outRad, rH_hz, startAngle, spanAngle);

        //Hole Position
        G4ThreeVector rHoleTrans(0, 0, 1*rThickness);

        //Build Reflector Solid
        G4SubtractionSolid* rSolid = new G4SubtractionSolid("rSolid", rB, rH, nullRot, rHoleTrans);

        //Reflector Logical Volume
        G4LogicalVolume* rLog = new G4LogicalVolume(rSolid, rMat, "rLog");

        //Positioning
        G4double r_x = c_x;
        G4double r_y = c_y;
        G4double r_z = c_z - (rThickness/2);

	//Shell//
	
	//Derived Parameters
        G4double totSideThickness = (sThickness + gapSide);
        G4double totFrontThickness = (sThickness + gapFront);

        //Create Base
        G4double sB_inRad = inRad;
        G4double sB_outRad = c_outRad + totSideThickness;
        G4double sB_hz = c_hz + totFrontThickness/2;

        //Base Solid
        G4Tubs* sB = new G4Tubs("sB", sB_inRad, sB_outRad, sB_hz, startAngle, spanAngle);

        //Create Hole
        G4double sH_inRad = inRad;
        G4double sH_outRad = c_outRad + gapFront;
        G4double sH_hz = sB_hz;

        //Hole Solid
        G4Tubs* sH = new G4Tubs("sH", sH_inRad, sH_outRad, sH_hz, startAngle, spanAngle);

        //Hole Position
        G4ThreeVector sHoleTrans(0, 0, 1*sThickness);

        //Build Shell Solid
        G4SubtractionSolid* sSolid = new G4SubtractionSolid("sSolid", sB, sH, nullRot, sHoleTrans);

        //Shell Logical Volume
        G4LogicalVolume* sLog = new G4LogicalVolume(sSolid, sMat, "sLog");

        //Positioning
        G4double s_x = c_x;
        G4double s_y = c_y;
        G4double s_z = c_z - (totFrontThickness/2);

	//Assembly//
	
	detector = new G4AssemblyVolume;
	
	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

        //Add Crystal
        Ta.setX(c_x); Ta.setY(c_y); Ta.setZ(c_z);
        Ma = G4Transform3D(Ra, Ta);
        detector->AddPlacedVolume(crystal, Ma);

        //Add Reflector
        Ta.setX(r_x); Ta.setY(r_y); Ta.setZ(r_z);
        Ma = G4Transform3D(Ra, Ta);
        detector->AddPlacedVolume(rLog, Ma);

        //Add Shell
        Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
        Ma = G4Transform3D(Ra, Ta);
        detector->AddPlacedVolume(sLog, Ma);


}

G4double Detector::GetOffset()
{
	G4double offset = length/2 + sThickness + gapFront;

	return offset;

}

}
