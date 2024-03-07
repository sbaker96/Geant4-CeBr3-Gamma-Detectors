//====================
//Detector Back Header
//===================

#ifndef DetectorBack_h
#define DetectorBack_h

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4AssemblyVolume.hh"

#include "DetectorPMT.hh"
#include "DetectorSupport.hh"

namespace CeBr3
{

class DetectorBack
{
	public:
		DetectorBack();
		~DetectorBack();

		//Set Functions
		void SetSideGap(G4double g)		{sideGap = g;};
		void SetShieldThickness(G4double t)	{shieldThickness = t;};

		void SetShieldMat(G4Material* m)	{shieldMat = m;};

		void SetPMT(DetectorPMT* p)		{pmt = p;};
		void SetDetSup(DetectorSupport* s)	{detSup = s;};

		//Get Functions
		G4double GetSideGap()			{return sideGap;};
		G4double GetShieldThickness()		{return shieldThickness;};

		G4Material* GetShieldMat()		{return shieldMat;};

		DetectorPMT* GetPMT()			{return pmt;};
		DetectorSupport* GetDetSup()		{return detSup;};

		G4AssemblyVolume* GetAssembly()		{return detBack;};

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
		DetectorSupport* detSup;

		//Assembly

		G4AssemblyVolume* detBack;
};

}


#endif
