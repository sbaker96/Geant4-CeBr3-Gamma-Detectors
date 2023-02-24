//====================================
//Detector Construction Implementation
//====================================

#include "DetectorConstruction.hh"

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
	G4Material* Concrete = man->FindOrBuildMaterial("G4_CONCRETE");

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
	G4double ss_thickness = 4.0*mm;

	G4double ss_inRad = ss_trueRad - ss_thickness;
	G4double ss_outRad = ss_trueRad;
	G4double ss_startPhi = 0*rad;
	G4double ss_spanPhi = 2*pi*rad;
	G4double ss_startTheta = 0*rad;
	G4double ss_spanTheta = pi*rad;

	//Solid
	G4Sphere* ssSolid = new G4Sphere("Spherical Shell", ss_inRad, ss_outRad,
			ss_startPhi, ss_spanPhi, ss_startTheta, ss_spanTheta);

	//Logical Volume
	
	G4LogicalVolume* ssLog = new G4LogicalVolume(ssSolid, Al, "Spherical Shell");

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
                        "Spherical Shell",      //name
                        worldLog,              	//Mother volume
                        false,          	//no bool
                        0);             	//copy number

	


//=============================================

//===========================================//
//		Small Detector		     //
//===========================================//

	//Create CeBr3 Crystal

        //Parameters
        G4double sd_c_inRad = 0.0*cm;
        G4double sd_c_outRad = 25.5*mm;
        G4double sd_c_hz = 25.5*mm;
        G4double sd_c_startAngle = 0.0*deg;
        G4double sd_c_spanAngle = 360.0*deg;

        //Solid
        G4Tubs* sd_cSolid = new G4Tubs("sd_CeBr3", sd_c_inRad, sd_c_outRad,
                        sd_c_hz, sd_c_startAngle, sd_c_spanAngle);
        //Logical Volume
        G4LogicalVolume* sd_cLog = new G4LogicalVolume(sd_cSolid, CeBr3, "sd_CeBr3");

        //Physical Volume Parameters
        G4double sd_c_x = 0.0*cm;
        G4double sd_c_y = 0.0*cm;
        G4double sd_c_z = 0.0*cm;

//------------------------------------------------
	
	//Create Reflector

	
        //Base Parameters
        
	G4double sd_rThickness = 0.01*mm;
        
	G4double sd_rBase_outRad = sd_c_outRad+sd_rThickness;
        G4double sd_rBase_inRad = sd_c_inRad;
        G4double sd_rBase_hz = sd_c_hz+sd_rThickness/2;
        G4double sd_rBase_startAngle = 0.0*deg;
        G4double sd_rBase_spanAngle = 360*deg;

        //Base Solid
        G4Tubs* sd_rBase = new G4Tubs("sd_rBase", sd_rBase_inRad, sd_rBase_outRad,
                        sd_rBase_hz, sd_rBase_startAngle, sd_rBase_spanAngle);

        //Hole Parameters
        G4double sd_rHole_outRad = sd_c_outRad;
        G4double sd_rHole_inRad = sd_c_inRad;
        G4double sd_rHole_hz = sd_rBase_hz;
        G4double sd_rHole_startAngle = sd_rBase_startAngle;
        G4double sd_rHole_spanAngle = sd_rBase_spanAngle;

        //Hole Solid
        G4Tubs* sd_rHole = new G4Tubs("sd_Hole", sd_rHole_inRad, sd_rHole_outRad,
                        sd_rHole_hz, sd_rHole_startAngle, sd_rHole_spanAngle);

        //Hole Positioning
        G4RotationMatrix* nullRot = new G4RotationMatrix;       //Should not rotate
        G4ThreeVector sd_rHoleTrans(0, 0, 1* sd_rThickness);

        //Build Shell Solid
        G4SubtractionSolid* sd_rSolid =
                new G4SubtractionSolid("sd_Reflector_Shell", sd_rBase, sd_rHole, nullRot, sd_rHoleTrans);

        //Reflector Logical Volume
        G4LogicalVolume* sd_rLog  = new G4LogicalVolume(sd_rSolid, Al, "sd_Reflector");

        //Physical Volume Parameters
        G4double sd_r_x = 0.0*m;
        G4double sd_r_y = 0.0*m;
        G4double sd_r_z = sd_c_z-(sd_rThickness/2); 
                                                                         

