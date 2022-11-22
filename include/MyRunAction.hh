#ifndef MyRunAction_HH
#define MyRunAction_HH 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    const G4double MinZ = 0.*CLHEP::cm;
    const G4double MaxZ = 100*CLHEP::cm;

    const G4double stepfordEdz = 1.* CLHEP::mm;

    std::vector<G4double> depthsdEdz;

    std::vector<G4double> InitializeZVector(G4double min, G4double max, G4double step);

};

#endif
