/**
 * FLUKA v5 by FLUKA development team
 * Copyright CERN 2019
 */

#ifdef G4_USE_FLUKA


#include "FLUKANuclearInelasticModel.h"

#include <G4HadFinalState.hh>
#include <G4HadProjectile.hh>
#include <G4Nucleus.hh>

#include "test_inelastic_interaction.h"


/*
 * FLUKA hadron inelastic physics final state.
 */
FLUKANuclearInelasticModel::FLUKANuclearInelasticModel()
	: G4HadronicInteraction("FLUKANuclearInelasticModel"),
	  finalState_(std::make_unique<G4HadFinalState>())
{}


/*
 * FLUKA hadron inelastic physics: returns final state from FLUKA.
 */
G4HadFinalState* FLUKANuclearInelasticModel::ApplyYourself(const G4HadProjectile& projectile, 
							   G4Nucleus& targetNucleus) {

	// Clean-up final state.
	finalState_->Clear();
	finalState_->SetStatusChange(stopAndKill);
	
	// GET FINAL STATE FROM FLUKA INTERFACE
	fluka_interface::setNuclearInelasticFinalState(finalState_.get(),
						       projectile,
						       targetNucleus);

	return finalState_.get();
}


#endif //USE_FLUKA
