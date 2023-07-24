//========================
//Detector Assembly Header
//========================

#ifndef DetectorAssembly_h
#define DetectorAssembly_h

#include "DetectorFront.hh"
#include "DetectorBack.hh"
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
		void SetDetBack(DetectorBack* b)	{detBack = b;};

		//Get Functions
		DetectorFront* GetDetFront()		{return detFront;};
		DetectorBack* GetDetBack()		{return detBack;};

		void ConstructAssembly();

		void PlaceAssembly(G4LogicalVolume* worldLog, G4double dist, G4double gap,
                        G4double theta, G4double phi, G4double spin);

		//Helper Functions
		G4double GetLength();

		G4double GetWidth();
		
	private:
		//Components
		DetectorFront* detFront;

		DetectorBack* detBack;

		G4AssemblyVolume* assembly;




};

}

#endif

