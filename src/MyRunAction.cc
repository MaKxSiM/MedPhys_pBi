#include "MyRunAction.hh"
#include "G4RunManager.hh"
#include "G4PrimaryVertex.hh"

MyRunAction::MyRunAction()
{
  vdEdz = InitializeZVector(MinZ, MaxZ, stepfordEdz);
  vEn = InitializeEnVector(MinZ, MaxZ, stepfordEdz);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->OpenFile("output.root");

    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);


    man->SetVerboseLevel(1);
    man->SetNtupleMerging(true);

    // Fluences for all particles, for a given particle easy to get taking a projection on a particle type

        man->CreateNtuple("Fluences", "Fluences");
        man->CreateNtupleIColumn("Event");
        man->CreateNtupleDColumn("X");
        man->CreateNtupleDColumn("Y");
        man->CreateNtupleDColumn("Zsurf");
        man->CreateNtupleDColumn("Energy");
        man->CreateNtupleIColumn("particle_id");
        man->CreateNtupleSColumn("material_name_end");
        man->FinishNtuple(0);

    // There will be an input vector of various Zs

        man->CreateNtuple("At_fixed_Zs", "At_fixed_Zs");
        man->CreateNtupleIColumn("Event");
        man->CreateNtupleDColumn("X");
        man->CreateNtupleDColumn("Y");
        man->CreateNtupleDColumn("Zsurf");
        man->CreateNtupleDColumn("Energy");
        man->CreateNtupleIColumn("particle_id");
        man->FinishNtuple(1);

    // Differential energy for the primary particle (step will be an input parameter)

        man -> CreateNtuple("dEdz","dEdz");
        man->CreateNtupleDColumn("Edep_MeV");
        man->CreateNtupleDColumn("Step");
        man->CreateNtupleDColumn("Z");
        man->CreateNtupleDColumn("En");
        man->FinishNtuple(2);

    // Energy deposited in preconfigured volumes

        man->CreateNtuple("Dose_in_volume_N", "Dose_in_volume_N");
        man->CreateNtupleIColumn("Event");
        man->CreateNtupleDColumn("Edep_MeV");
        man-> CreateNtupleIColumn("VolumeId");
        man->FinishNtuple(3);
}


void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    for (uint i = 0; i<vdEdz.size();i++){
        man->FillNtupleDColumn(2,0,vdEdz.at(i));
        man->FillNtupleDColumn(2,1,stepfordEdz);
        man->FillNtupleDColumn(2,2,i*stepfordEdz);
        man->FillNtupleDColumn(2,3,vEn.at(i));
        man->AddNtupleRow(2);
    }

    man->Write();
    man->CloseFile();
}

std::vector<G4double> MyRunAction::InitializeZVector(G4double Min_Z,G4double Max_Z, G4double step){
  G4double diff = Max_Z - Min_Z;
  std::vector<G4double> temp = {};
  for (int j = 0; j<int(diff/step); j++){
    temp.push_back(0);
  };
  return temp;
}

std::vector<G4double> MyRunAction::InitializeEnVector(G4double Min_Z,G4double Max_Z, G4double step){
  G4double diff = Max_Z - Min_Z;
  std::vector<G4double> temp = {};
  for (int j = 0; j<int(diff/step); j++){
    temp.push_back(-1.);
  };
  return temp;
}