//-------------------------------------------------------
        
	//Create Al Shell


        //Base Shell Parameters

        G4double sd_sThickness = 0.5*mm;
	G4double sd_gapThickness = 1.0*mm;

	G4double sd_totThickness = sd_sThickness + sd_gapThickness;

        G4double sd_sBase_outRad = sd_c_outRad+sd_totThickness;
        G4double sd_sBase_inRad = sd_c_inRad;
        G4double sd_sBase_hz = sd_c_hz+sd_totThickness/2;
        G4double sd_sBase_startAngle = 0.0*deg;
        G4double sd_sBase_spanAngle = 360*deg;

        //Base Shell Solid
        G4Tubs* sd_sBase = new G4Tubs("sd_Base_Shell", sd_sBase_inRad, sd_sBase_outRad,
                        sd_sBase_hz, sd_sBase_startAngle, sd_sBase_spanAngle);

        //Hole Parameters
        G4double sd_sHole_outRad = sd_c_outRad + sd_gapThickness;
        G4double sd_sHole_inRad = sd_c_inRad;
        G4double sd_sHole_hz = sd_sBase_hz;
        G4double sd_sHole_startAngle = sd_sBase_startAngle;
        G4double sd_sHole_spanAngle = sd_sBase_spanAngle;

        //Hole Solid
        G4Tubs* sd_sHole = new G4Tubs("sd_Shell_Hole", sd_sHole_inRad, sd_sHole_outRad,
                        sd_sHole_hz, sd_sHole_startAngle, sd_sHole_spanAngle);

        //Hole Positioning
        G4ThreeVector sd_sHoleTrans(0, 0, 1*sd_sThickness );

        //Build Shell Solid
        G4SubtractionSolid* sd_sSolid =
                new G4SubtractionSolid("sd_Shell", sd_sBase, sd_sHole, nullRot, sd_sHoleTrans);

        //Shell Logical Volume
        G4LogicalVolume* sd_sLog  = new G4LogicalVolume(sd_sSolid, Al, "sd_Shell");

        //Physical Volume Parameters
        G4double sd_s_x = 0.0*m;
        G4double sd_s_y = 0.0*m;
        G4double sd_s_z = sd_c_z-(sd_totThickness/2);

//------------------------------------------------------------

	//Detector Assembly
	G4AssemblyVolume* sd_detectorAssembly = new G4AssemblyVolume;

	//Rotation and Translation Matrices
	G4RotationMatrix Ra;
	G4ThreeVector Ta;

	G4Transform3D Ma;

	//Add Crystal
	Ta.setX(sd_c_x); Ta.setY(sd_c_y); Ta.setZ(sd_c_z);
	Ma = G4Transform3D(Ra, Ta);
	sd_detectorAssembly->AddPlacedVolume(sd_cLog, Ma);

	//Add Reflector
        Ta.setX(sd_r_x); Ta.setY(sd_r_y); Ta.setZ(sd_r_z);
	Ma = G4Transform3D(Ra, Ta);
        sd_detectorAssembly->AddPlacedVolume(sd_rLog, Ma);
	
	//Add Shell
        Ta.setX(sd_s_x); Ta.setY(sd_s_y); Ta.setZ(sd_s_z);
	Ma = G4Transform3D(Ra, Ta);
        sd_detectorAssembly->AddPlacedVolume(sd_sLog, Ma);

//==========================================
	
//===========================================//
//		Large Detector		     //
//===========================================//

	//Create CeBr3 Crystal

        //Parameters
        G4double ld_c_inRad = 0.0*cm;
        G4double ld_c_outRad = 38.1*mm;
        G4double ld_c_hz = 50.8*mm;
        G4double ld_c_startAngle = 0.0*deg;
        G4double ld_c_spanAngle = 360.0*deg;

        //Solid
        G4Tubs* ld_cSolid = new G4Tubs("ld_CeBr3", ld_c_inRad, ld_c_outRad,
                        ld_c_hz, ld_c_startAngle, ld_c_spanAngle);
        //Logical Volume
        G4LogicalVolume* ld_cLog = new G4LogicalVolume(ld_cSolid, CeBr3, "ld_CeBr3");

        //Physical Volume Parameters
        G4double ld_c_x = 0.0*cm;
        G4double ld_c_y = 0.0*cm;
        G4double ld_c_z = 0.0*cm;

