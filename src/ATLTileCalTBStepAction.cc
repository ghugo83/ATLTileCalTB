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
#include "G4VProcess.hh"

#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

//Constructor and de-constructor
//
ATLTileCalTBStepAction::ATLTileCalTBStepAction(ATLTileCalTBEventAction* EventAction)
    : G4UserSteppingAction(),
      fEventAction( EventAction ){}

ATLTileCalTBStepAction::~ATLTileCalTBStepAction() {
	G4cout << "DESTRUCTOR neutronCount = " << neutronCount
	       << ", antiNeutronCount = " << antiNeutronCount
	       << ", protonCount = " << protonCount
	       << ", antiProtonCount = " << antiProtonCount
	       << ", pionPlusCount = " << pionPlusCount
	       << ", pionMinusCount = " << pionMinusCount
	       << ", electronCount = " << electronCount
	       << ", positronCount = " << positronCount
	       << ", gammaCount = " << gammaCount
	       << ", othersCount = " << othersCount
	       << G4endl;
}

//UserSteppingaction() method
//
void ATLTileCalTBStepAction::UserSteppingAction( const G4Step* aStep ) {
    
	const auto& myTrack = aStep->GetTrack();

	if (aStep->GetStepLength() > 0 ||
	    aStep->GetTotalEnergyDeposit() > 0 || 
	    myTrack->GetTrackStatus() == G4TrackStatus::fStopAndKill) {

    //Collect out of world leakage
	//if (!aStep->GetTrack()->GetNextVolume()) {
	const G4StepPoint* const prePoint  = aStep->GetPreStepPoint();
	const G4StepPoint* const postPoint = aStep->GetPostStepPoint();
	if ( prePoint->GetTouchable()->GetVolume(0) 
	     && postPoint->GetTouchable()->GetVolume(0)
	     && prePoint->GetTouchable()->GetVolume(0)->GetName() == "ironboxPV"
	     && postPoint->GetTouchable()->GetVolume(0)->GetName() == "all_PV"
					  ) {



		auto PDGID = aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
		auto pTable = G4ParticleTable::GetParticleTable();
		const G4int neutronID = pTable->FindParticle("neutron")->GetPDGEncoding();
		const G4int antineutronID = pTable->FindParticle("anti_neutron")->GetPDGEncoding();
		const G4int protonID = pTable->FindParticle("proton")->GetPDGEncoding();
		const G4int antiprotonID = pTable->FindParticle("anti_proton")->GetPDGEncoding();
		const G4int pionplusID = pTable->FindParticle("pi+")->GetPDGEncoding();
		const G4int pionminusID = pTable->FindParticle("pi-")->GetPDGEncoding();
		const G4int electronID = pTable->FindParticle("e-")->GetPDGEncoding();
		const G4int positronID = pTable->FindParticle("e+")->GetPDGEncoding();
		const G4int gammaID = pTable->FindParticle("gamma")->GetPDGEncoding();
		if (PDGID == neutronID) {
			neutronCount++;
		}
		else if (PDGID == antineutronID) {
			antiNeutronCount++;
		}
		else if (PDGID == protonID) {
			protonCount++;
		}
		else if (PDGID == antiprotonID) {
			antiProtonCount++;
		}
		else if (PDGID == pionplusID) {
			pionPlusCount++;
		}
		else if (PDGID == pionminusID) {
			pionMinusCount++;
		}
		else if (PDGID == electronID) {
			electronCount++;
		}
		else if (PDGID == positronID) {
			positronCount++;
		}
		else if (PDGID == gammaID) {
			gammaCount++;
		}  
		else {
			othersCount++;
		}



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
}

//**************************************************
