/**
 * FLUKA v5 by FLUKA development team
 * Copyright CERN 2019
 */

#ifdef G4_USE_FLUKA
#ifndef BUILD_G4_PROCESS_HELPERS_H
#define BUILD_G4_PROCESS_HELPERS_H


#include <vector>

#include <globals.hh>


class G4ParticleDefinition;
class G4PhysicsListHelper;
class G4VCrossSectionDataSet;
class G4HadronicInteraction;


/*
 * Builders helper: 
 * Construct a G4HadronInelasticProcess (could be templated on process class),
 * assign XS and model to the process, 
 * and register the process to the process manager.
 */
namespace build_G4_process_helpers {

	void buildInelasticProcessForEachParticle(const std::vector<G4int>& partList,
						  G4PhysicsListHelper* ph,
						  G4VCrossSectionDataSet* xs,
						  //const double xsFactor,
						  G4HadronicInteraction* hadronicModel);


	void buildInelasticProcess(G4ParticleDefinition* particle,
				   G4PhysicsListHelper* ph,
				   G4VCrossSectionDataSet* xs,
				   //const double xsFactor,
				   G4HadronicInteraction* hadronicModel);
}


#endif
#endif // USE_FLUKA
