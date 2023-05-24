//Auxillary Construction Functions

#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"

namespace CeBr3 {


// CreateTransform:
// Takes the placement parameters as input and produces a G4Transform3D object as output.

G4Transform3D DetectorConstruction::CreateTransform(G4double dist, G4double theta, G4double phi, G4double spin)
{

        //Create Transforms
        G4RotationMatrix Rp;
        G4ThreeVector Tp;
        G4Transform3D Mp;

        //Set Parameters

        //Set Translation
        Tp.setX(dist*sin(theta)*cos(phi)); Tp.setY(dist*sin(theta)*sin(phi)); Tp.setZ(dist*cos(theta));

        //Set Rotation
        Rp.rotateZ(spin); Rp.rotateY(theta); Rp.rotateZ(phi);

        //Set Transform
        Mp = G4Transform3D(Rp, Tp);

        return Mp;
}

//Constants:
//G4double in = 25.4*mm;
G4double startAngle = 0*deg;
G4double spanAngle = 360.0*deg;
G4RotationMatrix* nullRot = new G4RotationMatrix;       //Should not rotate


//CreateDetectorCrystal:
// Takes crystal parameters and returns logical volume of crystal.
// (It is necessary to do this seperatly because the sensitive detector assignment in DetectorConstruction requires a logical volume)

G4LogicalVolume* DetectorConstruction::CreateDetectorCrystal(G4double width, G4double height, G4Material* mat)
{
	G4double inRad = 0.0*cm;
	G4double outRad = width/2;
	G4double hz = height/2;

	G4Tubs* Solid = new G4Tubs("Solid", inRad, outRad, hz, startAngle, spanAngle);

	G4LogicalVolume* Log = new G4LogicalVolume(Solid, mat, "Logical");

	return Log;
}

//CreateDetectorAssembly:
// Takes detector parameters and returns detector assembly.

G4AssemblyVolume* DetectorConstruction::CreateDetectorAssembly(G4LogicalVolume* crystal,G4double reflectorThickness, 
	G4double shellThickness, G4double gapSide, G4double gapFront, G4Material* rMat, G4Material* sMat)
{

	//Crystal Parameters
	G4double c_inRad = 0.0*cm;
	G4double c_outRad = dynamic_cast<G4Tubs*>(crystal->GetSolid())->GetOuterRadius();
	G4double c_hz = dynamic_cast<G4Tubs*>(crystal->GetSolid())->GetZHalfLength();

	G4double c_x = 0.0*cm;
	G4double c_y = 0.0*cm;
	G4double c_z = 0.0*cm;
	
	//================//
	//Create Reflector//
	//================//
	G4double rThickness = reflectorThickness;
		

	//Create Base
	G4double rB_inRad = c_inRad;
	G4double rB_outRad = c_inRad+rThickness;
	G4double rB_hz = c_hz + rThickness/2;

	//Base Solid
	G4Tubs* rB = new G4Tubs("rB", rB_inRad, rB_outRad, rB_hz, startAngle, spanAngle);

	//Create Hole
	G4double rH_inRad = c_inRad;
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

	//============//
	//Create Shell//
	//============//
	
	//Derived Parameters
	G4double totSideThickness = (shellThickness + gapSide);
	G4double totFrontThickness = (shellThickness + gapFront);

	//Create Base
	G4double sB_inRad = c_inRad;
	G4double sB_outRad = c_outRad + totSideThickness;
	G4double sB_hz = c_hz + totFrontThickness/2;

	//Base Solid
	G4Tubs* sB = new G4Tubs("sB", sB_inRad, sB_outRad, sB_hz, startAngle, spanAngle);

	//Create Hole
	G4double sH_inRad = c_inRad;
	G4double sH_outRad = c_outRad + gapFront;
	G4double sH_hz = sB_hz;

	//Hole Solid
	G4Tubs* sH = new G4Tubs("sH", sH_inRad, sH_outRad, sH_hz, startAngle, spanAngle);

	//Hole Position
	G4ThreeVector sHoleTrans(0, 0, 1*shellThickness);

	//Build Shell Solid
	G4SubtractionSolid* sSolid = new G4SubtractionSolid("sSolid", sB, sH, nullRot, sHoleTrans);

	//Shell Logical Volume
	G4LogicalVolume* sLog = new G4LogicalVolume(sSolid, sMat, "sLog");

	//Positioning
	G4double s_x = c_x;
	G4double s_y = c_y;
	G4double s_z = c_z - (totFrontThickness/2);

	//===============//
	//Create Assembly//
	//===============//
	
	//Detector Assembly
        G4AssemblyVolume* detectorAssembly = new G4AssemblyVolume;

        //Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

        //Add Crystal
        Ta.setX(c_x); Ta.setY(c_y); Ta.setZ(c_z);
        Ma = G4Transform3D(Ra, Ta);
        detectorAssembly->AddPlacedVolume(crystal, Ma);

        //Add Reflector
        Ta.setX(r_x); Ta.setY(r_y); Ta.setZ(r_z);
        Ma = G4Transform3D(Ra, Ta);
        detectorAssembly->AddPlacedVolume(rLog, Ma);

        //Add Shell
        Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
        Ma = G4Transform3D(Ra, Ta);
        detectorAssembly->AddPlacedVolume(sLog, Ma);

	return detectorAssembly;

}

}
