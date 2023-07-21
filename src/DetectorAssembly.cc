//================================
//Detector Assembly Implementation
//================================

#include "DetectorAssembly.hh"
#include "G4SystemOfUnits.hh"

namespace CeBr3
{

DetectorAssembly::DetectorAssembly()
{;}

DetectorAssembly::~DetectorAssembly()
{;}

void DetectorAssembly::ConstructAssembly()
{
	//Get Placement
	G4double f_x = 0*mm;
	G4double f_y = 0*mm;
	G4double f_z = 0*mm;

	assembly = new G4AssemblyVolume;

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

	//Add Detector Front
	Ta.setX(f_x); Ta.setY(f_y); Ta.setZ(f_z);
	Ma = G4Transform3D(Ra, Ta);

	assembly->AddPlacedVolume(detFront->GetAssembly(), Ma);

}

void DetectorAssembly::PlaceAssembly(G4LogicalVolume* worldLog, G4double dist, G4double gap,
                        G4double theta, G4double phi, G4double spin)
{
	//******************
	//Tenetive Placement
	//******************
	
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

	detector->MakeImprint(worldLog, Mp);

}

}
