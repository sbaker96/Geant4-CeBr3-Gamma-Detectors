//==============================
//Detector Construction Header
//==============================

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4Transform3D.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace CeBr3
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		DetectorConstruction();
		~DetectorConstruction() override;
		G4VPhysicalVolume* Construct() override;
		void ConstructSDandField() override;
		G4LogicalVolume* GetScoringVolume() const  { return fScoringVolume; }
	
		G4Transform3D CreateTransform(G4double dist, G4double theta, G4double phi, G4double spin); //Auxillary Function


       protected:
                G4LogicalVolume* fScoringVolume = nullptr;

};

}

#endif
