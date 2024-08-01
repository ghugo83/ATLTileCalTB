// ***************************************************************************
//
// Construct hadron inelastic physics processes with FLUKA.CERN XS and models.
//
// Author: G.Hugo, 01 August 2022
//
// History: L. Pezzotti adaption of the FLUKAHadronInelasticPhysicsConstructor
// by G. Hugo to not include the HP treatment for neutrons. (June 6, 2023)
// ***************************************************************************
#ifdef G4_USE_FLUKA

#include "FLUKAHadronInelasticPhysics.hh"

// G4
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
// G4
#include "G4ParticleDefinition.hh"
#include "G4PhysicsListHelper.hh"
// G4
#include "G4HadParticles.hh"
#include "G4HadronicParameters.hh"
// G4
#include "G4NeutronCaptureProcess.hh"
#include "G4NeutronRadCapture.hh"
// G4
#include "G4HadronInelasticProcess.hh"

#include "build_G4_process_helpers.hh"

#include "FLUKAInelasticScatteringXS.hh"
#include "FLUKANuclearInelasticModel.hh"


// DEBUG: CHERRY PICK FTFP_BERT XS / MODELS INSTEAD
#include "G4BGGNucleonInelasticXS.hh"
#include "G4NeutronInelasticXS.hh"
#include "G4BGGPionInelasticXS.hh"
#include "G4CrossSectionInelastic.hh"
#include "G4ComponentGGHadronNucleusXsc.hh"

#include "G4HadronicBuilder.hh"
#include "G4ProtonBuilder.hh"
#include "G4BertiniProtonBuilder.hh"
#include "G4FTFPProtonBuilder.hh"
#include "G4NeutronBuilder.hh"
#include "G4BertiniNeutronBuilder.hh"
#include "G4FTFPNeutronBuilder.hh"
#include "G4PhysListUtil.hh"
#include "G4NeutronRadCapture.hh"
#include "G4NeutronInelasticXS.hh"
#include "G4NeutronCaptureXS.hh"
#include "G4PionBuilder.hh"
#include "G4BertiniPionBuilder.hh"
#include "G4FTFPPionBuilder.hh"
#include "G4KaonBuilder.hh"
#include "G4BertiniKaonBuilder.hh"
#include "G4FTFPKaonBuilder.hh"

#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4FTFModel.hh"
#include "G4ExcitedStringDecay.hh"
//#include "G4QuasiElasticChannel.hh"


// ***************************************************************************
// Construct hadron inelastic physics processes with FLUKA.CERN XS and models.
// ***************************************************************************
FLUKAHadronInelasticPhysics::FLUKAHadronInelasticPhysics(G4int verbose)
:  G4VPhysicsConstructor("hInelastic FLUKA")
{
  if (verbose > 1) { 
    G4cout << "### FLUKA Hadron Inelastic Physics" << G4endl;
  }

  const auto param = G4HadronicParameters::Instance();
  param->SetEnableBCParticles(true);
}


// ***************************************************************************
// Construct particles. 
// ***************************************************************************
void FLUKAHadronInelasticPhysics::ConstructParticle() {
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}


