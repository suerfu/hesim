/// \file JanisDetectorConstruction.hh
/// \brief Definition of the JanisDetectorConstruction class

#ifndef JanisDetectorConstruction_h
#define JanisDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
//class OxCryoMaterials;
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
    const G4VPhysicalVolume* GetLHePV() const;

  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    G4VPhysicalVolume* ConstructVolumes();
    G4Material* mate;

    G4LogicalVolume*   WorldLV; // pointers
    G4VPhysicalVolume* WorldPV;

    G4VPhysicalVolume* liquid_helium_PV;

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    G4bool  fConstructed;
};

// inline functions

inline const G4VPhysicalVolume* JanisDetectorConstruction::GetLHePV() const {
  return liquid_helium_PV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
