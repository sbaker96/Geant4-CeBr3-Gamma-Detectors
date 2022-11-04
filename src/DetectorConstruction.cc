//====================================
//Detector Construction Implementation
//====================================

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "TrackerSD.hh"
#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4Transform3D.hh"

namespace CeBr3
{
//////////////////////

DetectorConstruction::DetectorConstruction()
{}

///////////////////////

DetectorConstruction::~DetectorConstruction()
{}

////////////////////////
G4VPhysicalVolume* DetectorConstruction::Construct()
{
        //Get nist Material Manager
        G4NistManager* man  = G4NistManager::Instance();

	//Define nist Materials
        G4Material* Air = man->FindOrBuildMaterial("G4_AIR");
        G4Material* Al = man->FindOrBuildMaterial("G4_Al");

	//Define CeBr3

	G4Element*  Ce = new G4Element( "Cerium", "Ce", 58., 140.116*g/mole);
	G4Element* Br = new G4Element( "Bromine", "Br", 35., 79.904*g/mole);
	
	G4Material* CeBr3 = new G4Material("CeBr3", 	//name
					   5.1*g/cm3,	//density
					   2);		//ncomponents

	CeBr3->AddElement(Ce, 36.889*perCent);
       	CeBr3->AddElement(Br, 63.111*perCent);

//======================================

	//Define World Parameters
        G4double world_hx = 5.0*m;
        G4double world_hy = 5.0*m;
        G4double world_hz = 5.0*m;

        //Create Solid World
        G4Box* worldSolid  = new G4Box("World", world_hx, world_hy, world_hz);

        //Create Logical World
        G4LogicalVolume* worldLog = new G4LogicalVolume(worldSolid, Air, "World");

        //Define Physical World Parameters
        G4double pos_x = 0.0*m;
        G4double pos_y = 0.0*m;
        G4double pos_z = 0.0*m;

        //Create Physical World
        G4VPhysicalVolume* worldPhys =
        new G4PVPlacement(0,            //no rotation
                        G4ThreeVector(pos_x, pos_y, pos_z),
                                        //translation position
                        worldLog,       //logical volume
                        "World",        //name
                        0,              //Mother volume
                        false,          //no bool
                        0);             //copy number

//============================================

//Create CeBr3 Crystal

        //Parameters
        G4double c_inRad = 0.0*cm;
        G4double c_outRad = 25.5*mm;
        G4double c_hz = 25.5*mm;
        G4double c_startAngle = 0.0*deg;
        G4double c_spanAngle = 360.0*deg;

        //Solid
        G4Tubs* cSolid = new G4Tubs("CeBr3", c_inRad, c_outRad,
                        c_hz, c_startAngle, c_spanAngle);
        //Logical Volume
        G4LogicalVolume* cLog = new G4LogicalVolume(cSolid, CeBr3, "CeBr3");

        //Physical Volume Parameters
        G4double c_x = 0.0*cm;
        G4double c_y = 0.0*cm;
        G4double c_z = 0.0*cm; //!!!!!!!!!!

//==============================================
	//Create Reflector

	
        //Base Parameters
        
	G4double rThickness = 0.01*mm;
        
	G4double rBase_outRad = c_outRad+rThickness;
        G4double rBase_inRad = c_inRad;
        G4double rBase_hz = c_hz+rThickness/2;
        G4double rBase_startAngle = 0.0*deg;
        G4double rBase_spanAngle = 360*deg;

        //Base Solid
        G4Tubs* rBase = new G4Tubs("rBase", rBase_inRad, rBase_outRad,
                        rBase_hz, rBase_startAngle, rBase_spanAngle);

        //Hole Parameters
        G4double rHole_outRad = c_outRad;
        G4double rHole_inRad = c_inRad;
        G4double rHole_hz = rBase_hz;
        G4double rHole_startAngle = rBase_startAngle;
        G4double rHole_spanAngle = rBase_spanAngle;

        //Hole Solid
        G4Tubs* rHole = new G4Tubs("Hole", rHole_inRad, rHole_outRad,
                        rHole_hz, rHole_startAngle, rHole_spanAngle);

        //Hole Positioning
        G4RotationMatrix* nullRot = new G4RotationMatrix;       //Should not rotate
        G4ThreeVector rHoleTrans(0, 0, 1* rThickness);

        //Build Shell Solid
        G4SubtractionSolid* rSolid =
                new G4SubtractionSolid("Reflector Shell", rBase, rHole, nullRot, rHoleTrans);

        //Reflector Logical Volume
        G4LogicalVolume* rLog  = new G4LogicalVolume(rSolid, Al, "Reflector");

        //Physical Volume Parameters
        G4double r_x = 0.0*m;
        G4double r_y = 0.0*m;
        G4double r_z = c_z-(rThickness/2); 
                                                                         

//==============================================
        //Create Al Shell


        //Base Shell Parameters

        G4double sThickness = 0.5*mm;
	G4double gapThickness = 1.0*mm;

	G4double totThickness = sThickness + gapThickness;

        G4double sBase_outRad = c_outRad+totThickness;
        G4double sBase_inRad = c_inRad;
        G4double sBase_hz = c_hz+totThickness/2;
        G4double sBase_startAngle = 0.0*deg;
        G4double sBase_spanAngle = 360*deg;

        //Base Shell Solid
        G4Tubs* sBase = new G4Tubs("Base Shell", sBase_inRad, sBase_outRad,
                        sBase_hz, sBase_startAngle, sBase_spanAngle);

        //Hole Parameters
        G4double sHole_outRad = c_outRad + gapThickness;
        G4double sHole_inRad = c_inRad;
        G4double sHole_hz = sBase_hz;
        G4double sHole_startAngle = sBase_startAngle;
        G4double sHole_spanAngle = sBase_spanAngle;

        //Hole Solid
        G4Tubs* sHole = new G4Tubs("Shell Hole", sHole_inRad, sHole_outRad,
                        sHole_hz, sHole_startAngle, sHole_spanAngle);

        //Hole Positioning
        G4ThreeVector sHoleTrans(0, 0, 1*sThickness );

        //Build Shell Solid
        G4SubtractionSolid* sSolid =
                new G4SubtractionSolid("Shell", sBase, sHole, nullRot, sHoleTrans);

        //Shell Logical Volume
        G4LogicalVolume* sLog  = new G4LogicalVolume(sSolid, Al, "Shell");

        //Physical Volume Parameters
        G4double s_x = 0.0*m;
        G4double s_y = 0.0*m;
        G4double s_z = c_z-(totThickness/2);

//===========================================

	//Detector Assembly
	G4AssemblyVolume* detectorAssembly = new G4AssemblyVolume;

	//Rotation and Translation Matrices
	G4RotationMatrix Ra;
	G4ThreeVector Ta;

	G4Transform3D Ma;

	//Add Crystal
	Ta.setX(c_x); Ta.setY(c_y); Ta.setZ(c_z);
	Ma = G4Transform3D(Ra, Ta);
	detectorAssembly->AddPlacedVolume(cLog, Ma);

	//Add Reflector
        Ta.setX(r_x); Ta.setY(r_y); Ta.setZ(r_z);
	Ma = G4Transform3D(Ra, Ta);
        detectorAssembly->AddPlacedVolume(rLog, Ma);
	
	//Add Shell
        Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
	Ma = G4Transform3D(Ra, Ta);
        detectorAssembly->AddPlacedVolume(sLog, Ma);

//==========================================
	
	//Place Detectors
	
	//Transform
	G4Transform3D Mp;

	//Placement Variables
	G4double dist; G4double theta; G4double phi; G4double spin;

	//Lengths
	G4double addL = c_hz + totThickness;

	//Detector_0

	dist = 114.3*mm;
	dist += addL;

	theta = 0*rad;
	phi = 0*rad;

	spin = 0*rad;

	Mp = CreateTransform(dist, theta, phi, spin);

	detectorAssembly->MakeImprint(worldLog, Mp);

	//Detector_1
        
	dist = 114.3*mm;
        dist += addL;

        theta = pi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
	Mp = CreateTransform(dist, theta, phi, spin);

        detectorAssembly->MakeImprint(worldLog, Mp);



//===========================================


	//Return
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
	return worldPhys;	
}


void DetectorConstruction::ConstructSDandField()
{

        //Sensitive Detectors

	auto sdman = G4SDManager::GetSDMpointer();
	
	G4String trackerSDname;

        trackerSDname = "/Detector";

        TrackerSD* detector = new TrackerSD(trackerSDname, "TrackerHitsCollection");

	sdman->AddNewDetector(detector);
        
	//Set Sensitive Detector
	SetSensitiveDetector("CeBr3", detector, true);
	
}

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


}
