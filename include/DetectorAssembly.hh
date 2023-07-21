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

		//Set Functions
		void SetDetFront(DetectorFront* f)	{detFront = f;};

		//Get Functions
		DetectorFront* GetDetFront()		{return detFront;};

		void ConstructAssembly();

		void PlaceAssembly(G4LogicalVolume* worldLog, G4double dist, G4double gap,
                        G4double theta, G4double phi, G4double spin);

		
	private:
		//Components
		DetectorFront* detFront;

		G4AssemblyVolume* assembly;




};

}

#endif