//------------------------------------------------
	
	//Create Reflector

	
        //Base Parameters
        
	G4double ld_rThickness = 0.01*mm;
        
	G4double ld_rBase_outRad = ld_c_outRad+ld_rThickness;
        G4double ld_rBase_inRad = ld_c_inRad;
        G4double ld_rBase_hz = ld_c_hz+ld_rThickness/2;
        G4double ld_rBase_startAngle = 0.0*deg;
        G4double ld_rBase_spanAngle = 360*deg;

        //Base Solid
        G4Tubs* ld_rBase = new G4Tubs("ld_rBase", ld_rBase_inRad, ld_rBase_outRad,
                        ld_rBase_hz, ld_rBase_startAngle, ld_rBase_spanAngle);

        //Hole Parameters
        G4double ld_rHole_outRad = ld_c_outRad;
        G4double ld_rHole_inRad = ld_c_inRad;
        G4double ld_rHole_hz = ld_rBase_hz;
        G4double ld_rHole_startAngle = ld_rBase_startAngle;
        G4double ld_rHole_spanAngle = ld_rBase_spanAngle;

        //Hole Solid
        G4Tubs* ld_rHole = new G4Tubs("ld_Hole", ld_rHole_inRad, ld_rHole_outRad,
                        ld_rHole_hz, ld_rHole_startAngle, ld_rHole_spanAngle);

        //Hole Positioning
        G4ThreeVector ld_rHoleTrans(0, 0, 1* ld_rThickness);

        //Build Shell Solid
        G4SubtractionSolid* ld_rSolid =
                new G4SubtractionSolid("ld_Reflector_Shell", ld_rBase, ld_rHole, nullRot, ld_rHoleTrans);

        //Reflector Logical Volume
        G4LogicalVolume* ld_rLog  = new G4LogicalVolume(ld_rSolid, Al, "ld_Reflector");

        //Physical Volume Parameters
        G4double ld_r_x = 0.0*m;
        G4double ld_r_y = 0.0*m;
        G4double ld_r_z = ld_c_z-(ld_rThickness/2); 
                                                                         

//-------------------------------------------------------
        
	//Create Al Shell


        //Base Shell Parameters

        G4double ld_sThickness = 0.5*mm;
	G4double ld_gapThickness = 2.4*mm;

	G4double ld_totThickness = ld_sThickness + ld_gapThickness;

        G4double ld_sBase_outRad = ld_c_outRad+ld_totThickness;
        G4double ld_sBase_inRad = ld_c_inRad;
        G4double ld_sBase_hz = ld_c_hz+ld_totThickness/2;
        G4double ld_sBase_startAngle = 0.0*deg;
        G4double ld_sBase_spanAngle = 360*deg;

        //Base Shell Solid
        G4Tubs* ld_sBase = new G4Tubs("ld_Base_Shell", ld_sBase_inRad, ld_sBase_outRad,
                        ld_sBase_hz, ld_sBase_startAngle, ld_sBase_spanAngle);


        //Hole Parameters
        G4double ld_sHole_outRad = ld_c_outRad + ld_gapThickness;
        G4double ld_sHole_inRad = ld_c_inRad;
        G4double ld_sHole_hz = ld_sBase_hz;
        G4double ld_sHole_startAngle = ld_sBase_startAngle;
        G4double ld_sHole_spanAngle = ld_sBase_spanAngle;

        //Hole Solid
        G4Tubs* ld_sHole = new G4Tubs("ld_Shell_Hole", ld_sHole_inRad, ld_sHole_outRad,
                        ld_sHole_hz, ld_sHole_startAngle, ld_sHole_spanAngle);

        //Hole Positioning
        G4ThreeVector ld_sHoleTrans(0, 0, 1*ld_sThickness );

        //Build Shell Solid
        G4SubtractionSolid* ld_sSolid =
                new G4SubtractionSolid("ld_Shell", ld_sBase, ld_sHole, nullRot, ld_sHoleTrans);

        //Shell Logical Volume
        G4LogicalVolume* ld_sLog  = new G4LogicalVolume(ld_sSolid, Al, "ld_Shell");

        //Physical Volume Parameters
        G4double ld_s_x = 0.0*m;
        G4double ld_s_y = 0.0*m;
        G4double ld_s_z = ld_c_z-(ld_totThickness/2);

//------------------------------------------------------------

	//Detector Assembly
	G4AssemblyVolume* ld_detectorAssembly = new G4AssemblyVolume;

	//Add Crystal
	Ta.setX(ld_c_x); Ta.setY(ld_c_y); Ta.setZ(ld_c_z);
	Ma = G4Transform3D(Ra, Ta);
	ld_detectorAssembly->AddPlacedVolume(ld_cLog, Ma);

	//Add Reflector
        Ta.setX(ld_r_x); Ta.setY(ld_r_y); Ta.setZ(ld_r_z);
	Ma = G4Transform3D(Ra, Ta);
        ld_detectorAssembly->AddPlacedVolume(ld_rLog, Ma);
	
	//Add Shell
        Ta.setX(ld_s_x); Ta.setY(ld_s_y); Ta.setZ(ld_s_z);
	Ma = G4Transform3D(Ra, Ta);
        ld_detectorAssembly->AddPlacedVolume(ld_sLog, Ma);

