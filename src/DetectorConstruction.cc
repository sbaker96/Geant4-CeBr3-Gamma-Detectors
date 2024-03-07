//====================================
//Detector Construction Implementation
//====================================

#include "DetectorConstruction.hh"

#include "DetectorCrystal.hh"
#include "DetectorFront.hh"
#include "DetectorPMT.hh"
#include "DetectorBack.hh"
#include "DetectorAssembly.hh"

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
	G4Material* Glass = man->FindOrBuildMaterial("G4_Pyrex_Glass");
	G4Material* Plastic = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	G4Material* Styrofoam = man->FindOrBuildMaterial("G4_POLYSTYRENE");

	//Define CeBr3

	G4Element*  Ce = new G4Element( "Cerium", "Ce", 58., 140.116*g/mole);
	G4Element* Br = new G4Element( "Bromine", "Br", 35., 79.904*g/mole);
	
	G4Material* CeBr3 = new G4Material("CeBr3", 	//name
					   5.1*g/cm3,	//density
					   2);		//ncomponents

	CeBr3->AddElement(Ce, 1);
       	CeBr3->AddElement(Br, 3);

	//Define Bialkali
	
	G4Element* Sb = new G4Element("Antimony", "Sb", 51., 121.760*g/mole);
	G4Element* K  = new G4Element("Potassium", "K", 19., 39.098*g/mole);
	G4Element* Cs = new G4Element("Cesium", "Cs", 55., 132.91*g/mole);

	G4Material* Bialkali = new G4Material("Bialkali", 3.46*g/cm3, 3);

	Bialkali->AddElement(Sb, 1);
	Bialkali->AddElement(K, 2);
	Bialkali->AddElement(Cs, 1);

	//Define Mu Metal
	
	G4Element* Ni = new G4Element("Nickel", "Ni", 28., 58.693*g/mole);
	G4Element* Fe = new G4Element("Iron", "Fe", 26., 55.845*g/mole);
	G4Element* Mo = new G4Element("Molybdenum", "Mo", 42., 95.95*g/mole);

	G4Material* MuMetal = new G4Material("MuMetal", 8.7*g/cm3, 3);

	MuMetal->AddElement(Ni, 80*perCent);
	MuMetal->AddElement(Fe, 15*perCent);
	MuMetal->AddElement(Mo, 5*perCent);

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
	G4double ss_trueRad = 4.05*in;		
	G4double ss_thickness = 1.75*mm;

	G4double ss_inRad = ss_trueRad;
	G4double ss_outRad = ss_trueRad + ss_thickness;
	G4double ss_startPhi = 0*rad;
	G4double ss_spanPhi = 2*pi*rad;
	G4double ss_startTheta = 0*rad;
	G4double ss_spanTheta = pi*rad;
/*	
	auto ss_mesh = CADMesh::TessellatedMesh::FromSTL("./SphericalShell.stl");

	auto ssSolid = ss_mesh->GetSolid();
*/
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
/*	
	G4VPhysicalVolume* ssPhys =
       	new G4PVPlacement(0,            	//no rotation
                        G4ThreeVector(ss_x, ss_y, ss_z),
                                        	//translation position
                        ssLog,       		//logical volume
                        "SphericalShell",       //name
                        worldLog,              	//Mother volume
                        false,          	//no bool
                        0);             	//copy number


*/


//=============================================

//Create Source Plastic

	//Parameters
	G4double sp_trueRad = 2*cm;
	G4double sp_thickRad = 0.5*cm;
	G4double sp_trueHeight = 1*cm;
	G4double sp_thickHeight = 0.5*cm;

	G4double sp_startAngle = 0*rad;
	G4double sp_spanAngle = 2*pi*rad;

	G4RotationMatrix* nullRot = new G4RotationMatrix();
	G4RotationMatrix* placeRot = new G4RotationMatrix();
		placeRot->rotateX(90*deg);
	G4ThreeVector nullVec = G4ThreeVector(0*cm, 0*cm, 0*cm);

	//Base Solid
	G4Tubs* spBaseSolid = new G4Tubs("spBase", 0*cm, sp_trueRad, sp_trueHeight/2, 
			sp_startAngle, sp_spanAngle);
	//Hole Solid
	G4Tubs* spHoleSolid = new G4Tubs("spHole", 0*cm, (sp_trueRad - sp_thickRad), 
			(sp_trueHeight - sp_thickHeight)/2, sp_startAngle, sp_spanAngle);

	//Source Plastic Solid
	G4SubtractionSolid* spSolid = new G4SubtractionSolid("spSolid", spBaseSolid,
			spHoleSolid, nullRot, nullVec);
	
	//Logical Volume
	G4LogicalVolume* spLog = new G4LogicalVolume(spSolid, Plastic, "SourcePlastic");

	//Physical Volume
	G4VPhysicalVolume* spPhys = new G4PVPlacement(placeRot, nullVec, spLog, "SourcePlastic", worldLog,
		false, 0);	


//=============================================

