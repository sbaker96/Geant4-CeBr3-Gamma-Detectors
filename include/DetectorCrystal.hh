//======================
//DetectorCrystal Header
//======================

#ifndef DetectorCrystal_h
#define DetectorCrystal_h

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"

namespace CeBr3
{

class DetectorCrystal
{
	public:
		DetectorCrystal();
		~DetectorCrystal();

		//Set Functions
		void SetWidth(G4double w)	{width = w;};
		void SetLength(G4double l)	{length = l;};
		void SetName(G4String n)	{name = n;};
		void SetMaterial(G4Material* m)	{mat = m;};

		//Get Functions
		G4double GetWidth()		{return width;};
		G4double GetLength()		{return length;};
		G4String GetName()		{return name;};
		G4Material* GetMaterial()	{return mat;};

		G4LogicalVolume* GetCrystal()	{return crystal;};

		//Crystal Construction
		void ConstructCrystal();
	
	private:
		//Parameters
		G4double width = 0;
		G4double length = 0;
		G4String name = " ";
		G4Material* mat;

		//Logical Volume
		G4LogicalVolume* crystal;
};

}

#endif
