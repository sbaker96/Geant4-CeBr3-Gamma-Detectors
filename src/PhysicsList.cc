//=================================
//Physics List Implementation
//=================================

#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4RadioactiveDecayPhysics.hh"

namespace CeBr3
{

PhysicsList::PhysicsList()
{
        RegisterPhysics(new G4DecayPhysics());
	RegisterPhysics(new G4EmStandardPhysics_option4());
        RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

}

