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
#include <G4Step.hh>

#include "ATLTileCalTBStepAction.hh"
#ifdef ATLTileCalTB_LEAKANALYSIS
#include "SpectrumAnalyzer.hh"
#endif

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
    //
    if ( !aStep->GetTrack()->GetNextVolume() ){

/*
const G4StepPoint* const prePoint  = aStep->GetPreStepPoint();
	const G4StepPoint* const postPoint = aStep->GetPostStepPoint();
	const auto& myTrack = aStep->GetTrack();
G4cout << G4endl;
	G4cout << "particle = " << myTrack->GetParticleDefinition()->GetParticleName()
	<< ", kE [MeV] = " << myTrack->GetKineticEnergy()
	<< ", step->GetTotalEnergyDeposit() = " << aStep->GetTotalEnergyDeposit() << G4endl;
	G4cout << ", PREPOINT: " << prePoint->GetPosition()
	       << ", GetStepStatus() = " << prePoint->GetStepStatus()
	       << G4endl;
	if (prePoint->GetTouchable()->GetVolume(0)) {
		G4cout << "prePoint->GetTouchable()->GetVolume(0) = " << prePoint->GetTouchable()->GetVolume(0)
		       << ", GetName() = " << prePoint->GetTouchable()->GetVolume(0)->GetName()
		       << G4endl;
	}
	G4cout << ", POSTPOINT: " << postPoint->GetPosition()
	       << ", GetStepStatus() = " << postPoint->GetStepStatus()
	       << G4endl;
	if (postPoint->GetTouchable()->GetVolume(0)) {
		G4cout << "postPoint->GetTouchable()->GetVolume(0) = " << postPoint->GetTouchable()->GetVolume(0)
		       << ", GetName() = " << postPoint->GetTouchable()->GetVolume(0)->GetName()
		       << G4endl;
		       }
*/

        fEventAction->Add( 0, aStep->GetTrack()->GetKineticEnergy() ); 
        #ifdef ATLTileCalTB_LEAKANALYSIS
        SpectrumAnalyzer::GetInstance()->Analyze(aStep);
        #endif
    }

    if ( aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "CALO::CALO" ||
         aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "Barrel" ) {
 
        //Collect calo energy deposition (everything but what goes into CALO::CALO and Barrel) 
        //Warning: not exact measurement
        //
        fEventAction->Add( 1, aStep->GetTotalEnergyDeposit() );
    
    }

}

//**************************************************