//======================//
//	Detectors	//
//======================//	

	//Detector Front Construction Parameters
        G4double cWidth,   	//width of detector crystal
                 cLength,       //length of detector crystal
                 refThick,    	//thickness of reflector
                 shellThick,    //thickness of shell
                 shellFrontGap, //gap between shell and crystal front
                 shellSideGap;  //gap between shell and crystal side
        
        G4String name;          //Name of crystal logical volume

	//Detector Back Construction Parameters
	G4double pmtWidth,	//width of PMT
		 pmtLength,	//length of PMT
		 pmtThick,	//thickness of PMT glass
		 cathodeThick,	//thickness of photocathode
		 shieldSideGap,	//gap between magnetic shield and PMT
		 shieldThick;	//thickness of magnetic shield


	//2x2 Detector
	
	
	//Construct Crystal
	cWidth = 2*in;
	cLength = 2*in;
	name = "2x2_Crystal";
	
	DetectorCrystal* crystal_2x2 = new DetectorCrystal();
	
	crystal_2x2->SetWidth(cWidth);
	crystal_2x2->SetLength(cLength);
	crystal_2x2->SetName(name);
	crystal_2x2->SetMaterial(CeBr3);

	crystal_2x2->ConstructCrystal();

	//Construct Detector Front
	refThick = 0.025*mm;
	shellThick = 0.5*mm;
	shellFrontGap = 0.5*mm;
	shellSideGap = 0.5*mm;

	DetectorFront* detFront_2x2 = new DetectorFront();

	detFront_2x2->SetRThickness(refThick);
	detFront_2x2->SetSThickness(shellThick);
	detFront_2x2->SetGapFront(shellFrontGap);
	detFront_2x2->SetGapSide(shellSideGap);

	detFront_2x2->SetRMat(Al);
	detFront_2x2->SetSMat(Al);

	detFront_2x2->SetCrystal(crystal_2x2);

	detFront_2x2->ConstructDetFront();

	//Construct PMT
	pmtWidth = 2*in;
	pmtLength = 90*mm;
	pmtThick = 5*mm;
	cathodeThick = 1*mm;

	DetectorPMT* pmt_2x2 = new DetectorPMT();

	pmt_2x2->SetWidth(pmtWidth);
	pmt_2x2->SetLength(pmtLength);
	pmt_2x2->SetThickness(pmtThick);
	pmt_2x2->SetCathodeThick(cathodeThick);

	pmt_2x2->SetGlassMat(Glass);
	pmt_2x2->SetGasMat(Air);
	pmt_2x2->SetCathodeMat(Bialkali);

	pmt_2x2->ConstructPMT();

	//Construct Detector Back
	shieldSideGap = 5*mm;
	shieldThick = 0.64*mm;

	DetectorBack* detBack_2x2 = new DetectorBack();

	detBack_2x2->SetSideGap(shieldSideGap);
	detBack_2x2->SetShieldThickness(shieldThick);

	detBack_2x2->SetShieldMat(MuMetal);

	detBack_2x2->SetPMT(pmt_2x2);

	detBack_2x2->ConstructDetBack();

	//Construct Detector Assembly
	
	DetectorAssembly* Detector_2x2 = new DetectorAssembly();

	Detector_2x2->SetDetFront(detFront_2x2);
	Detector_2x2->SetDetBack(detBack_2x2);

	Detector_2x2->ConstructAssembly();

	//3x4 Detector
	
	
	//Construct Crystal
	cWidth = 3*in;
	cLength = 4*in;
	name = "3x4_Crystal";
	
	DetectorCrystal* crystal_3x4 = new DetectorCrystal();
	
	crystal_3x4->SetWidth(cWidth);
	crystal_3x4->SetLength(cLength);
	crystal_3x4->SetName(name);
	crystal_3x4->SetMaterial(CeBr3);

	crystal_3x4->ConstructCrystal();

	//Construct Detector Front
	refThick = 0.025*mm;
	shellThick = 0.5*mm;
	shellFrontGap = 0.5*mm;
	shellSideGap = 0.5*mm;

	DetectorFront* detFront_3x4 = new DetectorFront();

	detFront_3x4->SetRThickness(refThick);
	detFront_3x4->SetSThickness(shellThick);
	detFront_3x4->SetGapFront(shellFrontGap);
	detFront_3x4->SetGapSide(shellSideGap);

	detFront_3x4->SetRMat(Al);
	detFront_3x4->SetSMat(Al);

	detFront_3x4->SetCrystal(crystal_3x4);

	detFront_3x4->ConstructDetFront();

	//Construct PMT
	pmtWidth = 3*in;
	pmtLength = 90*mm;
	pmtThick = 5*mm;
	cathodeThick = 1*mm;

	DetectorPMT* pmt_3x4 = new DetectorPMT();

	pmt_3x4->SetWidth(pmtWidth);
	pmt_3x4->SetLength(pmtLength);
	pmt_3x4->SetThickness(pmtThick);
	pmt_3x4->SetCathodeThick(cathodeThick);

	pmt_3x4->SetGlassMat(Glass);
	pmt_3x4->SetGasMat(Air);
	pmt_3x4->SetCathodeMat(Bialkali);

	pmt_3x4->ConstructPMT();

	//Construct Detector Back
	shieldSideGap = 5*mm;
	shieldThick = 0.64*mm;

	DetectorBack* detBack_3x4 = new DetectorBack();

	detBack_3x4->SetSideGap(shieldSideGap);
	detBack_3x4->SetShieldThickness(shieldThick);

	detBack_3x4->SetShieldMat(MuMetal);

	detBack_3x4->SetPMT(pmt_3x4);

	detBack_3x4->ConstructDetBack();

	//Construct Detector Assembly
	
	DetectorAssembly* Detector_3x4 = new DetectorAssembly();

	Detector_3x4->SetDetFront(detFront_3x4);
	Detector_3x4->SetDetBack(detBack_3x4);

	Detector_3x4->ConstructAssembly();


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

//	gap = //14.0*mm;
	gap = 5.0*in - ss_outRad;

        theta = pi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
//	Detector_2x2->PlaceAssembly(worldLog, ss_outRad, gap, theta, phi, spin);

	//Detector 1
	
//	gap = 19.0*mm;
	gap = 5.0*in - ss_outRad;

	theta = 0*rad;
	phi = 0*rad;

	spin = 0*rad;

	Detector_3x4->PlaceAssembly(worldLog, ss_outRad, gap, theta, phi, spin);

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
