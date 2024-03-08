//========================
// Detector Support Header
//========================

#ifndef DetectorSupport_h
#define DetectorSupport_h

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Transform3D.hh"

namespace CeBr3
{

class DetectorSupport
{
	public: 
		DetectorSupport();
		~DetectorSupport();

		//Set Functions
		void SetHeight(G4double h)		{height = h;};

		void SetMat(G4Material* m)		{mat = m;};

		//Get Functions
		G4double GetHeight()			{return height;};

		G4Material* GetMat()			{return mat;};

		//Helper Functions
		G4Transform3D GenerateHoleShift();
		G4Transform3D GenerateShift();
		
	private:
		//Parameters
		G4double height;
		
		G4Material* mat;	
};

}

#endif