//==========================================

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
	
	gap = 0.0*mm;
	addL = sd_c_hz + sd_totThickness + gap;

	dist = ss_outRad;
	dist += addL;

	theta = 0*rad;
	phi = 0*rad;

	spin = 0*rad;

	Mp = CreateTransform(dist, theta, phi, spin);

	sd_detectorAssembly->MakeImprint(worldLog, Mp);

	//Detector_1
/*        
	gap = 0.0*mm;
	addL = sd_c_hz + sd_totThickness + gap;
	
	dist = ss_outRad;
        dist += addL;

        theta = pi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
	Mp = CreateTransform(dist, theta, phi, spin);

        sd_detectorAssembly->MakeImprint(worldLog, Mp);
*/
/*
	//Detector_2
        
	gap = 0.0*mm;
	addL = sd_c_hz + sd_totThickness + gap;
	
	dist = 114.3*mm;
        dist += addL;

        theta = halfpi*rad;
        phi = 0*rad;

	spin = 0*rad;
	
	Mp = CreateTransform(dist, theta, phi, spin);

       sd_ detectorAssembly->MakeImprint(worldLog, Mp);
*/
//===========================================
	
	//================//
	// Concrete Walls //
	//================//

	//Parameters
	G4double floor_dist = 2.5*m;		//-y
	G4double floor_thickness = 1.0*m;	

	G4double ceiling_dist = 2.0*m;		//+y
	G4double ceiling_thickness = 1.0*m;

	G4double wall1_dist = 2.5*m;		//+x
	G4double wall1_thickness = 1.0*m;

	G4double wall2_dist = 2.5*m;		//+z
        G4double wall2_thickness = 1.0*m;

        G4double wall3_dist = 2.5*m;		//-x
        G4double wall3_thickness = 1.0*m;

        G4double wall4_dist = 2.5*m;		//-z
        G4double wall4_thickness = 1.0*m;
