//====================================
//Action Initialization Implementation
//====================================

#include "ActionGenerator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

namespace CeBr3
{
ActionGenerator::ActionGenerator(
                                const G4String& particleName,
                                G4double energy,
                                G4ThreeVector position,
                                G4ThreeVector momentumDirection)
                                  : G4VUserPrimaryGeneratorAction(),
                                 fParticleGun(0)

{
        G4int nofParticles = 1;
        fParticleGun = new G4ParticleGun(nofParticles);

        //Default Kinematics
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* particle
                = particleTable->FindParticle(particleName);
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleEnergy(energy);
        fParticleGun->SetParticlePosition(position);
        fParticleGun->SetParticleMomentumDirection(momentumDirection);

}

///////////////////////

ActionGenerator::~ActionGenerator()
{
        delete fParticleGun;
}
//////////////////////

void ActionGenerator::GeneratePrimaries(G4Event* anEvent)
{
        fParticleGun->GeneratePrimaryVertex(anEvent);
}


//////////////////////




}
