//===================
//Detector PMT Header
//===================

#ifndef DetectorPMT_h
#define DetectorPMT_h

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManager.hh"

namespace CeBr3
{

class DetectorPMT
{
	public:
		DetectorPMT();
		~DetectorPMT();

		//Set Functions
		void SetWidth(G4double w)		{width = w;};
		void SetLength(G4double l)		{length = l;};
		void SetThickness(G4double t)		{thickness = t;};

		void SetGlassMat(G4Material* m)		{glassMat = m;};
		void SetGasMat(G4Material* m)		{gasMat = m;};

		//Get Functions
		G4double GetWidth()			{return width;};
		G4double GetLength()			{return length;};
		G4double GetThickness()			{return thickness;};

		//Construct
		void ConstructPMT();

	private:
		//Parameters
		G4double width;
		G4double length;
		G4double thickness;

		G4Material* glassMat;
		G4Material* gasMat;

		//Assembly
		G4Assembly* pmt;
}
	
}
