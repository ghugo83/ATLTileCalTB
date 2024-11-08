//**************************************************
// \file SpectrumAnalyzer.hh
// \brief: Declaration of SpectrumAnalyzer class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 28 August 2023
//**************************************************

// A portable Geant4-based particle spectrum analyzer
// to be used within a Geant4 simulation without affecting it.
// Instead of coding it in the simulation, create a singleton
// and manage its usage with (#ifdef) compiler definition.

#ifdef ATLTileCalTB_LEAKANALYSIS

#ifndef SpectrumAnalyzer_h
#  define SpectrumAnalyzer_h

// Includers from Geant4
//
#  include "G4Step.hh"
#  include "G4ThreadLocalSingleton.hh"

// Includers from C++
//
#  include <functional>

class SpectrumAnalyzer
{
    friend class G4ThreadLocalSingleton<SpectrumAnalyzer>;

  public:
    // Return pointer to class instance
    static SpectrumAnalyzer* GetInstance()
    {
      static G4ThreadLocalSingleton<SpectrumAnalyzer> instance{};
      return instance.Instance();
    }

    // Methods
    //
    // Run-wise methods
    void CreateNtupleAndScorer(const G4String scName = "te");
    inline void ClearNtupleID() { ntupleID = 99; }
    // Event-wise methods
    inline void ClearEventFields()
    {
      neutronScore = 0.;
      antiNeutronScore = 0.;
      protonScore = 0.;
      antiProtonScore = 0.;
      pionPlusScore = 0.;
      pionMinusScore = 0.;
      electronScore = 0.;
      positronScore = 0.;
      gammaScore = 0.;
      othersScore = 0.;
    }
	void FillEventFields() const;
	// Step-wise methods
	void Analyze(const G4Step* step);

	G4double getNeutronScore() const { return neutronScore; }
	G4double getProtonScore() const { return protonScore; }
	G4double getPionPlusScore() const { return pionPlusScore; }
	G4double getGammaScore() const { return gammaScore; }
	G4double getElectronScore() const { return electronScore; }
	G4double getOthersScore() const { return othersScore; }

	void ResetCounts();
	void ComputeEventCounts();
	void PrintEventCounts() const;

  private:
    // Members
    //
    // Run-wise members
    G4int ntupleID;
    std::function<G4double(const G4Step* step)> scorer;
    G4String scorerName{};
    // Event-wise members
    G4double neutronScore;
	G4double antiNeutronScore;
    G4double protonScore;
	G4double antiProtonScore;
    G4double pionPlusScore;
	G4double pionMinusScore;
    G4double electronScore;
	G4double positronScore;
    G4double gammaScore;
    G4double othersScore;

    G4int neutronCount;
	G4int antiNeutronCount;
    G4int protonCount;
	G4int antiProtonCount;
    G4int pionPlusCount;
	G4int pionMinusCount;
    G4int electronCount;
	G4int positronCount;
    G4int gammaCount;
    G4int othersCount;

G4int neutronCountPerEvent = 0.;
	G4int antiNeutronCountPerEvent = 0.;
    G4int protonCountPerEvent = 0.;
	G4int antiProtonCountPerEvent = 0.;
    G4int pionPlusCountPerEvent = 0.;
	G4int pionMinusCountPerEvent = 0.;
    G4int electronCountPerEvent = 0.;
	G4int positronCountPerEvent = 0.;
    G4int gammaCountPerEvent = 0.;
    G4int othersCountPerEvent = 0.;

    // Scoring quantities
    inline static G4double GetMomentum(const G4Step* step)
    {
      return step->GetTrack()->GetMomentum().mag();
    };
    inline static G4double GetKE(const G4Step* step)
    {
      return step->GetTrack()->GetKineticEnergy();
    };
    inline static G4double GetTE(const G4Step* step) { return step->GetTrack()->GetTotalEnergy(); };

  private:
    // Private constructor
    SpectrumAnalyzer() = default;

  public:
    SpectrumAnalyzer(SpectrumAnalyzer const&) = delete;
    void operator=(SpectrumAnalyzer const&) = delete;
};

#endif  // SpectrumAnalyzer_h
#endif  // ATLTileCalTB_LEAKANALYSIS

//**************************************************
