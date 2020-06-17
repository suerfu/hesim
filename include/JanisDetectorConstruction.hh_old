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
#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
//class OxCryoMaterials;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;
class G4Event;
class JanisDetectorConstructionMessenger;

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

    void setFarSideDistance(G4double fs_distance);
    void setPMTAngle(G4double pmt_angle);
    void add1stFarSideAngle(G4double new_fs_angle_1);
    void add2ndFarSideAngle(G4double new_fs_angle_2);
    void add3rdFarSideAngle(G4double new_fs_angle_3);
    void add4thFarSideAngle(G4double new_fs_angle_4);
    void add5thFarSideAngle(G4double new_fs_angle_5);
    void add6thFarSideAngle(G4double new_fs_angle_6);
    void add7thFarSideAngle(G4double new_fs_angle_7);
    void add8thFarSideAngle(G4double new_fs_angle_8);
    void add9thFarSideAngle(G4double new_fs_angle_9);
    void add10thFarSideAngle(G4double new_fs_angle_10);
    void add1stNaIFarSideAngle(G4double new_fsNaI_angle_1);
    void add2ndNaIFarSideAngle(G4double new_fsNaI_angle_2);
    void add3rdNaIFarSideAngle(G4double new_fsNaI_angle_3);
    void add4thNaIFarSideAngle(G4double new_fsNaI_angle_4);
    void add5thNaIFarSideAngle(G4double new_fsNaI_angle_5);
    void add6thNaIFarSideAngle(G4double new_fsNaI_angle_6);
    void add7thNaIFarSideAngle(G4double new_fsNaI_angle_7);
    void add8thNaIFarSideAngle(G4double new_fsNaI_angle_8);

    // G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

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
    G4double angle_corr;
    G4double fs_placement_angle;
    G4double fs_placement_distance;

    G4LogicalVolume*   WorldLV; // pointers
    G4VPhysicalVolume* WorldPV;

    G4LogicalVolume* fs_head_inner_LV;
    G4LogicalVolume* fs_head_outer_LV;
    G4VPhysicalVolume* fs_head_inner_PV;
    G4VPhysicalVolume* fs_head_outer_PV;
    G4RotationMatrix* fs_head_outer_rm;

    G4LogicalVolume* fs_head_inner_1_LV;
    G4LogicalVolume* fs_head_outer_1_LV;
    G4VPhysicalVolume* fs_head_inner_1_PV;
    G4VPhysicalVolume* fs_head_outer_1_PV;

    G4LogicalVolume* fs_head_inner_2_LV;
    G4LogicalVolume* fs_head_outer_2_LV;
    G4VPhysicalVolume* fs_head_inner_2_PV;
    G4VPhysicalVolume* fs_head_outer_2_PV;

    G4LogicalVolume* fs_head_inner_3_LV;
    G4LogicalVolume* fs_head_outer_3_LV;
    G4VPhysicalVolume* fs_head_inner_3_PV;
    G4VPhysicalVolume* fs_head_outer_3_PV;

    G4LogicalVolume* fs_head_inner_4_LV;
    G4LogicalVolume* fs_head_outer_4_LV;
    G4VPhysicalVolume* fs_head_inner_4_PV;
    G4VPhysicalVolume* fs_head_outer_4_PV;

    G4LogicalVolume* fs_head_inner_5_LV;
    G4LogicalVolume* fs_head_outer_5_LV;
    G4VPhysicalVolume* fs_head_inner_5_PV;
    G4VPhysicalVolume* fs_head_outer_5_PV;

    G4LogicalVolume* fs_head_inner_6_LV;
    G4LogicalVolume* fs_head_outer_6_LV;
    G4VPhysicalVolume* fs_head_inner_6_PV;
    G4VPhysicalVolume* fs_head_outer_6_PV;

    G4LogicalVolume* fs_head_inner_7_LV;
    G4LogicalVolume* fs_head_outer_7_LV;
    G4VPhysicalVolume* fs_head_inner_7_PV;
    G4VPhysicalVolume* fs_head_outer_7_PV;

    G4LogicalVolume* fs_head_inner_8_LV;
    G4LogicalVolume* fs_head_outer_8_LV;
    G4VPhysicalVolume* fs_head_inner_8_PV;
    G4VPhysicalVolume* fs_head_outer_8_PV;

    G4LogicalVolume* fs_head_inner_9_LV;
    G4LogicalVolume* fs_head_outer_9_LV;
    G4VPhysicalVolume* fs_head_inner_9_PV;
    G4VPhysicalVolume* fs_head_outer_9_PV;

    G4LogicalVolume* fs_head_inner_10_LV;
    G4LogicalVolume* fs_head_outer_10_LV;
    G4VPhysicalVolume* fs_head_inner_10_PV;
    G4VPhysicalVolume* fs_head_outer_10_PV;

    G4LogicalVolume* fsNaI_head_inner_1_LV;
    G4LogicalVolume* fsNaI_head_outer_1_LV;
    G4LogicalVolume* fsNaI_scintillator_1_LV;
    G4VPhysicalVolume* fsNaI_head_inner_1_PV;
    G4VPhysicalVolume* fsNaI_head_outer_1_PV;
    G4VPhysicalVolume* fsNaI_scintillator_1_PV;

    G4LogicalVolume* fsNaI_head_inner_2_LV;
    G4LogicalVolume* fsNaI_head_outer_2_LV;
    G4LogicalVolume* fsNaI_scintillator_2_LV;
    G4VPhysicalVolume* fsNaI_head_inner_2_PV;
    G4VPhysicalVolume* fsNaI_head_outer_2_PV;
    G4VPhysicalVolume* fsNaI_scintillator_2_PV;

    G4LogicalVolume* fsNaI_head_inner_3_LV;
    G4LogicalVolume* fsNaI_head_outer_3_LV;
    G4VPhysicalVolume* fsNaI_head_inner_3_PV;
    G4VPhysicalVolume* fsNaI_head_outer_3_PV;
    G4LogicalVolume* fsNaI_scintillator_3_LV;
    G4VPhysicalVolume* fsNaI_scintillator_3_PV;

    G4LogicalVolume* fsNaI_head_inner_4_LV;
    G4LogicalVolume* fsNaI_head_outer_4_LV;
    G4VPhysicalVolume* fsNaI_head_inner_4_PV;
    G4VPhysicalVolume* fsNaI_head_outer_4_PV;
    G4LogicalVolume* fsNaI_scintillator_4_LV;
    G4VPhysicalVolume* fsNaI_scintillator_4_PV;

    G4LogicalVolume* fsNaI_head_inner_5_LV;
    G4LogicalVolume* fsNaI_head_outer_5_LV;
    G4VPhysicalVolume* fsNaI_head_inner_5_PV;
    G4VPhysicalVolume* fsNaI_head_outer_5_PV;
    G4LogicalVolume* fsNaI_scintillator_5_LV;
    G4VPhysicalVolume* fsNaI_scintillator_5_PV;

    G4LogicalVolume* fsNaI_head_inner_6_LV;
    G4LogicalVolume* fsNaI_head_outer_6_LV;
    G4VPhysicalVolume* fsNaI_head_inner_6_PV;
    G4VPhysicalVolume* fsNaI_head_outer_6_PV;
    G4LogicalVolume* fsNaI_scintillator_6_LV;
    G4VPhysicalVolume* fsNaI_scintillator_6_PV;

    G4LogicalVolume* fsNaI_head_inner_7_LV;
    G4LogicalVolume* fsNaI_head_outer_7_LV;
    G4VPhysicalVolume* fsNaI_head_inner_7_PV;
    G4VPhysicalVolume* fsNaI_head_outer_7_PV;
    G4LogicalVolume* fsNaI_scintillator_7_LV;
    G4VPhysicalVolume* fsNaI_scintillator_7_PV;

    G4LogicalVolume* fsNaI_head_inner_8_LV;
    G4LogicalVolume* fsNaI_head_outer_8_LV;
    G4VPhysicalVolume* fsNaI_head_inner_8_PV;
    G4VPhysicalVolume* fsNaI_head_outer_8_PV;
    G4LogicalVolume* fsNaI_scintillator_8_LV;
    G4VPhysicalVolume* fsNaI_scintillator_8_PV;

    void PlacePMT(G4LogicalVolume* worldLV,G4double &PMT_posX,G4double &PMT_posY,G4double &PMT_posZ,G4RotationMatrix* PMT_RM);

    G4VPhysicalVolume* os_body_PV;
    G4VPhysicalVolume* liquid_helium_PV;

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    G4bool  fConstructed;
    G4bool  fReady1; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady2; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady3; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady4; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady5; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady6; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady7; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady8; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady9; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fReady10; // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
    G4bool  fNaIReady1;
    G4bool  fNaIReady2;
    G4bool  fNaIReady3;
    G4bool  fNaIReady4;
    G4bool  fNaIReady5;
    G4bool  fNaIReady6;
    G4bool  fNaIReady7;
    G4bool  fNaIReady8;

    JanisDetectorConstructionMessenger* fDetectorMessenger;

    G4double mylar_surf_density;
    G4double mylar_thickness;
    G4double mylar_density;

    G4double src_dia;
    G4double src_thickness;
    G4double src_active_dia;

    G4double kapton_surf_density;
    G4double kapton_thickness;
    G4double kapton_density;

};

// inline functions

inline const G4VPhysicalVolume* JanisDetectorConstruction::GetLHePV() const {
  return liquid_helium_PV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
