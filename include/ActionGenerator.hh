//================================
//Action Generator Header
//===============================


#ifndef ActionGenerator_h
#define ActionGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;

namespace CeBr3
{

class ActionGenerator : public G4VUserPrimaryGeneratorAction
{
        public:
               ActionGenerator(
                const G4String& particleName = "gamma",
                G4double energy = 500.0*keV,
                G4ThreeVector position = G4ThreeVector(0,0,0),
                G4ThreeVector momentumDirection = G4ThreeVector(0,0,1));
               ~ActionGenerator();

               virtual void GeneratePrimaries(G4Event*);

        private:
    		//Data Members
              	 G4ParticleGun* fParticleGun;
};



}

#endif

