#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"


#include "G4PhysListFactory.hh"
#include "G4RadioactiveDecayPhysics.hh"

using namespace CeBr3;

int main(int argc, char** argv)
{
        //G4UIExecutive
        G4UIExecutive* ui = nullptr;
        if ( argc == 1) {ui = new G4UIExecutive(argc, argv); }

        //Default Run Manager Construction
        auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

        //Mandatory Init Classes
        runManager->SetUserInitialization(new DetectorConstruction);

        //Physics List

	runManager->SetUserInitialization(new PhysicsList);
        
	//ActionInit
        runManager->SetUserInitialization(new ActionInitialization);

        //Initializae G4 Kernel
        runManager->Initialize();

        //Initialize Visualization
        G4VisManager* visManager = new G4VisExecutive;
        visManager->Initialize();

        //Get UI Manager Pointer
        G4UImanager* UImanager = G4UImanager::GetUIpointer();

        //Verbosity Setting
        UImanager->ApplyCommand("/run/verbose 0");
        UImanager->ApplyCommand("/event/verbose 0");
        UImanager->ApplyCommand("/tracking/verbose 0");

	UImanager->ApplyCommand("/cuts/setLowEdge 10 eV");
        //Start Run
        int numOfEvent = 0;

        //UI Test
//	UImanager->ApplyCommand("/control/execute vis.mac");
        runManager->BeamOn(numOfEvent);
        ui->SessionStart();
        delete ui;

        //Termination
        delete runManager;
        delete visManager;
        return 0;
}


