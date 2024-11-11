/**
 * FLUKA v5 by FLUKA development team
 * Copyright CERN 2019
 */

#ifdef G4_USE_FLUKA
#ifndef FLUKA_NUCLEARINELASTICMODEL_H
#define FLUKA_NUCLEARINELASTICMODEL_H


#include <G4HadronicInteraction.hh>


class G4HadFinalState;
class G4HadProjectile;
class G4Nucleus;


/*
 * FLUKA hadron inelastic physics: returns final state from FLUKA.
 */
class FLUKANuclearInelasticModel final : public G4HadronicInteraction {

public:
	FLUKANuclearInelasticModel();

	virtual G4HadFinalState* ApplyYourself(const G4HadProjectile& projectile, 
					       G4Nucleus& targetNucleus) override;

private:
	std::unique_ptr<G4HadFinalState> finalState_;
};


#endif
#endif // USE_FLUKA
