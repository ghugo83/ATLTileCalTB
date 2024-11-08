//**************************************************
// \file ATLTileCalTBStepAction.cc
// \brief: implementation of ATLTileCalTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 8 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBStepAction.hh"
#ifdef ATLTileCalTB_LEAKANALYSIS
#include "SpectrumAnalyzer.hh"
#endif
#include "G4VProcess.hh"

//Constructor and de-constructor
//
ATLTileCalTBStepAction::ATLTileCalTBStepAction(ATLTileCalTBEventAction* EventAction)
    : G4UserSteppingAction(),
      fEventAction( EventAction ){}

ATLTileCalTBStepAction::~ATLTileCalTBStepAction() {}

//UserSteppingaction() method
//
void ATLTileCalTBStepAction::UserSteppingAction( const G4Step* aStep ) {
    
    //Collect out of world leakage
	//if (!aStep->GetTrack()->GetNextVolume()) {
	const G4StepPoint* const prePoint  = aStep->GetPreStepPoint();
	const G4StepPoint* const postPoint = aStep->GetPostStepPoint();
	if ( prePoint->GetTouchable()->GetVolume(0) 
	     && postPoint->GetTouchable()->GetVolume(0)
	     && prePoint->GetTouchable()->GetVolume(0)->GetName() == "ironboxPV"
	     && postPoint->GetTouchable()->GetVolume(0)->GetName() == "all_PV"
		) {

	    //G4cout << "ATLTileCalTBStepAction::UserSteppingAction aStep->GetTrack()->GetKineticEnergy() = " << aStep->GetTrack()->GetKineticEnergy() << G4endl;
        
        #ifdef ATLTileCalTB_LEAKANALYSIS
        SpectrumAnalyzer::GetInstance()->Analyze(aStep);
        #endif

	fEventAction->Add( 0, aStep->GetTrack()->GetKineticEnergy() ); 


	
	const auto& myTrack = aStep->GetTrack();

	auto creator = myTrack->GetCreatorProcess();
	int processCount = 1;
	while (creator && creator->GetProcessName() == "neutronInelastic" && processCount <= 3) {
	    creator = creator->GetCreatorProcess();
	    processCount++;
        }


	//if (myTrack->GetParticleDefinition()->GetParticleName() == "neutron") {
	/*if (myTrack->GetParticleDefinition()->GetParticleName() == "pi-") {
		G4cout << G4endl;
		G4cout << "particle = " << myTrack->GetParticleDefinition()->GetParticleName()
		       << ", ke [MeV] = " << myTrack->GetKineticEnergy() 
		       << ", creator = " << myTrack->GetCreatorProcess()->GetProcessName()
		       << G4endl;
		G4cout << "@@@@@@@@@@@@@@@@@@@@@@@ Ancestor creator level " << processCount << " is " << creator->GetProcessName() << G4endl;
		G4cout << ", PREPOINT: " << prePoint->GetPosition() << ", R = " << prePoint->GetPosition().getR()
		       << ", GetStepStatus() = " << prePoint->GetStepStatus()
		       << G4endl;
		if (prePoint->GetTouchable()->GetVolume(0)) {
			G4cout << "prePoint->GetTouchable()->GetVolume(0) = " << prePoint->GetTouchable()->GetVolume(0)
			       << ", GetName() = " << prePoint->GetTouchable()->GetVolume(0)->GetName()
			       << G4endl;
		}
		G4cout << ", POSTPOINT: " << postPoint->GetPosition() << ", R = " << postPoint->GetPosition().getR()
		       << ", GetStepStatus() = " << postPoint->GetStepStatus()
		       << G4endl;
		if (postPoint->GetTouchable()->GetVolume(0)) {
			G4cout << "postPoint->GetTouchable()->GetVolume(0) = " << postPoint->GetTouchable()->GetVolume(0)
			       << ", GetName() = " << postPoint->GetTouchable()->GetVolume(0)->GetName()
			       << G4endl;
		}
		}*/

}


	/*
    if ( aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "CALO::CALO" ||
         aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "Barrel" ) {
 
	    //Collect calo energy deposition (everything but what goes into CALO::CALO and Barrel) 
	    //Warning: not exact measurement
	    //
	    fEventAction->Add( 1, aStep->GetTotalEnergyDeposit() );
    
	    }*/

}

//**************************************************
