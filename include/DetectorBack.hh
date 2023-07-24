//====================
//Detector Back Header
//===================

#ifndef DetectorBack_h
#define DetectorBack_h

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorPMT.hh"

namespace CeBr3
{

class DetectorBack
{
	public:
		DetectorBack();
		~DetectorBack();

		//Set Functions
		void setSideGap(G4double g)		{sideGap = g;};
		void setShieldThickness(G4double t)	{shieldThickness = t;};

		void setShieldMat(G4Material* m)	{sheildMat = m;};

		void setPMT(DetectorPMT* p)		{pmt = p;};

		//Get Functions
		G4double GetSideGap()			{return sideGap;};
		G4double GetShieldThickness()		{return shieldThickness;};

		G4Material* GetShieldMat()		{return sheildMat;};

		DetectorPMT* GetPMT()			{return pmt;};

		//Construct
		void ConstructDetBack();

		//Helper Functions
		G4double CalculateLength();
		G4double CalculateWidth();


	private:
		//Parameters
		G4double sideGap;
		G4double shieldThickness;

		G4Material* shieldMat;

		DetectorPMT* pmt;

		//Assembly

		G4AssemblyVolume* detBack;
};

}


#endif DetectorBack_h
