//================================
// Detector Support Implementation
//================================

#include "DetectorSupport.hh"
#include "G4SystemOfUnits.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

namespace CeBr3
{

	DetectorSupport::DetectorSupport()
	{;}

	DetectorSupport::~DetectorSupport()
	{;}

	G4Transform3D DetectorSupport::GenerateShift()
	{
		//Shift Coordinates
		G4double s_x = 0.0*cm;
		G4double s_y = height/2;
		G4double s_z = 0.0*cm;
		
		//Rotation and Translation Matrices
       		G4RotationMatrix Ra;
        	G4ThreeVector Ta;
		
        	G4Transform3D Ma;
		
		Ta.setX(s_x); Ta.setY(s_y); Ta.setZ(s_z);
		Ma = G4Transform3D(Ra, Ta);

		return Ma;

	}

}
