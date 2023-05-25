//====================================
//Detector Construction Implementation
//====================================

#include "DetectorConstruction.hh"
#include "AuxConstructionFunctions.hh"
#include "Detector.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"

#include "G4MultiUnion.hh"
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
{;}

///////////////////////

DetectorConstruction::~DetectorConstruction()
{;}

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

	//Create Al Spherical Shell
	
	//Parameters
	G4double ss_trueRad = 114.3*mm;		
	G4double ss_thickness = 2*mm;

	G4double ss_inRad = ss_trueRad;
	G4double ss_outRad = ss_trueRad + ss_thickness;
	G4double ss_startPhi = 0*rad;
	G4double ss_spanPhi = 2*pi*rad;
	G4double ss_startTheta = 0*rad;
	G4double ss_spanTheta = pi*rad;

	//Solid
	G4Sphere* ssSolid = new G4Sphere("SphericalShell", ss_inRad, ss_outRad,
			ss_startPhi, ss_spanPhi, ss_startTheta, ss_spanTheta);

	//Logical Volume
	
	G4LogicalVolume* ssLog = new G4LogicalVolume(ssSolid, Al, "SphericalShell");

	//Physical Volume Parameters
	G4double ss_x = 0.0*cm;
	G4double ss_y = 0.0*cm;
	G4double ss_z = 0.0*cm;

	//Physical Volume
	
	G4VPhysicalVolume* ssPhys =
       	new G4PVPlacement(0,            	//no rotation
                        G4ThreeVector(ss_x, ss_y, ss_z),
                                        	//translation position
                        ssLog,       		//logical volume
                        "SphericalShell",       //name
                        worldLog,              	//Mother volume
                        false,          	//no bool
                        0);             	//copy number

//======================//
//	Detectors	//
//======================//	

	G4double in = 25.4*mm;

	//Small Detector
	
	//Crystal Parameters
	G4double width = 2*in;
	G4double height = 2*in;

	//Make Crystal
	G4LogicalVolume* sd_c = CreateDetectorCrystal(width, height, CeBr3);
	
	//Assembly Parameters
	G4double reflectorThickness = 0.025*mm;
	G4double shellThickness = 0.5*mm;
	G4double gapSide = 0.5*mm;
	G4double gapFront = 0.5*mm;

	//Make Assembly
	G4AssemblyVolume* sd_detectorAssembly = CreateDetectorAssembly(sd_c, reflectorThickness, 
			shellThickness, gapSide, gapFront, Al, Al);


//New Class Test

	Detector* Detector_2x2 = new Detector();
	

	
	//Detector Parameters//
	
	//Transform
	G4Transform3D Mp;

	//Placement Variables
	G4double dist;		//Distance of detector from origin
       	G4double theta; 	//Polar Angle of detector
	G4double phi; 		//Azimuthal Angle of detector
	G4double spin;		//Rotation of detector along its symmetry axis

	//Lengths
	G4double gap;
	G4double addL;

//===========================================


	//Detector Placement//

	//Detector_0
/*	
	gap = 19.0*mm;
	addL = ld_c_hz + ld_totThickness + gap;

	dist = ss_outRad;
	dist += addL;

	theta = 0*rad;
	phi = 0*rad;

	spin = 0*rad;

	Mp = CreateTransform(dist, theta, phi, spin);

	ld_detectorAssembly->MakeImprint(worldLog, Mp);
*/
	//Detector_1
        
	gap = 14.0*mm;
	//addL = sd_c_hz + sd_totThickness + gap;
	addL = (height)/2 + (shellThickness+gapFront) + gap;	

	dist = ss_outRad;
        dist += addL;

        theta = pi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
	Mp = CreateTransform(dist, theta, phi, spin);

        sd_detectorAssembly->MakeImprint(worldLog, Mp);

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
	SetSensitiveDetector("Logical", detector, true);
	
	//SetSensitiveDetector("ld_CeBr3", detector, true);
}

}