// ***************************************************************************
// For each particle of interest, 
// processes are created, assigned XS and models, and registered to the process manager.
//
// IMPORTANT NB: The XS (G4VCrossSectionDataSet), models (G4HadronicInteraction), 
// and even processes (G4HadronInelasticProcess) 
// are constructed in a similar way as for any other G4 physics list in G4 source code.
// They do not seem to be OWNED by the G4CrossSectionDataStore, G4EnergyRangeManager 
// and G4ProcessManager respectively, HENCE THEY ARE NEVER DELETED
// (true for ANY physics list).
// Should not matter too much though, because the destructions 
// should have happened at the very end of the run anyway.
// ***************************************************************************
void FLUKAHadronInelasticPhysics::ConstructProcess() {

	const auto param = G4HadronicParameters::Instance();
	const bool isQE = false;
	double minFTFP_pion = param->GetMinEnergyTransitionFTF_Cascade();
	double maxBERT_pion = param->GetMaxEnergyTransitionFTF_Cascade();
	double minFTFP_kaon = param->GetMinEnergyTransitionFTF_Cascade();
	double maxBERT_kaon = param->GetMaxEnergyTransitionFTF_Cascade();
	double minFTFP_proton = param->GetMinEnergyTransitionFTF_Cascade();
	double maxBERT_proton = param->GetMaxEnergyTransitionFTF_Cascade();
	double minFTFP_neutron = param->GetMinEnergyTransitionFTF_Cascade();
	double maxBERT_neutron = param->GetMaxEnergyTransitionFTF_Cascade();
	double minBERT_proton = 0.;
	double minBERT_neutron = 0.;

	const auto helper = G4PhysicsListHelper::GetPhysicsListHelper();

  // FLUKA hadron - nucleus inelastic XS
	//const auto flukaInelasticScatteringXS = new FLUKAInelasticScatteringXS();

  // FLUKA hadron - nucleus model
  //double minFLUKA = 29.*CLHEP::GeV;
	double minFLUKA = 6.1*CLHEP::GeV;
	const auto flukaModel = new FLUKANuclearInelasticModel();
	flukaModel->SetMinEnergy(minFLUKA);


  // PROTON
	/*build_G4_process_helpers::buildInelasticProcess(G4Proton::Proton(),
                                                  helper,
                                                  flukaInelasticScatteringXS,
                                                  flukaModel);*/
	
  // DEBUG: CHERRY PICK G4 XS INSTEAD
  /*auto protonInelasticProcess = new G4HadronInelasticProcess("protonInelastic", G4Proton::Proton());
  helper->RegisterProcess(protonInelasticProcess, G4Proton::Proton());	
    //protonInelasticProcess->AddDataSet(flukaInelasticScatteringXS);
    auto BGG = new G4BGGNucleonInelasticXS(G4Proton::Proton());
    protonInelasticProcess->AddDataSet(BGG);

    protonInelasticProcess->RegisterMe(flukaModel);*/
    // IMPORTANT NB: BERTINI IS MISSING IN THE G4 CHERRY-PICK!!
    //auto theModel = new G4TheoFSGenerator("FTFP");
    //auto theStringModel = new G4FTFModel();
    //theStringModel->SetFragmentationModel(new G4ExcitedStringDecay());
    //theModel->SetHighEnergyGenerator(theStringModel);
    //theModel->SetQuasiElasticChannel(new G4QuasiElasticChannel());
    //auto theCascade = new G4GeneratorPrecompoundInterface();
    //theModel->SetTransport(theCascade);
    //theModel->SetMinEnergy(G4HadronicParameters::Instance()->GetMinEnergyTransitionFTF_Cascade());
    //theModel->SetMaxEnergy(G4HadronicParameters::Instance()->GetMaxEnergy());
    //protonInelasticProcess->RegisterMe(theModel);
    

  // TAKE G4 XS and MODELS
  auto pro = new G4ProtonBuilder;
  AddBuilder(pro);
  auto ftfpp = new G4FTFPProtonBuilder(isQE);
  AddBuilder(ftfpp);
  pro->RegisterMe(ftfpp);
  ftfpp->SetMinEnergy(minFTFP_proton);
  auto bertp = new G4BertiniProtonBuilder;
  AddBuilder(bertp);
  pro->RegisterMe(bertp);
  bertp->SetMinEnergy(minBERT_proton);
  bertp->SetMaxEnergy(maxBERT_proton);
  pro->Build();


    

  // NEUTRON
  /*const auto neutron = G4Neutron::Neutron();

  // NEUTRON INELASTIC
  const auto neutronInelasticProcess = new G4HadronInelasticProcess("neutronInelastic", neutron);
  // NB: No XS is set by default in the G4HadronInelasticProcess constructor.
  helper->RegisterProcess(neutronInelasticProcess, neutron);
  const auto flukaNeutronModel = new FLUKANuclearInelasticModel();
  neutronInelasticProcess->RegisterMe(flukaNeutronModel);

  // Also non-HP: FLUKA neutron inelastic
  // IMPORTANT NB: Since flukaInelasticScatteringXS is SetForAllAtomsAndEnergies,
  // it needs to be set first (would erase any previously defined dataset, 
  // see G4CrossSectionDataStore::AddDataSet).
  neutronInelasticProcess->AddDataSet(flukaInelasticScatteringXS);*/
  // DEBUG: CHERRY PICK G4 XS INSTEAD
  //neutronInelasticProcess->AddDataSet(new G4NeutronInelasticXS());
  
  

  // TO DO: Not elegant to have G4 neutron capture and fission included in FLUKA inelastic. 
  // Create a Physics constructor just for it? 
  // (NB: CANNOT use the neutron builders, 
  // because they would ALSO create a G4HadronInelasticProcess, while we use the FLUKA one).

  // NEUTRON CAPTURE
  /*const auto neutronCaptureProcess = new G4NeutronCaptureProcess();
  // NB: XS (G4NeutronCaptureXS) is already added, in G4NeutronCaptureProcess constructor.
  helper->RegisterProcess(neutronCaptureProcess, neutron);
  const auto neutronRadCaptureModel = new G4NeutronRadCapture();
  neutronCaptureProcess->RegisterMe(neutronRadCaptureModel);*/

  // NO NEUTRON FISSION


  // TAKE G4 XS and MODELS
  auto neu = new G4NeutronBuilder;
  AddBuilder(neu);
  auto ftfpn = new G4FTFPNeutronBuilder(isQE);
  AddBuilder( ftfpn );
  neu->RegisterMe(ftfpn);
  ftfpn->SetMinEnergy(minFTFP_neutron);
  auto bertn = new G4BertiniNeutronBuilder;
  AddBuilder(bertn);
  neu->RegisterMe(bertn);
  bertn->SetMinEnergy(minBERT_neutron);
  bertn->SetMaxEnergy(maxBERT_neutron);
  neu->Build();

  const G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4HadronicProcess* inel = G4PhysListUtil::FindInelasticProcess(neutron);
  if(nullptr != inel) { 
	  inel->AddDataSet(new G4NeutronInelasticXS()); 
  }
  G4HadronicProcess* capture = G4PhysListUtil::FindCaptureProcess(neutron);
  if (nullptr != capture) {
	  capture->RegisterMe(new G4NeutronRadCapture());
  }




  // PI+, PI-
  /*build_G4_process_helpers::buildInelasticProcess(G4PionPlus::PionPlus(),
                                                  helper,
                                                  flukaInelasticScatteringXS,
                                                  flukaModel);*/
  // DEBUG: CHERRY PICK G4 XS INSTEAD
  /*auto pionPlusInelasticProcess = new G4HadronInelasticProcess("pionPlusInelastic", G4PionPlus::PionPlus());
  helper->RegisterProcess(pionPlusInelasticProcess, G4PionPlus::PionPlus());	
  pionPlusInelasticProcess->AddDataSet(new G4BGGPionInelasticXS(G4PionPlus::Definition()) );
  pionPlusInelasticProcess->RegisterMe(flukaModel);*/

  /*build_G4_process_helpers::buildInelasticProcess(G4PionMinus::PionMinus(),
                                                  helper,
                                                  flukaInelasticScatteringXS,
                                                  flukaModel);*/
  // DEBUG: CHERRY PICK G4 XS INSTEAD
  /*auto pionMinusInelasticProcess = new G4HadronInelasticProcess("pionMinusInelastic", G4PionMinus::PionMinus());
  helper->RegisterProcess(pionMinusInelasticProcess, G4PionMinus::PionMinus());	
  pionMinusInelasticProcess->AddDataSet(new G4BGGPionInelasticXS(G4PionMinus::Definition()) );
  pionMinusInelasticProcess->RegisterMe(flukaModel);*/


  // TAKE G4 XS and MODELS
  auto pi = new G4PionBuilder;
  AddBuilder(pi);
  auto ftfppi = new G4FTFPPionBuilder(isQE);
  AddBuilder(ftfppi);
  pi->RegisterMe(ftfppi);
  ftfppi->SetMinEnergy(minFTFP_pion);
  ftfppi->SetMaxEnergy(minFLUKA); // HEEEERE
  auto bertpi = new G4BertiniPionBuilder;
  AddBuilder(bertpi);
  pi->RegisterMe(bertpi);
  bertpi->SetMaxEnergy(maxBERT_pion);
  pi->Build();

  // HEEERE
  auto pionPlusInelasticProcess = G4PhysListUtil::FindInelasticProcess(G4PionPlus::PionPlus());
  if (pionPlusInelasticProcess) { pionPlusInelasticProcess->RegisterMe(flukaModel); }
  auto pionMinusInelasticProcess = G4PhysListUtil::FindInelasticProcess(G4PionMinus::PionMinus());
  if (pionMinusInelasticProcess) { 
	  G4TheoFSGenerator* theModel = new G4TheoFSGenerator("FTFP");

	  G4FTFModel* theStringModel = new G4FTFModel();
	  theStringModel->SetFragmentationModel(new G4ExcitedStringDecay());
	  G4GeneratorPrecompoundInterface* theCascade = new G4GeneratorPrecompoundInterface();
	  theModel->SetHighEnergyGenerator(theStringModel);
	  theModel->SetTransport(theCascade);

	  theModel->SetMinEnergy(minFLUKA);
	  theModel->SetMaxEnergy(G4HadronicParameters::Instance()->GetMaxEnergy());
	  pionMinusInelasticProcess->RegisterMe(theModel); 
  }




  // KAONS
  /*build_G4_process_helpers::buildInelasticProcessForEachParticle(G4HadParticles::GetKaons(), 
                                                                 helper,
                                                                 flukaInelasticScatteringXS,
                                                                 flukaModel);*/

  // DEBUG: CHERRY PICK G4 XS INSTEAD
  /*const auto particlesTable = G4ParticleTable::GetParticleTable();
  // Loop on all particles
  for (const auto& particlePDGId : G4HadParticles::GetKaons()) {
	  const auto particle = particlesTable->FindParticle(particlePDGId);
	  if (!particle) { continue; }
	  auto kaonInelasticProcess = new G4HadronInelasticProcess(particle->GetParticleName() + "HadronInelastic", particle);
	  helper->RegisterProcess(kaonInelasticProcess, particle);	
	  kaonInelasticProcess->AddDataSet(new G4CrossSectionInelastic(new G4ComponentGGHadronNucleusXsc()) );
	  kaonInelasticProcess->RegisterMe(flukaModel);
	  }*/
  

  // TAKE G4 XS and MODELS
  auto k = new G4KaonBuilder;
  AddBuilder(k);
  auto ftfpk = new G4FTFPKaonBuilder(isQE);
  AddBuilder(ftfpk);
  k->RegisterMe(ftfpk);
  ftfpk->SetMinEnergy(minFTFP_kaon);
  auto bertk  = new G4BertiniKaonBuilder;
  AddBuilder(bertk);
  k->RegisterMe(bertk);
  bertk->SetMaxEnergy(maxBERT_kaon);
  k->Build();
  



  if (param->GetMaxEnergy() > param->EnergyThresholdForHeavyHadrons()) {

    // HYPERONS, ANTI-HYPERONS
	  /*build_G4_process_helpers::buildInelasticProcessForEachParticle(G4HadParticles::GetHyperons(),
                                                                   helper,
                                                                   flukaInelasticScatteringXS,
                                                                   flukaModel);
    build_G4_process_helpers::buildInelasticProcessForEachParticle(G4HadParticles::GetAntiHyperons(),
                                                                   helper,
                                                                   flukaInelasticScatteringXS,
                                                                   flukaModel);

    // LIGHT ANTI-IONS: PBAR, NBAR, ANTI LIGHT IONS
    build_G4_process_helpers::buildInelasticProcessForEachParticle(G4HadParticles::GetLightAntiIons(),
                                                                   helper,
                                                                   flukaInelasticScatteringXS,
                                                                   flukaModel);

    // B-, C- BARYONS AND MESONS
    if (G4HadronicParameters::Instance()->EnableBCParticles() ) {
      build_G4_process_helpers::buildInelasticProcessForEachParticle(G4HadParticles::GetBCHadrons(),
                                                                     helper,
                                                                     flukaInelasticScatteringXS,
                                                                     flukaModel);
								     }*/
	  // TAKE G4 XS and MODELS
	  // anti light ions
	  G4HadronicBuilder::BuildAntiLightIonsFTFP();

	  // hyperons
	  G4HadronicBuilder::BuildHyperonsFTFP_BERT();

	  // b-, c- baryons and mesons
	  G4cout << "@@@@@@@ param->EnableBCParticles() = " << param->EnableBCParticles() << G4endl;
	  if ( param->EnableBCParticles() ) {
		  G4HadronicBuilder::BuildBCHadronsFTFP_BERT();
	  }

	  // light hypernuclei and anti-hypernuclei
	  G4cout << "@@@@@@@ param->EnableHyperNuclei() = " << param->EnableHyperNuclei() << G4endl;
	  if ( param->EnableHyperNuclei() ) {
		  G4HadronicBuilder::BuildHyperNucleiFTFP_BERT();
		  G4HadronicBuilder::BuildHyperAntiNucleiFTFP_BERT();
	  }

  }
}


#endif // G4_USE_FLUKA
