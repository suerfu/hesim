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
/// \file JanisDetectorConstruction.hh
/// \brief Definition of the JanisDetectorConstruction class

#ifndef JanisDetectorConstruction_h
#define JanisDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "OxCryoMaterials.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class OxCryoMaterials;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;
class G4Event;

/// Detector construction class to define materials and geometry.

class JanisDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    JanisDetectorConstruction();
    virtual ~JanisDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    //virtual void ConstructSDandField();

    G4Material* FindMaterial(G4String);

    // get methods
    //
    const G4VPhysicalVolume* GetMeterPV() const;
    G4LogicalVolume* GetMeterLV() const;

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    G4VPhysicalVolume* ConstructVolumes();
    OxCryoMaterials* materialConstruction;
    G4Material* mate;

    G4String fDetector;
    G4double fOffset;

    G4LogicalVolume*   WorldLV;       // pointers
    G4VPhysicalVolume* WorldPV;

    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
                                      // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

  protected:
    G4LogicalVolume*  fScoringVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
