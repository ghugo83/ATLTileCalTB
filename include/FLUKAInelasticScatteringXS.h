/**
 * FLUKA v5 by FLUKA development team
 * Copyright CERN 2019
 */

#ifdef G4_USE_FLUKA
#ifndef FLUKA_INELASTIC_SCATTERING_XS_H
#define FLUKA_INELASTIC_SCATTERING_XS_H


#include <G4VCrossSectionDataSet.hh>
#include <globals.hh>
#include <G4ElementData.hh>
#include <G4PhysicsVector.hh>
#include <vector>


class G4DynamicParticle;
class G4ParticleDefinition;
class G4Element;
class G4Meterial;
class G4Isotope;


/*
 * FLUKA hadron nucleus inelastic XS.
 */
class FLUKAInelasticScatteringXS final : public G4VCrossSectionDataSet {

public: 
	explicit FLUKAInelasticScatteringXS();

	void CrossSectionDescription(std::ostream&) const;
    
	virtual G4bool IsElementApplicable(const G4DynamicParticle*, 
					   G4int Z,
					   const G4Material*) override;

	virtual G4bool IsIsoApplicable(const G4DynamicParticle*, 
				       G4int Z, G4int A,
				       const G4Element*, const G4Material*) override;

	virtual G4double GetElementCrossSection(const G4DynamicParticle*, 
						G4int Z, 
						const G4Material*) override;

	virtual G4double GetIsoCrossSection(const G4DynamicParticle*, 
					    G4int Z, G4int A,
					    const G4Isotope* iso,
					    const G4Element* elm,
					    const G4Material* mat) override;	

	FLUKAInelasticScatteringXS & operator=(const FLUKAInelasticScatteringXS &right) = delete;
	FLUKAInelasticScatteringXS(const FLUKAInelasticScatteringXS&) = delete;
};


#endif
#endif // USE_FLUKA
