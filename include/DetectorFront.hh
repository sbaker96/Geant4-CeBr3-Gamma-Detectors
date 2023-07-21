//====================
//DetectorFront Header
//====================

#ifndef DetectorFront_h
#define DetectorFront_h

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManger.hh"

#include "DetectorCrystal.hh"

namespace CeBr3
{

class DetectorFront
{
	public:
		DetectorFront();
		~DetectorFront();

		//Set Functions
		void SetRThickness(G4double r)  	{rThickness = r;};
                void SetSThickness(G4double s)  	{sThickness = s;};
                void SetGapFront(G4double g)    	{gapFront = g;};
                void SetGapSide(G4double g)     	{gapSide = g;};

                void SetRMat(G4Material* m)     	{rMat = m;};
                void SetSMat(G4Material* m)     	{sMat = m;};

		void SetCrystal(DetectorCrystal* c )	{crystal = c;};

		//Get Functions
		G4double GetRThickness()        	{return rThickness;};
                G4double GetSThickness()        	{return sThickness;};
                G4double GetGapFront()          	{return gapFront;};
                G4double GetGapSide()           	{return gapSide;};

		G4Material* GetRMat()			{return rMat;};
		G4Material* GetSMat()			{return sMat;};

		DetectorCrystal* GetCrystal()		{return crystal;};

		G4AssemblyVolume* GetAssembly()		{return detFront;};

		//Construct Detector Front
		void ConstructDetFront();

		
	
	private:
		//Parameters
		G4double rThickness = 0;
                G4double sThickness = 0;
                G4double gapFront = 0;
                G4double gapSide = 0;
                G4Material* rMat;
                G4Material* sMat;

		//Crystal
		DetectorCrystal* crystal;

		//Assembly Volume
		G4AssemblyVolume* detFront;

		//AuxillaryFunctions
		G4LogicalVolume* GenerateCan(G4double thick, G4double inRad, 
				G4double length, G4Material* mat, G4String name);




}

}

#endif
