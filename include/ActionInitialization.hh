//=================================
//Action Initialization Header
//=================================

#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "G4VUserActionInitialization.hh"

namespace CeBr3
{

class ActionInitialization : public G4VUserActionInitialization
{
        public:
         ActionInitialization();
         ~ActionInitialization() override;

	 void BuildForMaster() const override;
         void Build() const override;
};

}

#endif

