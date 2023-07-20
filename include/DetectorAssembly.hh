//========================
//Detector Assembly Header
//========================

#ifndef DetectorAssembly_h
#define DetectorAssembly_h

#include "DetectorFront.hh"
#include "globals.hh"
#include "G4AssemblyVolume.hh"

namespace CeBr3
{

class DetectorAssembly
{

	public:
		DetectorAssembly();
		~DetectorAssembly();

		void ConstructAssembly();

		void PlaceAssembly(G4LogicalVolume* worldLog, G4double dist, G4double gap,
                        G4double theta, G4double phi, G4double spin);

		
	private:
		//Components
		DetectorFront detectorFront;

		G4AssemblyVolume* assembly;




};

}

#endif

