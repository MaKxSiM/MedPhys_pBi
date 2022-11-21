#include "TreeProducer.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

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
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleDColumn("Edep_MeV");
    man->CreateNtupleDColumn("Step");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleDColumn("En");
    man->FinishNtuple(2);

// Energy deposited in preconfigured volumes

    man->CreateNtuple("Dose_in_volume_N");
    man->CreateNtupleIColumn("Event");
    man->CreateNtupleDColumn("Edep_MeV");
    man-> CreateNtupleIColumn("VolumeId");
    man->FinishNtuple(3);

}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output"+strRunID.str()+".root");
    std::vector<double>
}


void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}
