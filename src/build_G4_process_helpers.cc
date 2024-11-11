/**
 * FLUKA v5 by FLUKA development team
 * Copyright CERN 2019
 */

#ifdef G4_USE_FLUKA


#include "build_G4_process_helpers.h"

#include <G4ParticleDefinition.hh>
#include <G4PhysicsListHelper.hh>
//#include <G4HadronicParameters.hh>
//#include <G4ProcessManager.hh>
#include <G4VCrossSectionDataSet.hh>
#include <G4HadronicInteraction.hh>

#include <G4HadronInelasticProcess.hh>


namespace build_G4_process_helpers {

	/*
	 * For all particles.
	 */
	void buildInelasticProcessForEachParticle(const std::vector<G4int>& allParticlesPDGIds,
						  G4PhysicsListHelper* const helper,
						  G4VCrossSectionDataSet* const xs,
						  /*const double xsFactor,*/
						  G4HadronicInteraction* const hadronicModel) {

		const auto particlesTable = G4ParticleTable::GetParticleTable();

		// Loop on all particles
		for (const auto& particlePDGId : allParticlesPDGIds) {

			const auto particle = particlesTable->FindParticle(particlePDGId);
			if (!particle) { continue; }

			buildInelasticProcess(particle,
					      helper,
					      xs,
					      //xsFactor,
					      hadronicModel);
		}
	}


	/*
	 * For a specific particle.
	 */
	void buildInelasticProcess(G4ParticleDefinition* const particle,
				   G4PhysicsListHelper* const helper,
				   G4VCrossSectionDataSet* const xs,
				   //const double xsFactor,
				   G4HadronicInteraction* const hadronicModel) {

		const auto process = new G4HadronInelasticProcess(particle->GetParticleName() + "HadronInelastic", particle);
		process->AddDataSet(xs);
		process->RegisterMe(hadronicModel);
		//if (param->ApplyFactorXS()) ((G4HadronicProcess*)process)->MultiplyCrossSectionBy(xsFactor);
		helper->RegisterProcess(process, particle);
	}

} // namespace build_G4_process_helpers


#endif // USE_FLUKA