/*
	//Calculated values
	G4double walls_Base_hx = ((wall1_dist + wall1_thickness) + (wall3_dist + wall3_thickness))/2;
	G4double walls_Base_hy = ((wall2_dist + wall2_thickness) + (wall4_dist + wall4_thickness))/2;
	G4double walls_Base_hz = ((ceiling_dist + ceiling_thickness) + (floor_dist + floor_thickness))/2;

        G4double walls_Hole_hx = ((wall1_dist + 0*wall1_thickness) + (wall3_dist + 0*wall3_thickness))/2;
        G4double walls_Hole_hy = ((wall2_dist + 0*wall2_thickness) + (wall4_dist + 0*wall4_thickness))/2;
        G4double walls_Hole_hz = ((ceiling_dist + 0*ceiling_thickness) + (floor_dist + 0*floor_thickness))/2;

	G4double walls_x = ((wall1_dist + wall1_thickness) - (wall3_dist + wall3_thickness))/2;
	G4double walls_y = ((wall2_dist + wall2_thickness) - (wall4_dist + wall4_thickness))/2;
	G4double walls_z = ((ceiling_dist + ceiling_thickness) - (floor_dist + floor_thickness))/2;

	G4ThreeVector walls_Pos(walls_x, walls_y, walls_z);



	//Create Base
	G4Box* walls_Base = new G4Box("Base", walls_Base_hx, walls_Base_hy, walls_Base_hz);

	//Create Hole
	G4Box* walls_Hole = new G4Box("Hole", walls_Hole_hx, walls_Hole_hy, walls_Hole_hz);

	//Create Solid
	G4SubtractionSolid* wallsSolid = new G4SubtractionSolid("Walls", walls_Base, walls_Hole);
*/

	//Calculated Values
        G4double walls_Base_hx = ((wall1_dist + wall1_thickness) + (wall3_dist + wall3_thickness))/2;
        G4double walls_Base_hy = ((ceiling_dist + ceiling_thickness) + (floor_dist + floor_thickness))/2;
        G4double walls_Base_hz = ((wall2_dist + wall2_thickness) + (wall4_dist + wall4_thickness))/2;

	G4double walls_Base_posx = ((wall1_dist + wall1_thickness) - (wall3_dist + wall3_thickness))/2;
        G4double walls_Base_posy = ((ceiling_dist + ceiling_thickness) - (floor_dist + floor_thickness))/2;
        G4double walls_Base_posz = ((wall2_dist + wall2_thickness) - (wall4_dist + wall4_thickness))/2;


	//Create Solids
	G4Box* floor = new G4Box("Floor", walls_Base_hx, floor_thickness/2, walls_Base_hz);
	G4Box* ceiling = new G4Box("Ceiling", walls_Base_hx, ceiling_thickness/2, walls_Base_hz);
	G4Box* wall1 = new G4Box("Wall 1", wall1_thickness/2, walls_Base_hy, walls_Base_hz);
	G4Box* wall2 = new G4Box("Wall 2", walls_Base_hx, walls_Base_hy, wall2_thickness/2);
        G4Box* wall3 = new G4Box("Wall 3", wall3_thickness/2, walls_Base_hy, walls_Base_hz);
        G4Box* wall4 = new G4Box("Wall 4", walls_Base_hx, walls_Base_hy, wall4_thickness/2);

	//Create Placement Vectors
	G4ThreeVector floor_pos(walls_Base_posx, -(floor_dist + floor_thickness/2), walls_Base_posz); 
	G4ThreeVector ceiling_pos(walls_Base_posx, ceiling_dist + ceiling_thickness/2, walls_Base_posz);
	G4ThreeVector wall1_pos(wall1_dist + wall1_thickness/2, walls_Base_posy, walls_Base_posz);
	G4ThreeVector wall2_pos(walls_Base_posx, walls_Base_posy,  wall2_dist + wall2_thickness/2);
	G4ThreeVector wall3_pos(-(wall3_dist + wall3_thickness/2), walls_Base_posy, walls_Base_posz);
	G4ThreeVector wall4_pos(walls_Base_posx, walls_Base_posy, -(wall4_dist + wall4_thickness/2));

	//Rotation
	G4RotationMatrix noRot = G4RotationMatrix();

	//Create Transforms
	G4Transform3D tr_floor = G4Transform3D(noRot, floor_pos);
	G4Transform3D tr_ceiling = G4Transform3D(noRot, ceiling_pos);
	G4Transform3D tr_w1 = G4Transform3D(noRot, wall1_pos);
	G4Transform3D tr_w2 = G4Transform3D(noRot, wall2_pos);
	G4Transform3D tr_w3 = G4Transform3D(noRot, wall3_pos);
	G4Transform3D tr_w4 = G4Transform3D(noRot, wall4_pos);

	//Create Union Solid
	G4MultiUnion* wallsSolid = new G4MultiUnion("Walls");

	wallsSolid->AddNode(*floor, tr_floor);
	wallsSolid->AddNode(*ceiling, tr_ceiling);
	wallsSolid->AddNode(*wall1, tr_w1);
	wallsSolid->AddNode(*wall2, tr_w2);
	wallsSolid->AddNode(*wall3, tr_w3);
	wallsSolid->AddNode(*wall4, tr_w4);

	wallsSolid->Voxelize();

	//Create Logical Volume
	G4LogicalVolume* wallsLog = new G4LogicalVolume(wallsSolid, Concrete, "Walls");

	//Create Physical Volume
	G4VPhysicalVolume* wallsPhys = new G4PVPlacement(0, G4ThreeVector(), wallsLog, "Walls", worldLog, false, 0);



/*
	//Define Floor Parameters
        G4double floor_hx = 5.0*m;
        G4double floor_hy = 1.0*m;
        G4double floor_hz = 5.0*m;

        //Create Solid World
        G4Box* floorSolid  = new G4Box("Floor", floor_hx, floor_hy, floor_hz);

        //Create Logical World
        G4LogicalVolume* floorLog = new G4LogicalVolume(floorSolid, Concrete, "Floor");

        //Define Physical World Parameters
        G4double floor_pos_x = 0.0*m;
        G4double floor_pos_y = -3.0*m;
        G4double floor_pos_z = 0.0*m;

        //Create Physical World
        G4VPhysicalVolume* floorPhys =
        new G4PVPlacement(0,            //no rotation
                        G4ThreeVector(floor_pos_x, floor_pos_y, floor_pos_z),
                                        //translation position
                        floorLog,       //logical volume
                        "Floor",        //name
                        worldLog,       //Mother volume
                        false,          //no bool
                        0);             //copy number

*/
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
	SetSensitiveDetector("sd_CeBr3", detector, true);
	
	SetSensitiveDetector("ld_CeBr3", detector, true);
}

//Auxillary Functions//

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


}
