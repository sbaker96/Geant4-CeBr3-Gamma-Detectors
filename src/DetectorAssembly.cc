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

	G4double b_x = f_x;
	G4double b_y = f_y;
	G4double b_z = f_z + detFront->CalculateLength();

	assembly = new G4AssemblyVolume;

	//Rotation and Translation Matrices
        G4RotationMatrix Ra;
        G4ThreeVector Ta;

        G4Transform3D Ma;

	//Add Detector Front
	Ta.setX(f_x); Ta.setY(f_y); Ta.setZ(f_z);
	Ma = G4Transform3D(Ra, Ta);

	assembly->AddPlacedAssembly(detFront->GetAssembly(), Ma);

	//Add Detector Back
	Ta.setX(b_x); Ta.setY(b_y); Ta.setZ(b_z);
	Ma = G4Transform3D(Ra, Ta);

	assembly->AddPlacedAssembly(detBack->GetAssembly(), Ma);

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
	G4double placeDist = dist + gap;

        //Set Translation
        Tp.setX(placeDist*sin(theta)*cos(phi)); Tp.setY(placeDist*sin(theta)*sin(phi)); Tp.setZ(placeDist*cos(theta));

        //Set Rotation
        Rp.rotateZ(spin); Rp.rotateY(theta); Rp.rotateZ(phi);

        //Set Transform
        Mp = G4Transform3D(Rp, Tp);

	assembly->MakeImprint(worldLog, Mp);

}

G4double DetectorAssembly::CalculateLength()
{
	G4double length = detFront->CalculateLength() + detBack->CalculateLength();

	return length;
}

G4double DetectorAssembly::CalculateWidth()
{
	G4double width = std::max(detFront->CalculateWidth(), detBack->CalculateWidth());

	return width;
}

}
