#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class MyRunAction;
class PrimaryGeneratorAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(MyRunAction* runAction, PrimaryGeneratorAction* generatorAction);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep, G4int i) { fEdepV.at(i) += edep; };

    G4double xprime, yprime, zprime, distdEdx;
    std::vector<G4double> vdEdz;
    std::vector<G4double> vEn;

    std::vector<G4double> InitializeZVector(G4double min, G4double max, G4double step);
    std::vector<G4double> InitializeEnVector(G4double min, G4double max, G4double step);
    std::vector<G4double> Initialize_EinVol_Vector(G4int N);

  private:
    MyRunAction* fRunAction = nullptr;
    PrimaryGeneratorAction* fGeneratorAction;
    std::vector<G4double> fEdepV;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
