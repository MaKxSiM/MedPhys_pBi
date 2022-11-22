//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "MyRunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MySteppingAction::MySteppingAction(MyRunAction* runAc ,EventAction* eventAction)
:fRunAction(runAc),
 fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MySteppingAction::~MySteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MySteppingAction::UserSteppingAction(const G4Step* step)
{
  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  G4Track* mytrack = step -> GetTrack ();
  G4int i_z, i_p;
  G4double distance, distance_post, xi, yi, zi, xi_post, yi_post, zi_post, En;

  En = step->GetPostStepPoint()->GetKineticEnergy()/CLHEP::MeV;

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  xi = step->GetPreStepPoint()->GetPosition().x();
  yi = step->GetPreStepPoint()->GetPosition().y();
  zi=  step->GetPreStepPoint()->GetPosition().z();

  xi_post = step->GetPostStepPoint()->GetPosition().x();
  yi_post = step->GetPostStepPoint()->GetPosition().y();
  zi_post = step->GetPostStepPoint()->GetPosition().z();


  distance = sqrt( xi*xi + yi*yi + zi*zi ) ;
  distance_post = sqrt( xi_post*xi_post + yi_post*yi_post + zi_post*zi_post ) ;
  i_z = int((distance - fRunAction->MinZ)/fRunAction->stepfordEdz);
  i_p = int((distance_post - fRunAction->MinZ)/fRunAction->stepfordEdz);

  if (mytrack->GetTrackID() == 1){
    //  std::cout << i_z << std::endl;
      if ((distance<fRunAction->MaxZ) && i_z>=0) {
        fRunAction->vdEdz.at(i_z) = fRunAction->vdEdz.at(i_z) + edepStep;
      };
      if (i_p != i_z ){
        fRunAction->vEn.at(i_z) = En;
      }
  };
  if (i_p != i_z ){
    if (G4RunManager::GetRunManager()->GetCurrentEvent()) man->FillNtupleIColumn(0,0,G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    man->FillNtupleDColumn(0,1,yi);
    man->FillNtupleDColumn(0,2,zi);
    man->FillNtupleDColumn(0,3,(i_z+1)*fRunAction->stepfordEdz);
    man->FillNtupleDColumn(0,4,step->GetPreStepPoint()->GetKineticEnergy());
    man->FillNtupleIColumn(0,5,mytrack->GetDefinition()->GetPDGEncoding());
    man->FillNtupleSColumn(0,6,mytrack->GetMaterial()->GetName());
    man->AddNtupleRow(0);
  }
}
