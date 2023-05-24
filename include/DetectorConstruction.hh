//==============================
//Detector Construction Header
//==============================

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4Transform3D.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManager.hh"

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
	
		//Auxillary Functions
		G4LogicalVolume* CreateDetectorCrystal(G4double width, G4double height, G4Material* mat);
		G4AssemblyVolume* CreateDetectorAssembly(G4LogicalVolume* crystal, G4double width, G4double height,
				G4double reflectorThickness, G4double shellThickness, G4double gapSide, G4double gapFront,
				G4Material* rMat, G4Material* sMat);
		G4Transform3D CreateTransform(G4double dist, G4double theta, G4double phi, G4double spin);

	protected:
                G4LogicalVolume* fScoringVolume = nullptr;

};

}

#endif
