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
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"



DetectorConstruction::DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fStepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  env_mat = nist->FindOrBuildMaterial(env_mat_name);

  // Option to switch on/off checking of volumes overlaps
  //


  world_mat = nist->FindOrBuildMaterial(world_mat_name);

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Envelope
  //
  G4Box* solidEnv =
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size

  logicEnv =
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking






  ConstructScoringVolumes();



// Set maximal step size
  G4double maxStep = 0.1 * CLHEP::mm;
  fStepLimit = new G4UserLimits(maxStep);
  logicEnv->SetUserLimits(fStepLimit);
  logicShape2->SetUserLimits(fStepLimit);



  //
  //always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructScoringVolumes(){


  G4NistManager* nist = G4NistManager::Instance();
  G4int  N = int(world_sizeZ/sc_vol_st);

  G4Box* templatebox =  new G4Box("LayerBox", 0.5*env_sizeZ, 0.5*env_sizeZ, 0.5*sc_vol_st);     //its size
  G4LogicalVolume* templatelogic = new G4LogicalVolume(templatebox,          //its solid
                        env_mat,           //its material
                        "Layer");
  G4String temp_str;
  const G4String pref = "Layer";

  G4VPhysicalVolume* phys_vol;

  for (G4int i = 0; i<N; i++){
          temp_str = pref + std::to_string(i);
          std::cout << "here" << std::endl;
          phys_vol = new G4PVPlacement(0,                    //no rotation
                    G4ThreeVector(0, 0, -0.5*env_sizeZ + (sc_vol_st/2)*i),         //at (0,0,0)
                    templatelogic,                //its logical volume
                    temp_str,                     //its name
                    logicEnv,                     //its mother  volume
                    false,                        //no boolean operation
                    0,                            //copy number
                    false);               //overlaps checking
          fScoringVolumes.push_back(phys_vol);          
  };

// place shape2 inside one of volumes

  G4Box* solidShape2 =
    new G4Box("Shape2",                      //its name
              0.5*shape2_dxy,0.5*shape2_dxy, 0.5*shape2_dz
    ); //its size


  shape2_mat = nist->FindOrBuildMaterial(shape2_mat_name);


  logicShape2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name

  shape2_mat = nist->FindOrBuildMaterial(shape2_mat_name);
  //G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_B");

// find mother volume

//  G4int Npos = (pos2.z() + 0.5*env_sizeZ)/sc_vol_st;
//  G4String name = pref + std::to_string(N);

  G4VPhysicalVolume* bismutvol = new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  if (bismutvol->CheckOverlaps()) {
    std::cout << "overlaps seen" << std::endl;
  }


}
