//========================
// Detector Support Header
//========================

#ifndef DetectorSupport_h
#define DetectorSupport_h

#include "globals.hh"
#include "G4NistManager.hh"

namespace CeBr3
{

class DetectorSupport
{
	public: 
		DetectorSupport();
		~DetectorSupport();

		//Set Functions
		void setHeight(G4double h)		{height = h;};

		void setMat(G4Material* m)		{mat = m;};

		//Get Functions
		G4double getHeight()			{return height;};

		G4Material* getMat()			{return mat;};

		//Helper Functions
		G4Transform3D GenerateShift();
		
	private:
		//Parameters
		G4double height;
		
		G4Material* mat;	
};

}

#endif
