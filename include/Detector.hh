//=================
//Detector Header
//=================

#ifndef Detector_h
#define Detector_h

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManager.hh"

namespace CeBr3
{

class Detector
{
	public:
		Detector();
		~Detector();

		//Crystal Functions
		void SetWidth(G4double w) 	{width = w;};
		void SetLength(G4double l) 	{length = l;};
		void SetName(G4String n)	{name = n;};
	
		void SetCMat(G4Material* m)	{cMat = m;};
		
		void ConstructCrystal();	

		G4double GetWidth()		{return width;};
		G4double GetLength()		{return length;};
		G4String GetName()		{return name;};

		G4LogicalVolume* GetCrystal()	{return crystal;};

		//Detector Functions
		void SetRThickness(G4double r)	{rThickness = r;};
		void SetSThickness(G4double s)	{sThickness = s;};
		void SetGapFront(G4double g)	{gapFront = g;};
		void SetGapSide(G4double g)	{gapSide = g;};
	
		void SetRMat(G4Material* m)	{rMat = m;};
		void SetSMat(G4Material* m)	{sMat = m;};

		void ConstructDetector();

		G4double GetRThickness()	{return rThickness;};
		G4double GetSThickness()	{return sThickness;};
		G4double GetGapFront()		{return gapFront;};
		G4double GetGapSide()		{return gapSide;};

		G4AssemblyVolume* GetDetector()	{return detector;};
		
		//Placement Functions
		G4double GetOffset();

		G4Transform3D CreateTransform(G4double dist, 
			G4double theta, G4double phi, G4double spin);
		
		void PlaceDetector(G4LogicalVolume* worldLog, G4double dist, G4double gap,
			G4double theta, G4double phi, G4double spin);


	private:
		//Crystal Parameters
		G4double width = 0;
		G4double length = 0;
		G4String name = " ";
		G4Material* cMat;
		
		//Crystal Logical Volume
		G4LogicalVolume* crystal;

		//Detector Parameters
		G4double rThickness = 0;
		G4double sThickness = 0;
		G4double gapFront = 0;
		G4double gapSide = 0;
		G4Material* rMat;
		G4Material* sMat;

		//Detector Assembly Volume
		G4AssemblyVolume* detector;


};

}

#endif
