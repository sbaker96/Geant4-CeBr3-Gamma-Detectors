//====================================
//Detector Construction Implementation
//====================================

#include "DetectorConstruction.hh"
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

#include "CADMesh.hh"

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

	G4double in = 25.4*mm;

	//Create Al Spherical Shell
	
	//Parameters
	G4double ss_trueRad = 4.20*in;		
	G4double ss_thickness = 2.00*mm;

	G4double ss_inRad = ss_trueRad;
	G4double ss_outRad = ss_trueRad + ss_thickness;
	G4double ss_startPhi = 0*rad;
	G4double ss_spanPhi = 2*pi*rad;
	G4double ss_startTheta = 0*rad;
	G4double ss_spanTheta = pi*rad;
	
	auto ss_mesh = CADMesh::TessellatedMesh::FromSTL("./SphericalShell.stl");

	auto ssSolid = ss_mesh->GetSolid();
/*
	//Solid
	G4Sphere* ssSolid = new G4Sphere("SphericalShell", ss_inRad, ss_outRad,
			ss_startPhi, ss_spanPhi, ss_startTheta, ss_spanTheta);
*/
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




//=============================================

//======================//
//	Detectors	//
//======================//	

	//Construction Parameters
	G4double width,		//width of detector crystal
		 length, 	//length of detector crystal
		 rThickness, 	//thickness of reflector
		 sThickness,	//thickness of shell
		 gapFront, 	//gap between shell and crystal front
		 gapSide;	//gap between shell and crystal side
	
	G4String name;		//Name of crystal logical volume

	//Constants

	//2x2 Detector//

	Detector* Detector_2x2 = new Detector();
	
	//Construct Crystal
	width = 2*in;
	length = 2*in;
	name = "2x2_Crystal";
	
	Detector_2x2->SetWidth(width);
	Detector_2x2->SetLength(length);
	Detector_2x2->SetName(name);
	Detector_2x2->SetCMat(CeBr3);

	Detector_2x2->ConstructCrystal();

	//Construct Detector
	rThickness = 0.025*mm;
	sThickness = 0.5*mm;
	gapFront = 0.5*mm;
	gapSide = 0.5*mm;
	
	Detector_2x2->SetRThickness(rThickness);
	Detector_2x2->SetSThickness(sThickness);
	Detector_2x2->SetGapFront(gapFront);
	Detector_2x2->SetGapSide(gapSide);
	Detector_2x2->SetRMat(Al);
	Detector_2x2->SetSMat(Al);

	Detector_2x2->ConstructDetector();

	//3x4 Detector//

	Detector* Detector_3x4 = new Detector();
	
	//Construct Crystal
	width = 3*in;
	length = 4*in;
	name = "3x4_Crystal";
	
	Detector_3x4->SetWidth(width);
	Detector_3x4->SetLength(length);
	Detector_3x4->SetName(name);
	Detector_3x4->SetCMat(CeBr3);

	Detector_3x4->ConstructCrystal();

	//Construct Detector
	rThickness = 0.025*mm;
	sThickness = 0.5*mm;
	gapFront = 0.5*mm;
	gapSide = 2.4*mm;
	
	Detector_3x4->SetRThickness(rThickness);
	Detector_3x4->SetSThickness(sThickness);
	Detector_3x4->SetGapFront(gapFront);
	Detector_3x4->SetGapSide(gapSide);
	Detector_3x4->SetRMat(Al);
	Detector_3x4->SetSMat(Al);

	Detector_3x4->ConstructDetector();


//===========================================
	
	//Placement Parameters//
	
	//Transform
	G4Transform3D Mp;

	//Placement Variables
	G4double gap;		//Distance of detector from apparatus
       	G4double theta; 	//Polar Angle of detector
	G4double phi; 		//Azimuthal Angle of detector
	G4double spin;		//Rotation of detector along its symmetry axis

	//Detector Placement//

	//Detector 0
/*
	gap = 14.0*mm;

        theta = pi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
	Detector_2x2->PlaceDetector(worldLog, ss_outRad, gap, theta, phi, spin);
*/
	//Detector 1
	
	gap = 19.0*mm;

	theta = 0*rad;
	phi = 0*rad;

	spin = 0*rad;

	Detector_3x4->PlaceDetector(worldLog, ss_outRad, gap, theta, phi, spin);

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
	
	//Note: Currently, as the detector building takes place
	// in a seperate function, the names of the logical volumes
	// of the crystals must be manually set here.
	
	SetSensitiveDetector("2x2_Crystal", detector, true);
	
	SetSensitiveDetector("3x4_Crystal", detector, true);
}

}
