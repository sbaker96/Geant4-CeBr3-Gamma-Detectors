//===============================
//Physics List Header
//==============================

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

namespace CeBr3
{

class PhysicsList : public G4VModularPhysicsList
{
        public:
                PhysicsList();
                ~PhysicsList() override;
};

}

#endif

