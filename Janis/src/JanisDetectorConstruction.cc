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
/// \file JanisDetectorConstruction.cc
/// \brief Implementation of the JanisDetectorConstruction class

#include "JanisDetectorConstruction.hh"
#include "OxCryoMaterials.hh"

#include "G4tgbVolumeMgr.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "JanisDetectorConstructionMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4VisExtent.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDetectorConstruction::JanisDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true),
  fDetectorMessenger(0),
  fConstructed(false),
  fs_head_inner_LV(0),fs_head_inner_PV(0),
  fs_head_outer_LV(0),fs_head_outer_PV(0),
  fs_head_outer_rm(0)
{
    fDetectorMessenger = new JanisDetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDetectorConstruction::~JanisDetectorConstruction()
{
    delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* JanisDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::DefineMaterials()
{

    G4NistManager* nistManager =  G4NistManager::Instance();

    G4double a;  // mass of a mole;
    G4double z;  // z=mean number of protons;
    G4double density, fractionMass;
    G4String symbol, name;
    G4int nComponents, nAtoms;
    G4double temp;

    G4Element* elH  = new G4Element(name = "Hydrogen"   , symbol = "H"  , z = 1.  , a =  1.008*g/mole);
    G4Element* elC  = new G4Element(name = "Carbon"     , symbol = "C"  , z = 6.  , a = 12.011*g/mole);
    G4Element* elN  = new G4Element(name = "Nitrogen"   , symbol = "N"  , z = 7.  , a = 14.007*g/mole);
    G4Element* elO  = new G4Element(name = "Oxygen"     , symbol = "O"  , z = 8.  , a = 15.999*g/mole);
    G4Element* elSi = new G4Element(name = "Silicon"    , symbol = "Si" , z = 14. , a = 28.086*g/mole);
    G4Element* elP  = new G4Element(name = "Phosphorus" , symbol = "P"  , z = 15. , a = 30.974*g/mole);
    G4Element* elS  = new G4Element(name = "Sulfur"     , symbol = "S"  , z = 16. , a = 32.065*g/mole);
    G4Element* elCr = new G4Element(name = "Chromium"   , symbol = "Cr" , z = 24. , a = 51.996*g/mole);
    G4Element* elMn = new G4Element(name = "Manganese"  , symbol = "Mn" , z = 25. , a = 54.938*g/mole);
    G4Element* elFe = new G4Element(name = "Iron"       , symbol = "Fe" , z = 26. , a = 55.845*g/mole);
    G4Element* elNi = new G4Element(name = "Nickel"     , symbol = "Ni" , z = 28. , a = 58.693*g/mole);
    G4Element* elMo = new G4Element(name = "Molybdenum" , symbol = "Mo" , z = 42. , a = 95.94 *g/mole);

    // Liquid helium material
    // Density from (assuming 3K LHe)
    // https://www.bnl.gov/magnets/staff/gupta/cryogenic-data-handbook/Section2.pdf

    new G4Material("liquid_helium",   z=2., a= 4.00*g/mole,  density= 0.141*g/cm3, kStateLiquid, temp=3.*kelvin);
    new G4Material("liquid_nitrogen", z=7., a= 14.00*g/mole, density= 0.807*g/cm3, kStateLiquid, temp=77.*kelvin);

    // Vacuum
    G4Material* Galactic  = new G4Material("galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

    //Stainless Steel Type 304
    G4Material* ss_t316 = new G4Material(name = "ss_t316" , density = 8.03*g/cm3 , nComponents = 10);
    ss_t316 -> AddElement(elC  , fractionMass =   0.08*perCent);
    ss_t316 -> AddElement(elMn , fractionMass =   2.00*perCent);
    ss_t316 -> AddElement(elP  , fractionMass =  0.045*perCent);
    ss_t316 -> AddElement(elS  , fractionMass =  0.030*perCent);
    ss_t316 -> AddElement(elSi , fractionMass =   0.75*perCent);
    ss_t316 -> AddElement(elCr , fractionMass =  17.00*perCent);
    ss_t316 -> AddElement(elNi , fractionMass =  12.00*perCent);
    ss_t316 -> AddElement(elMo , fractionMass =   2.50*perCent);
    ss_t316 -> AddElement(elN  , fractionMass =   0.10*perCent);
    ss_t316 -> AddElement(elFe , fractionMass = 65.495*perCent);

    new G4Material(name = "Cu",z = 29., a =  63.546*g/mole, density = 8.96*g/cm3);

    new G4Material(name = "Al",z = 13., a =  26.981538*g/mole, density = 2.70*g/cm3);

    //Naphthalene
    G4Material* BC501A = new G4Material(name = "BC-501A" , density = 0.874*g/cm3 , nComponents = 2);
    BC501A -> AddElement(elH , nAtoms = 482);
    BC501A -> AddElement(elC  , nAtoms = 398);

    G4Material* Synthetic_Silica = new G4Material(name = "synthetic_silica" , density = 2.65*g/cm3 , nComponents = 2);
    Synthetic_Silica -> AddElement(elSi , nAtoms = 1);
    Synthetic_Silica -> AddElement(elO  , nAtoms = 2);

    G4Material* Generic_Acrylic = new G4Material(name = "generic_acrylic", 1.18*g/cm3, nComponents = 3);
    Generic_Acrylic -> AddElement(elH , nAtoms=8);
    Generic_Acrylic -> AddElement(elC , nAtoms=5);
    Generic_Acrylic -> AddElement(elO , nAtoms=2);

    G4Material* Generic_PMT = new G4Material(name = "generic_pmt", 0.1*g/cm3, nComponents = 2);
    Generic_PMT -> AddMaterial(Galactic , fractionMass=80.0*perCent);
    Generic_PMT -> AddMaterial(ss_t316 , fractionMass=20.0*perCent);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* JanisDetectorConstruction::DefineVolumes()
{
    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    //===============  Materials ===============//

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger

    //
    G4Material* world_material = mat_man -> FindOrBuildMaterial("galactic");

    // Cryostat

    G4Material* default_material = G4Material::GetMaterial("galactic");
    G4Material* liquid_helium_material = G4Material::GetMaterial("liquid_helium");

    G4Material* can_sample_plate_material = G4Material::GetMaterial("ss_t316");
    G4Material* can_sample_ring_material = G4Material::GetMaterial("ss_t316");
    G4Material* can_sample_outer_material = G4Material::GetMaterial("ss_t316");
    G4Material* can_4k_outer_material = G4Material::GetMaterial("Cu");
    G4Material* can_77k_outer_material = G4Material::GetMaterial("Cu");
    G4Material* can_vacuum_outer_material = G4Material::GetMaterial("ss_t316");
    G4Material* arm_vacuum_material = G4Material::GetMaterial("ss_t316");
    G4Material* arm_77k_material = G4Material::GetMaterial("Al");
    G4Material* arm_4k_material = G4Material::GetMaterial("Al");
    G4Material* arm_sample_material = G4Material::GetMaterial("Al");
    G4Material* cryo_vacuum_outer_material = G4Material::GetMaterial("ss_t316");
    G4Material* cryo_77k_outer_material = G4Material::GetMaterial("ss_t316");
    G4Material* cryo_nitrogen_material = G4Material::GetMaterial("liquid_nitrogen");
    G4Material* cryo_4k_outer_material = G4Material::GetMaterial("ss_t316");

    G4Material* can_sample_inner_material = G4Material::GetMaterial("liquid_helium");
    G4Material* can_4k_inner_material = G4Material::GetMaterial("galactic");
    G4Material* can_77k_inner_material = G4Material::GetMaterial("galactic");
    G4Material* can_vacuum_inner_material = G4Material::GetMaterial("galactic");
    G4Material* cryo_vacuum_inner_material = G4Material::GetMaterial("galactic");
    G4Material* cryo_77k_inner_material = G4Material::GetMaterial("galactic");
    G4Material* cryo_4k_inner_material = G4Material::GetMaterial("liquid_helium");

    // Far-side Detector

    G4Material* fs_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fs_head_inner_material = G4Material::GetMaterial("BC-501A");
        //They are hidden because they are not necessary
    //G4Material* fs_neck_outer_material = G4Material::GetMaterial("Al");
    //G4Material* fs_body_outer_material = G4Material::GetMaterial("Al");
    //G4Material* fs_pants_outer_material = G4Material::GetMaterial("Al");
    //G4Material* fs_leg_outer_material = G4Material::GetMaterial("Al");
    //G4Material* fs_foot_outer_material = G4Material::GetMaterial("Al");

    // PMT Related

    G4Material* acrylic_cell_material = G4Material::GetMaterial("generic_acrylic");
    G4Material* pmt_array_material = G4Material::GetMaterial("liquid_helium");

    if ( ! default_material || ! acrylic_cell_material || ! liquid_helium_material ) {
      G4ExceptionDescription msg;
      msg << "Cannot retrieve materials already defined.";
      G4Exception("JanisDetectorConstruction::DefineVolumes()",
        "MyCode0001", FatalException, msg);
    }

    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    //===============  Dimensions ===============//

    // World

    G4double world_x = 5000.*cm;
    G4double world_y = 5000.*cm;
    G4double world_z = 2000.*cm;

    // Cryostat

    G4double can_sample_plate_rMin = 0.0*mm;
    G4double can_sample_plate_rMax = 68.26*mm;
    G4double can_sample_plate_Dz = 6.35*mm;
    G4double can_sample_plate_SPhi = 0.0*deg;
    G4double can_sample_plate_DPhi = 360.0*deg;

    G4double can_sample_ring_rMin = 63.5*mm;
    G4double can_sample_ring_rMax = 68.26*mm;
    G4double can_sample_ring_Dz = 8.97*mm;
    G4double can_sample_ring_SPhi = 0.0*deg;
    G4double can_sample_ring_DPhi = 360.0*deg;

    G4double can_sample_outer_rMin = 0.0*mm;
    G4double can_sample_outer_rMax = 63.5*mm;
    G4double can_sample_outer_Dz = 190.754*mm;
    G4double can_sample_outer_SPhi = 0.0*deg;
    G4double can_sample_outer_DPhi = 360.0*deg;

    G4double can_sample_inner_rMin = 0.0*mm;
    G4double can_sample_inner_rMax = can_sample_outer_rMax - 1.65*mm;
    G4double can_sample_inner_Dz = can_sample_outer_Dz - 4.75*mm;
    G4double can_sample_inner_SPhi = 0.0*deg;
    G4double can_sample_inner_DPhi = 360.0*deg;

    G4double can_4k_outer_rMin = 0.0*mm;
    G4double can_4k_outer_rMax = 70.65*mm;
    G4double can_4k_outer_Dz = 365.125*mm;
    G4double can_4k_outer_SPhi = 0.0*deg;
    G4double can_4k_outer_DPhi = 360.0*deg;

    G4double can_4k_inner_rMin = 0.0*mm;
    G4double can_4k_inner_rMax = can_4k_outer_rMax - 1.5875*mm;
    G4double can_4k_inner_Dz = can_4k_outer_Dz - 1.58*mm;
    G4double can_4k_inner_SPhi = 0.0*deg;
    G4double can_4k_inner_DPhi = 360.0*deg;

    G4double can_77k_outer_rMin = 0.0*mm;
    G4double can_77k_outer_rMax = 76.2*mm;
    G4double can_77k_outer_Dz = 400.05*mm;
    G4double can_77k_outer_SPhi = 0.0*deg;
    G4double can_77k_outer_DPhi = 360.0*deg;

    G4double can_77k_inner_rMin = 0.0*mm;
    G4double can_77k_inner_rMax = can_77k_outer_rMax- 1.3*mm;
    G4double can_77k_inner_Dz = can_77k_outer_Dz - 6.35*mm;
    G4double can_77k_inner_SPhi = 0.0*deg;
    G4double can_77k_inner_DPhi = 360.0*deg;

    G4double can_vacuum_outer_rMin = 0.0*mm;
    G4double can_vacuum_outer_rMax = 101.6*mm;
    G4double can_vacuum_outer_Dz = 450.85*mm;
    G4double can_vacuum_outer_SPhi = 0.0*deg;
    G4double can_vacuum_outer_DPhi = 360.0*deg;

    G4double can_vacuum_inner_rMin = 0.0*mm;
    G4double can_vacuum_inner_rMax = can_vacuum_outer_rMax-1.3*mm;
    G4double can_vacuum_inner_Dz = can_vacuum_outer_Dz-6.35*mm;
    G4double can_vacuum_inner_SPhi = 0.0*deg;
    G4double can_vacuum_inner_DPhi = 360.0*deg;

    G4double arm_vacuum_rMax = 50.8*mm;
    G4double arm_vacuum_rMin = arm_vacuum_rMax - 3.175*mm;
    G4double arm_vacuum_Dz = 304.8*mm;
    G4double arm_vacuum_SPhi = 0.0*deg;
    G4double arm_vacuum_DPhi = 360.0*deg;

    G4double arm_77k_rMax = 44.45*mm;
    G4double arm_77k_rMin = arm_77k_rMax - 1.58*mm;
    G4double arm_77k_Dz = arm_vacuum_Dz;
    G4double arm_77k_SPhi = 0.0*deg;
    G4double arm_77k_DPhi = 360.0*deg;

    G4double arm_4k_rMax = 38.1*mm;
    G4double arm_4k_rMin = arm_4k_rMax - 1.57*mm;
    G4double arm_4k_Dz = arm_vacuum_Dz;
    G4double arm_4k_SPhi = 0.0*deg;
    G4double arm_4k_DPhi = 360.0*deg;

    G4double arm_sample_rMax = 12.67*mm;
    G4double arm_sample_rMin = arm_sample_rMax - 1.2446*mm;
    G4double arm_sample_Dz = arm_vacuum_Dz;
    G4double arm_sample_SPhi = 0.0*deg;
    G4double arm_sample_DPhi = 360.0*deg;

    G4double cryo_vacuum_outer_rMin = 0.0*mm;
    G4double cryo_vacuum_outer_rMax = 209.6*mm;
    G4double cryo_vacuum_outer_Dz = 889.0*mm;
    G4double cryo_vacuum_outer_SPhi = 0.0*deg;
    G4double cryo_vacuum_outer_DPhi = 360.0*deg;

    G4double cryo_vacuum_inner_rMin = 0.0*mm;
    G4double cryo_vacuum_inner_rMax = cryo_vacuum_outer_rMax - 5.0*mm;
    G4double cryo_vacuum_inner_Dz = cryo_vacuum_outer_Dz - 10.0*mm;
    G4double cryo_vacuum_inner_SPhi = 0.0*deg;
    G4double cryo_vacuum_inner_DPhi = 360.0*deg;

    G4double cryo_77k_outer_rMin = 0.0*mm;
    G4double cryo_77k_outer_rMax = 184.2*mm;
    G4double cryo_77k_outer_Dz = 762.0*mm;
    G4double cryo_77k_outer_SPhi = 0.0*deg;
    G4double cryo_77k_outer_DPhi = 360.0*deg;

    G4double cryo_nitrogen_rMin = 152.4*mm;
    G4double cryo_nitrogen_rMax = cryo_77k_outer_rMax - 5.0*mm;
    G4double cryo_nitrogen_Dz = cryo_77k_outer_Dz - 25.4*mm;
    G4double cryo_nitrogen_SPhi = 0.0*deg;
    G4double cryo_nitrogen_DPhi = 360.0*deg;

    G4double cryo_77k_inner_rMin = 0.0*mm;
    G4double cryo_77k_inner_rMax = cryo_nitrogen_rMin - 5.0*mm;
    G4double cryo_77k_inner_Dz = cryo_77k_outer_Dz - 10.0*mm;
    G4double cryo_77k_inner_SPhi = 0.0*deg;
    G4double cryo_77k_inner_DPhi = 360.0*deg;

    G4double cryo_4k_outer_rMin = 0.0*mm;
    G4double cryo_4k_outer_rMax = 133.4*mm;
    G4double cryo_4k_outer_Dz = 508.0*mm;
    G4double cryo_4k_outer_SPhi = 0.0*deg;
    G4double cryo_4k_outer_DPhi = 360.0*deg;

    G4double cryo_4k_inner_rMin = 0.0*mm;
    G4double cryo_4k_inner_rMax = cryo_4k_outer_rMax - 5.0*mm;
    G4double cryo_4k_inner_Dz = cryo_4k_outer_Dz - 10.0*mm;
    G4double cryo_4k_inner_SPhi = 0.0*deg;
    G4double cryo_4k_inner_DPhi = 360.0*deg;

    // Liquid Helium Size (one of our sensitive volumes)

    G4double liquid_helium_sizeX = 25.7*mm;
    G4double liquid_helium_sizeY = 25.7*mm;
    G4double liquid_helium_sizeZ = 25.7*mm;

    // Acrylic cell around liquid helium

    G4double acrylic_cell_wall_thickness = 0.5*mm;

    G4double acrylic_cell_exterior_sizeX = liquid_helium_sizeX+(2*acrylic_cell_wall_thickness);
    G4double acrylic_cell_exterior_sizeY = liquid_helium_sizeY+(2*acrylic_cell_wall_thickness);
    G4double acrylic_cell_exterior_sizeZ = liquid_helium_sizeZ+(2*acrylic_cell_wall_thickness);

    // PMT

    G4double pmt_sizeX = 25.7*mm;
    G4double pmt_sizeY = 25.7*mm;
    G4double pmt_sizeZ = 29.45*mm;

    // PMT Array (Mother volume for the six PMTs)

    G4double pmt_array_size_xyz = 1*mm + (pmt_sizeZ*2 + acrylic_cell_exterior_sizeZ);

    // Far-side Detector

    G4double fs_head_outer_rMin = 0.0*mm;
    G4double fs_head_outer_rMax = 66.675*mm;
    G4double fs_head_outer_Dz = 133.35*mm;
    G4double fs_head_outer_SPhi = 0.0*deg;
    G4double fs_head_outer_DPhi = 360.0*deg;

    G4double fs_head_inner_rMin = 0.0*mm;
    G4double fs_head_inner_rMax = 63.5*mm;
    G4double fs_head_inner_Dz = 12.7*cm;
    G4double fs_head_inner_SPhi = 0.0*deg;
    G4double fs_head_inner_DPhi = 360.0*deg;

    //They are hidden because they are not necessary
    /*
    G4double fs_neck_outer_rMin = 0.0*mm;
    G4double fs_neck_outer_rMax = 82.55*mm;
    G4double fs_neck_outer_Dz = 2.065*cm;
    G4double fs_neck_outer_SPhi = 0.0*deg;
    G4double fs_neck_outer_DPhi = 360.0*deg;

    G4double fs_body_outer_rMin = 0.0*mm;
    G4double fs_body_outer_rMax = 7.145*cm;
    G4double fs_body_outer_Dz = 133.35*mm;
    G4double fs_body_outer_SPhi = 0.0*deg;
    G4double fs_body_outer_DPhi = 360.0*deg;

    G4double fs_pants_outer_rMin1 = 0.0*mm;
    G4double fs_pants_outer_rMax1 = 71.45*mm;
    G4double fs_pants_outer_rMin2 = 0.0*mm;
    G4double fs_pants_outer_rMax2 = 50.8*mm;
    G4double fs_pants_outer_Dz = 28.575*mm;
    G4double fs_pants_outer_SPhi = 0.0*deg;
    G4double fs_pants_outer_DPhi = 360.0*deg;

    G4double fs_leg_outer_rMin = 0.0*mm;
    G4double fs_leg_outer_rMax = 50.8*mm;
    G4double fs_leg_outer_Dz = 85.725*mm;
    G4double fs_leg_outer_SPhi = 0.0*deg;
    G4double fs_leg_outer_DPhi = 360.0*deg;

    G4double fs_foot_outer_rMin = 0.0*mm;
    G4double fs_foot_outer_rMax = 2.54*cm;
    G4double fs_foot_outer_Dz = 85.725*mm;
    G4double fs_foot_outer_SPhi = 0.0*deg;
    G4double fs_foot_outer_DPhi = 360.0*deg;
    */

    //===============  Positions ===============//

    // Cryostat

    G4double can_sample_plate_posX = 0.0*mm;
    G4double can_sample_plate_posY = 0.0*mm;
    G4double can_sample_plate_posZ = (can_sample_outer_Dz + can_sample_plate_Dz)/2.0;

    G4double can_sample_ring_posX = 0.0*mm;
    G4double can_sample_ring_posY = 0.0*mm;
    G4double can_sample_ring_posZ = (can_sample_outer_Dz - can_sample_ring_Dz)/2.0;

    G4double can_sample_outer_posX = 0.0*mm;
    G4double can_sample_outer_posY = 0.0*mm;
    G4double can_sample_outer_posZ = (can_4k_outer_Dz - can_sample_outer_Dz)/2.0 - 146.05*mm;

    G4double can_sample_inner_posX = 0.0*mm;
    G4double can_sample_inner_posY = 0.0*mm;
    G4double can_sample_inner_posZ = 4.75/2.0*mm;

    G4double can_4k_outer_posX = 0.0*mm;
    G4double can_4k_outer_posY = 0.0*mm;
    G4double can_4k_outer_posZ = (can_77k_outer_Dz - can_4k_outer_Dz)/2.0 - 31.75*mm;

    G4double can_4k_inner_posX = 0.0*mm;
    G4double can_4k_inner_posY = 0.0*mm;
    G4double can_4k_inner_posZ = 1.57/2.0*mm;

    G4double can_77k_outer_posX = 0.0*mm;
    G4double can_77k_outer_posY = 0.0*mm;
    G4double can_77k_outer_posZ = (can_vacuum_outer_Dz - can_77k_outer_Dz)/2.0 - 31.75*mm; // problematic

    G4double can_77k_inner_posX = 0.0*mm;
    G4double can_77k_inner_posY = 0.0*mm;
    G4double can_77k_inner_posZ = 6.35/2.0*mm;

    G4double can_vacuum_outer_posX = 0.0*mm;
    G4double can_vacuum_outer_posY = 0.0*mm;
    G4double can_vacuum_outer_posZ = 0.0*mm;

    G4double can_vacuum_inner_posX = 0.0*mm;
    G4double can_vacuum_inner_posY = 0.0*mm;
    G4double can_vacuum_inner_posZ = 6.35/2.0*mm;

    G4double arm_vacuum_posX = can_vacuum_outer_posX;
    G4double arm_vacuum_posY = can_vacuum_outer_posY;
    G4double arm_vacuum_posZ = can_vacuum_outer_posZ + (can_vacuum_outer_Dz/2.0) + (arm_vacuum_Dz/2.0);

    G4double arm_77k_posX = arm_vacuum_posX;
    G4double arm_77k_posY = arm_vacuum_posY;
    G4double arm_77k_posZ = arm_vacuum_posZ;

    G4double arm_4k_posX = arm_vacuum_posX;
    G4double arm_4k_posY = arm_vacuum_posY;
    G4double arm_4k_posZ = arm_vacuum_posZ;

    G4double arm_sample_posX = arm_vacuum_posX;
    G4double arm_sample_posY = arm_vacuum_posY;
    G4double arm_sample_posZ = arm_vacuum_posZ;

    G4double cryo_vacuum_outer_posX = can_vacuum_outer_posX;
    G4double cryo_vacuum_outer_posY = can_vacuum_outer_posY;
    G4double cryo_vacuum_outer_posZ = arm_vacuum_posZ + (arm_vacuum_Dz/2.0) + (cryo_vacuum_outer_Dz/2.0);

    G4double cryo_vacuum_inner_posX = 0.0*mm;
    G4double cryo_vacuum_inner_posY = 0.0*mm;
    G4double cryo_vacuum_inner_posZ = 0.0*mm;

    G4double cryo_77k_outer_posX = 0.0*mm;
    G4double cryo_77k_outer_posY = 0.0*mm;
    G4double cryo_77k_outer_posZ = -38.1*mm;

    G4double cryo_nitrogen_posX = 0.0*mm;
    G4double cryo_nitrogen_posY = 0.0*mm;
    G4double cryo_nitrogen_posZ = 0.0*mm;

    G4double cryo_77k_inner_posX = 0.0*mm;
    G4double cryo_77k_inner_posY = 0.0*mm;
    G4double cryo_77k_inner_posZ = 0.0*mm;

    G4double cryo_4k_outer_posX = 0.0*mm;
    G4double cryo_4k_outer_posY = 0.0*mm;
    G4double cryo_4k_outer_posZ = 25.4*mm;

    G4double cryo_4k_inner_posX = 0.0*mm;
    G4double cryo_4k_inner_posY = 0.0*mm;
    G4double cryo_4k_inner_posZ = 0.0*mm;


    // PMT Array Position

    G4double pmt_array_posX = 0*mm;
    G4double pmt_array_posY = 0*mm;
    G4double pmt_array_posZ = -25.33*mm;

    // PMT Positions

    G4double pmt1_posX = acrylic_cell_exterior_sizeX/2.0 + pmt_sizeZ/2.0*mm;
    G4double pmt1_posY = 0*mm;
    G4double pmt1_posZ = 0*mm;

    G4double pmt2_posX = -(acrylic_cell_exterior_sizeX/2.0 + pmt_sizeZ/2.0)*mm;
    G4double pmt2_posY = 0*mm;
    G4double pmt2_posZ = 0*mm;

    G4double pmt3_posX = 0*mm;
    G4double pmt3_posY = acrylic_cell_exterior_sizeY/2.0 + pmt_sizeZ/2.0*mm;
    G4double pmt3_posZ = 0*mm;

    G4double pmt4_posX = 0*mm;
    G4double pmt4_posY = -(acrylic_cell_exterior_sizeY/2.0 + pmt_sizeZ/2.0)*mm;
    G4double pmt4_posZ = 0*mm;

    G4double pmt5_posX = 0*mm;
    G4double pmt5_posY = 0*mm;
    G4double pmt5_posZ = acrylic_cell_exterior_sizeZ/2.0 + pmt_sizeZ/2.0*mm;

    G4double pmt6_posX = 0*mm;
    G4double pmt6_posY = 0*mm;
    G4double pmt6_posZ = -(acrylic_cell_exterior_sizeZ/2.0 + pmt_sizeZ/2.0)*mm;


    // Far-side Detector

        // Please modify here for Far-side detector placement
    G4double fs_placement_angle = 0.0*deg;
    G4double fs_placement_height = -10.0*cm;
    G4double fs_placement_distance = 140*cm;

    G4double fs_head_outer_posX = 0.0*mm;
    G4double fs_head_outer_posY = 0.0*mm;
    G4double fs_head_outer_posZ = 0.0*mm;

    G4double fs_head_inner_posX = fs_placement_distance * cos(fs_placement_angle);
    G4double fs_head_inner_posY = fs_placement_distance * sin(fs_placement_angle);
    G4double fs_head_inner_posZ = fs_placement_height;

    /*
        //They are hidden because they are not necessary
    G4double fs_neck_outer_posX = 0.0*mm;
    G4double fs_neck_outer_posY = 0.0*mm;
    G4double fs_neck_outer_posZ = (fs_neck_outer_Dz + fs_head_outer_Dz)/2;

    G4double fs_body_outer_posX = 0.0*mm;
    G4double fs_body_outer_posY = 0.0*mm;
    G4double fs_body_outer_posZ = (fs_neck_outer_Dz + fs_body_outer_Dz)/2;

    G4double fs_pants_outer_posX = 0.0*mm;
    G4double fs_pants_outer_posY = 0.0*mm;
    G4double fs_pants_outer_posZ = (fs_pants_outer_Dz + fs_body_outer_Dz)/2;

    G4double fs_leg_outer_posX = 0.0*mm;
    G4double fs_leg_outer_posY = 0.0*mm;
    G4double fs_leg_outer_posZ = (fs_pants_outer_Dz + fs_leg_outer_Dz)/2;

    G4double fs_foot_outer_posX = 0.0*mm;
    G4double fs_foot_outer_posY = 0.0*mm;
    G4double fs_foot_outer_posZ = (fs_foot_outer_Dz + fs_leg_outer_Dz)/2;
    */

    //===============  Rotations ===============//

    // Cryostat

    G4RotationMatrix* can_sample_plate_rm = new G4RotationMatrix;
    G4RotationMatrix* can_sample_ring_rm = new G4RotationMatrix;
    G4RotationMatrix* can_sample_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* can_4k_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* can_77k_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* can_vacuum_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* arm_vacuum_rm = new G4RotationMatrix;
    G4RotationMatrix* arm_77k_rm = new G4RotationMatrix;
    G4RotationMatrix* arm_4k_rm = new G4RotationMatrix;
    G4RotationMatrix* arm_sample_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_vacuum_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_77k_outer_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_nitrogen_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_4k_outer_rm = new G4RotationMatrix;

    G4RotationMatrix* can_sample_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* can_4k_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* can_77k_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* can_vacuum_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_vacuum_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_77k_inner_rm = new G4RotationMatrix;
    G4RotationMatrix* cryo_4k_inner_rm = new G4RotationMatrix;

    // PMT
    G4RotationMatrix *NO_ROT = new G4RotationMatrix;
    G4RotationMatrix *pmt1_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt2_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt3_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt4_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt5_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt6_rm = new G4RotationMatrix;
    G4RotationMatrix *pmt_array_rm = new G4RotationMatrix;

    // Far-side Detector
    G4RotationMatrix *fs_head_outer_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_inner_rm = G4RotationMatrix();
    /*
        //They are hidden because they are not necessary
    G4RotationMatrix *fs_neck_outer_rm = new G4RotationMatrix;
    G4RotationMatrix *fs_body_outer_rm = new G4RotationMatrix;
    G4RotationMatrix *fs_pants_outer_rm = new G4RotationMatrix;
    G4RotationMatrix *fs_leg_outer_rm = new G4RotationMatrix;
    G4RotationMatrix *fs_foot_outer_rm = new G4RotationMatrix;
    */

    // Rotate the various PMTs so that they all point inward
    pmt1_rm->rotateY(90.0*deg);
    pmt2_rm->rotateY(-90.0*deg);
    pmt3_rm->rotateX(-90.0*deg);
    pmt4_rm->rotateX(90.0*deg);
    pmt5_rm->rotateX(180.0*deg);
    pmt6_rm->rotateX(0*deg);

    // Rotate the Far-side Detector
    fs_head_inner_rm.rotateY(270.0*deg);
    fs_head_inner_rm.rotateZ(fs_placement_angle);

    //===============  Build Geometry ===============//

    G4String name;

    // World

    name = "world_box";
    G4Box* world_box = new G4Box(name, world_x/2.0, world_y/2.0, world_z/2.0);
    WorldLV = new G4LogicalVolume(world_box, world_material, "WorldLV");
    WorldPV = new G4PVPlacement(0, G4ThreeVector(0,0,0), WorldLV, name, 0, false, 0, checkOverlaps);

    // Cryostat

    name = "can_vacuum_outer";
    G4Tubs* can_vacuum_outer_S = new G4Tubs(name, can_vacuum_outer_rMin, can_vacuum_outer_rMax, can_vacuum_outer_Dz/2.0, can_vacuum_outer_SPhi, can_vacuum_outer_DPhi);
    G4LogicalVolume* can_vacuum_outer_LV = new G4LogicalVolume(can_vacuum_outer_S, can_vacuum_outer_material, name);
    new G4PVPlacement(can_vacuum_outer_rm, G4ThreeVector(can_vacuum_outer_posX,can_vacuum_outer_posY,can_vacuum_outer_posZ), can_vacuum_outer_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "can_vacuum_inner";
    G4Tubs* can_vacuum_inner_S = new G4Tubs(name, can_vacuum_inner_rMin, can_vacuum_inner_rMax, can_vacuum_inner_Dz/2.0, can_vacuum_inner_SPhi, can_vacuum_inner_DPhi);
    G4LogicalVolume* can_vacuum_inner_LV = new G4LogicalVolume(can_vacuum_inner_S, can_vacuum_inner_material, name);
    new G4PVPlacement(can_vacuum_inner_rm, G4ThreeVector(can_vacuum_inner_posX,can_vacuum_inner_posY,can_vacuum_inner_posZ), can_vacuum_inner_LV, name, can_vacuum_outer_LV, false, 0, fCheckOverlaps);

    name = "can_77k_outer";
    G4Tubs* can_77k_outer_S = new G4Tubs(name, can_77k_outer_rMin, can_77k_outer_rMax, can_77k_outer_Dz/2.0, can_77k_outer_SPhi, can_77k_outer_DPhi);
    G4LogicalVolume* can_77k_outer_LV = new G4LogicalVolume(can_77k_outer_S, can_77k_outer_material, name);
    new G4PVPlacement(can_77k_outer_rm, G4ThreeVector(can_77k_outer_posX,can_77k_outer_posY,can_77k_outer_posZ), can_77k_outer_LV, name, can_vacuum_inner_LV, false, 0, fCheckOverlaps);

    name = "can_77k_inner";
    G4Tubs* can_77k_inner_S = new G4Tubs(name, can_77k_inner_rMin, can_77k_inner_rMax, can_77k_inner_Dz/2.0, can_77k_inner_SPhi, can_77k_inner_DPhi);
    G4LogicalVolume* can_77k_inner_LV = new G4LogicalVolume(can_77k_inner_S, can_77k_inner_material, name);
    new G4PVPlacement(can_77k_inner_rm, G4ThreeVector(can_77k_inner_posX,can_77k_inner_posY,can_77k_inner_posZ), can_77k_inner_LV, name, can_77k_outer_LV, false, 0, fCheckOverlaps);

    name = "can_4k_outer";
    G4Tubs* can_4k_outer_S = new G4Tubs(name, can_4k_outer_rMin, can_4k_outer_rMax, can_4k_outer_Dz/2.0, can_4k_outer_SPhi, can_4k_outer_DPhi);
    G4LogicalVolume* can_4k_outer_LV = new G4LogicalVolume(can_4k_outer_S, can_4k_outer_material, name);
    new G4PVPlacement(can_4k_outer_rm, G4ThreeVector(can_4k_outer_posX,can_4k_outer_posY,can_4k_outer_posZ), can_4k_outer_LV, name, can_77k_inner_LV, false, 0, fCheckOverlaps);

    name = "can_4k_inner";
    G4Tubs* can_4k_inner_S = new G4Tubs(name, can_4k_inner_rMin, can_4k_inner_rMax, can_4k_inner_Dz/2.0, can_4k_inner_SPhi, can_4k_inner_DPhi);
    G4LogicalVolume* can_4k_inner_LV = new G4LogicalVolume(can_4k_inner_S, can_4k_inner_material, name);
    new G4PVPlacement(can_4k_inner_rm, G4ThreeVector(can_4k_inner_posX,can_4k_inner_posY,can_4k_inner_posZ), can_4k_inner_LV, name, can_4k_outer_LV, false, 0, fCheckOverlaps);

    name = "can_sample_outer";
    G4Tubs* can_sample_outer_S = new G4Tubs(name, can_sample_outer_rMin, can_sample_outer_rMax, can_sample_outer_Dz/2.0, can_sample_outer_SPhi, can_sample_outer_DPhi);
    G4LogicalVolume* can_sample_outer_LV = new G4LogicalVolume(can_sample_outer_S, can_sample_outer_material, name);
    new G4PVPlacement(can_sample_outer_rm, G4ThreeVector(can_sample_outer_posX,can_sample_outer_posY,can_sample_outer_posZ), can_sample_outer_LV, name, can_4k_inner_LV, false, 0, fCheckOverlaps);

    name = "can_sample_inner";
    G4Tubs* can_sample_inner_S = new G4Tubs(name, can_sample_inner_rMin, can_sample_inner_rMax, can_sample_inner_Dz/2.0, can_sample_inner_SPhi, can_sample_inner_DPhi);
    G4LogicalVolume* can_sample_inner_LV = new G4LogicalVolume(can_sample_inner_S, can_sample_inner_material, name);
    new G4PVPlacement(can_sample_inner_rm, G4ThreeVector(can_sample_inner_posX,can_sample_inner_posY,can_sample_inner_posZ), can_sample_inner_LV, name, can_sample_outer_LV, false, 0, fCheckOverlaps);

    name = "can_sample_ring";
    G4Tubs* can_sample_ring_S = new G4Tubs(name, can_sample_ring_rMin, can_sample_ring_rMax, can_sample_ring_Dz/2.0, can_sample_ring_SPhi, can_sample_ring_DPhi);
    G4LogicalVolume* can_sample_ring_LV = new G4LogicalVolume(can_sample_ring_S, can_sample_ring_material, name);
    new G4PVPlacement(can_sample_ring_rm, G4ThreeVector(can_sample_ring_posX, can_sample_ring_posY, can_sample_ring_posZ), can_sample_ring_LV, name, can_sample_outer_LV, false, 0, fCheckOverlaps);

    name = "can_sample_plate";
    G4Tubs* can_sample_plate_S = new G4Tubs(name, can_sample_plate_rMin, can_sample_plate_rMax, can_sample_plate_Dz/2.0, can_sample_plate_SPhi, can_sample_plate_DPhi);
    G4LogicalVolume* can_sample_plate_LV = new G4LogicalVolume(can_sample_plate_S, can_sample_plate_material, name);
    new G4PVPlacement(can_sample_plate_rm, G4ThreeVector(can_sample_plate_posX, can_sample_plate_posY, can_sample_plate_posZ), can_sample_plate_LV, name, can_sample_outer_LV, false, 0, fCheckOverlaps);

    name = "arm_vacuum";
    G4Tubs* arm_vacuum_S = new G4Tubs(name, arm_vacuum_rMin, arm_vacuum_rMax, arm_vacuum_Dz/2.0, arm_vacuum_SPhi, arm_vacuum_DPhi);
    G4LogicalVolume* arm_vacuum_LV = new G4LogicalVolume(arm_vacuum_S, arm_vacuum_material, name);
    new G4PVPlacement(arm_vacuum_rm, G4ThreeVector(arm_vacuum_posX,arm_vacuum_posY,arm_vacuum_posZ), arm_vacuum_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "arm_77k";
    G4Tubs* arm_77k_S = new G4Tubs(name, arm_77k_rMin, arm_77k_rMax, arm_77k_Dz/2.0, arm_77k_SPhi, arm_77k_DPhi);
    G4LogicalVolume* arm_77k_LV = new G4LogicalVolume(arm_77k_S, arm_77k_material, name);
    new G4PVPlacement(arm_77k_rm, G4ThreeVector(arm_77k_posX,arm_77k_posY,arm_77k_posZ), arm_77k_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "arm_4k";
    G4Tubs* arm_4k_S = new G4Tubs(name, arm_4k_rMin, arm_4k_rMax, arm_4k_Dz/2.0, arm_4k_SPhi, arm_4k_DPhi);
    G4LogicalVolume* arm_4k_LV = new G4LogicalVolume(arm_4k_S, arm_4k_material, name);
    new G4PVPlacement(arm_4k_rm, G4ThreeVector(arm_4k_posX,arm_4k_posY,arm_4k_posZ), arm_4k_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "arm_sample";
    G4Tubs* arm_sample_S = new G4Tubs(name, arm_sample_rMin, arm_sample_rMax, arm_sample_Dz/2.0, arm_sample_SPhi, arm_sample_DPhi);
    G4LogicalVolume* arm_sample_LV = new G4LogicalVolume(arm_sample_S, arm_sample_material, name);
    new G4PVPlacement(arm_sample_rm, G4ThreeVector(arm_sample_posX,arm_sample_posY,arm_sample_posZ), arm_sample_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "cryo_vacuum_outer";
    G4Tubs* cryo_vacuum_outer_S = new G4Tubs(name, cryo_vacuum_outer_rMin, cryo_vacuum_outer_rMax, cryo_vacuum_outer_Dz/2.0, cryo_vacuum_outer_SPhi, cryo_vacuum_outer_DPhi);
    G4LogicalVolume* cryo_vacuum_outer_LV = new G4LogicalVolume(cryo_vacuum_outer_S, cryo_vacuum_outer_material, name);
    new G4PVPlacement(cryo_vacuum_outer_rm, G4ThreeVector(cryo_vacuum_outer_posX,cryo_vacuum_outer_posY,cryo_vacuum_outer_posZ), cryo_vacuum_outer_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "cryo_vacuum_inner";
    G4Tubs* cryo_vacuum_inner_S = new G4Tubs(name, cryo_vacuum_inner_rMin, cryo_vacuum_inner_rMax, cryo_vacuum_inner_Dz/2.0, cryo_vacuum_inner_SPhi, cryo_vacuum_inner_DPhi);
    G4LogicalVolume* cryo_vacuum_inner_LV = new G4LogicalVolume(cryo_vacuum_inner_S, cryo_vacuum_inner_material, name);
    new G4PVPlacement(cryo_vacuum_inner_rm, G4ThreeVector(cryo_vacuum_inner_posX,cryo_vacuum_inner_posY,cryo_vacuum_inner_posZ), cryo_vacuum_inner_LV, name, cryo_vacuum_outer_LV, false, 0, fCheckOverlaps);

    name = "cryo_77k_outer";
    G4Tubs* cryo_77k_outer_S = new G4Tubs(name, cryo_77k_outer_rMin, cryo_77k_outer_rMax, cryo_77k_outer_Dz/2.0, cryo_77k_outer_SPhi, cryo_77k_outer_DPhi);
    G4LogicalVolume* cryo_77k_outer_LV = new G4LogicalVolume(cryo_77k_outer_S, cryo_77k_outer_material, name);
    new G4PVPlacement(cryo_77k_outer_rm, G4ThreeVector(cryo_77k_outer_posX,cryo_77k_outer_posY,cryo_77k_outer_posZ), cryo_77k_outer_LV, name, cryo_vacuum_inner_LV, false, 0, fCheckOverlaps);

    name = "cryo_nitrogen";
    G4Tubs* cryo_nitrogen_S = new G4Tubs(name, cryo_nitrogen_rMin, cryo_nitrogen_rMax, cryo_nitrogen_Dz/2.0, cryo_nitrogen_SPhi, cryo_nitrogen_DPhi);
    G4LogicalVolume* cryo_nitrogen_LV = new G4LogicalVolume(cryo_nitrogen_S, cryo_nitrogen_material, name);
    new G4PVPlacement(cryo_nitrogen_rm, G4ThreeVector(cryo_nitrogen_posX,cryo_nitrogen_posY,cryo_nitrogen_posZ), cryo_nitrogen_LV, name, cryo_77k_outer_LV, false, 0, fCheckOverlaps);

    name = "cryo_77k_inner";
    G4Tubs* cryo_77k_inner_S = new G4Tubs(name, cryo_77k_inner_rMin, cryo_77k_inner_rMax, cryo_77k_inner_Dz/2.0, cryo_77k_inner_SPhi, cryo_77k_inner_DPhi);
    G4LogicalVolume* cryo_77k_inner_LV = new G4LogicalVolume(cryo_77k_inner_S, cryo_77k_inner_material, name);
    new G4PVPlacement(cryo_77k_inner_rm, G4ThreeVector(cryo_77k_inner_posX,cryo_77k_inner_posY,cryo_77k_inner_posZ), cryo_77k_inner_LV, name, cryo_77k_outer_LV, false, 0, fCheckOverlaps);

    name = "cryo_4k_outer";
    G4Tubs* cryo_4k_outer_S = new G4Tubs(name, cryo_4k_outer_rMin, cryo_4k_outer_rMax, cryo_4k_outer_Dz/2.0, cryo_4k_outer_SPhi, cryo_4k_outer_DPhi);
    G4LogicalVolume* cryo_4k_outer_LV = new G4LogicalVolume(cryo_4k_outer_S, cryo_4k_outer_material, name);
    new G4PVPlacement(cryo_4k_outer_rm, G4ThreeVector(cryo_4k_outer_posX,cryo_4k_outer_posY,cryo_4k_outer_posZ), cryo_4k_outer_LV, name, cryo_77k_inner_LV, false, 0, fCheckOverlaps);

    name = "cryo_4k_inner";
    G4Tubs* cryo_4k_inner_S = new G4Tubs(name, cryo_4k_inner_rMin, cryo_4k_inner_rMax, cryo_4k_inner_Dz/2.0, cryo_4k_inner_SPhi, cryo_4k_inner_DPhi);
    G4LogicalVolume* cryo_4k_inner_LV = new G4LogicalVolume(cryo_4k_inner_S, cryo_4k_inner_material, name);
    new G4PVPlacement(cryo_4k_inner_rm, G4ThreeVector(cryo_4k_inner_posX,cryo_4k_inner_posY,cryo_4k_inner_posZ), cryo_4k_inner_LV, name, cryo_4k_outer_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* helium_vis = new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.75));
    G4VisAttributes* nitrogen_vis = new G4VisAttributes(G4Colour(0.0,1.0,1.0,0.75));
    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);

    // PMT Array (Uses function PlacePMT to place individual PMTs)

    name = "pmt_array";
    G4VSolid* pmt_array_S = new G4Box(name, pmt_array_size_xyz/2., pmt_array_size_xyz/2., pmt_array_size_xyz/2.);
    G4LogicalVolume* pmt_array_LV = new G4LogicalVolume(pmt_array_S, pmt_array_material, name);
    new G4PVPlacement(pmt_array_rm, G4ThreeVector(pmt_array_posX,pmt_array_posY,pmt_array_posZ), pmt_array_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt1_posX,pmt1_posY,pmt1_posZ,pmt1_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt2_posX,pmt2_posY,pmt2_posZ,pmt2_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt3_posX,pmt3_posY,pmt3_posZ,pmt3_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt4_posX,pmt4_posY,pmt4_posZ,pmt4_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt5_posX,pmt5_posY,pmt5_posZ,pmt5_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt6_posX,pmt6_posY,pmt6_posZ,pmt6_rm);

    // Acrylic cell

    name = "acrylic_cell";
    G4VSolid* acrylic_cell_exterior_S = new G4Box(name, acrylic_cell_exterior_sizeX/2., acrylic_cell_exterior_sizeY/2., acrylic_cell_exterior_sizeZ/2.);
    G4LogicalVolume* acrylic_cell_LV = new G4LogicalVolume(acrylic_cell_exterior_S, acrylic_cell_material, name);
    new G4PVPlacement(NO_ROT,G4ThreeVector(), acrylic_cell_LV, name, pmt_array_LV, false, 0, fCheckOverlaps);

    // Liquid Helium

    name = "liquid helium";
    G4VSolid* liquid_helium_S = new G4Box(name, liquid_helium_sizeX/2., liquid_helium_sizeY/2., liquid_helium_sizeZ/2.);
    G4LogicalVolume* liquid_helium_LV = new G4LogicalVolume(liquid_helium_S, liquid_helium_material, name);
    liquid_helium_PV = new G4PVPlacement(NO_ROT, G4ThreeVector(), liquid_helium_LV, name, acrylic_cell_LV, false, 0, fCheckOverlaps);

    // Far-side Detector

    name = 'fs_head_inner';
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    G4LogicalVolume* fs_head_inner_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    G4Transform3D fs_head_inner_transform(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ));
    G4VPhysicalVolume* fs_head_inner_PV = new G4PVPlacement(fs_head_inner_transform, fs_head_inner_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = 'fs_head_outer';
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    G4LogicalVolume* fs_head_outer_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4VPhysicalVolume* fs_head_outer_PV = new G4PVPlacement(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ), fs_head_outer_LV, name, fs_head_inner_LV, false, 0, fCheckOverlaps);

    /*
    name = 'fs_neck_outer';
    G4Tubs* fs_neck_outer_S = new G4Tubs(name, fs_neck_outer_rMin, fs_neck_outer_rMax, fs_neck_outer_Dz/2.0, fs_neck_outer_SPhi, fs_neck_outer_DPhi);
    G4LogicalVolume* fs_neck_outer_LV = new G4LogicalVolume(fs_neck_outer_S, fs_neck_outer_material, name);
    new G4PVPlacement(fs_neck_outer_rm, G4ThreeVector(fs_neck_outer_posX,fs_neck_outer_posY,fs_neck_outer_posZ), fs_neck_outer_LV, name, fs_head_outer_LV, false, 0, fCheckOverlaps);

    name = 'fs_body_outer';
    G4Tubs* fs_body_outer_S = new G4Tubs(name, fs_body_outer_rMin, fs_body_outer_rMax, fs_body_outer_Dz/2.0, fs_body_outer_SPhi, fs_body_outer_DPhi);
    G4LogicalVolume* fs_body_outer_LV = new G4LogicalVolume(fs_body_outer_S, fs_body_outer_material, name);
    new G4PVPlacement(fs_body_outer_rm, G4ThreeVector(fs_body_outer_posX,fs_body_outer_posY,fs_body_outer_posZ), fs_body_outer_LV, name, fs_neck_outer_LV, false, 0, fCheckOverlaps);

    name = 'fs_pants_outer';
    G4Cons* fs_pants_outer_S = new G4Cons(name, fs_pants_outer_rMin1, fs_pants_outer_rMax1, fs_pants_outer_rMin2, fs_pants_outer_rMax2, fs_pants_outer_Dz/2.0, fs_pants_outer_SPhi, fs_pants_outer_DPhi);
    G4LogicalVolume* fs_pants_outer_LV = new G4LogicalVolume(fs_pants_outer_S, fs_pants_outer_material, name);
    new G4PVPlacement(fs_pants_outer_rm, G4ThreeVector(fs_pants_outer_posX,fs_pants_outer_posY,fs_pants_outer_posZ), fs_pants_outer_LV, name, fs_body_outer_LV, false, 0, fCheckOverlaps);

    name = 'fs_leg_outer';
    G4Tubs* fs_leg_outer_S = new G4Tubs(name, fs_leg_outer_rMin, fs_leg_outer_rMax, fs_leg_outer_Dz/2.0, fs_leg_outer_SPhi, fs_leg_outer_DPhi);
    G4LogicalVolume* fs_leg_outer_LV = new G4LogicalVolume(fs_leg_outer_S, fs_leg_outer_material, name);
    new G4PVPlacement(fs_leg_outer_rm, G4ThreeVector(fs_leg_outer_posX,fs_leg_outer_posY,fs_leg_outer_posZ), fs_leg_outer_LV, name, fs_pants_outer_LV, false, 0, fCheckOverlaps);

    name = 'fs_foot_outer';
    G4Tubs* fs_foot_outer_S = new G4Tubs(name, fs_foot_outer_rMin, fs_foot_outer_rMax, fs_foot_outer_Dz/2.0, fs_foot_outer_SPhi, fs_foot_outer_DPhi);
    G4LogicalVolume* fs_foot_outer_LV = new G4LogicalVolume(fs_foot_outer_S, fs_foot_outer_material, name);
    new G4PVPlacement(fs_foot_outer_rm, G4ThreeVector(fs_foot_outer_posX,fs_foot_outer_posY,fs_foot_outer_posZ), fs_foot_outer_LV, name, fs_leg_outer_LV, false, 0, fCheckOverlaps);
    */


    //===============  Visualization ===============//

    G4VisAttributes* yellowTVA = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
    G4VisAttributes* redTVA = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    G4VisAttributes* greyTVA = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.25));
    G4VisAttributes* blueTVA = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5));
    G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0, 0.5));

    // World

    WorldLV->SetVisAttributes(G4VisAttributes::Invisible);

    // Cryostat

    acrylic_cell_LV->SetVisAttributes(yellowTVA);
    pmt_array_LV->SetVisAttributes(helium_vis);
    can_sample_inner_LV->SetVisAttributes(helium_vis);
    can_4k_inner_LV->SetVisAttributes(G4VisAttributes::Invisible);
    can_77k_inner_LV->SetVisAttributes(G4VisAttributes::Invisible);
    can_vacuum_inner_LV->SetVisAttributes(G4VisAttributes::Invisible);
    cryo_vacuum_inner_LV->SetVisAttributes(G4VisAttributes::Invisible);
    cryo_77k_inner_LV->SetVisAttributes(G4VisAttributes::Invisible);
    cryo_4k_inner_LV->SetVisAttributes(helium_vis);

    can_sample_plate_LV->SetVisAttributes(container_vis);
    can_sample_ring_LV->SetVisAttributes(container_vis);
    can_sample_outer_LV->SetVisAttributes(container_vis);
    can_4k_outer_LV->SetVisAttributes(container_vis);
    can_77k_outer_LV->SetVisAttributes(container_vis);
    can_vacuum_outer_LV->SetVisAttributes(container_vis);
    arm_vacuum_LV->SetVisAttributes(container_vis);
    arm_77k_LV->SetVisAttributes(container_vis);
    arm_4k_LV->SetVisAttributes(container_vis);
    arm_sample_LV->SetVisAttributes(container_vis);
    cryo_vacuum_outer_LV->SetVisAttributes(container_vis);
    cryo_77k_outer_LV->SetVisAttributes(container_vis);
    cryo_nitrogen_LV->SetVisAttributes(nitrogen_vis);
    cryo_4k_outer_LV->SetVisAttributes(container_vis);

    // Far-side Detector

    fs_head_outer_LV->SetVisAttributes(container_vis);
    fs_head_inner_LV->SetVisAttributes(greenTVA);
    /*
    fs_neck_outer_LV->SetVisAttributes(container_vis);
    fs_body_outer_LV->SetVisAttributes(container_vis);
    fs_pants_outer_LV->SetVisAttributes(container_vis);
    fs_leg_outer_LV->SetVisAttributes(container_vis);
    fs_foot_outer_LV->SetVisAttributes(container_vis);
    */
    fConstructed = true;

    return WorldPV;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::PlacePMT(G4LogicalVolume* can_sample_inner_LV, G4double &pmt_posX,G4double &pmt_posY,G4double &pmt_posZ,G4RotationMatrix* pmt_rm)
{

  //
  // This function only places one PMT
  // Used to shorten the code
  // All six PMT geometries have the same properties, just different rotation/positions
  //

  G4String name;

  G4double pmt_sizeX = 25.7*mm;
  G4double pmt_sizeY = 25.7*mm;
  G4double pmt_sizeZ = 29.45*mm;

  G4double pmt_Window_sizeX = 24.*mm;
  G4double pmt_Window_sizeY = 24.*mm;
  G4double pmt_Window_sizeZ = 1.2*mm;

  G4double pmt_Body_sizeX = 25.7*mm;
  G4double pmt_Body_sizeY = 25.7*mm;
  G4double pmt_Body_sizeZ = 28.25*mm;

  G4double pmt_interior_sizeX = 22.7*mm;
  G4double pmt_interior_sizeY = 22.7*mm;
  G4double pmt_interior_sizeZ = 26.25*mm;

  G4Material* pmt_material = G4Material::GetMaterial("galactic");
  G4Material* pmt_Window_material = G4Material::GetMaterial("synthetic_silica");
  G4Material* pmt_Body_material = G4Material::GetMaterial("ss_t316");
  G4Material* pmt_interior_material = G4Material::GetMaterial("generic_pmt");

  if ( ! pmt_material || ! pmt_Window_material || ! pmt_Body_material ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("JanisDetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  name = "pmt";
  G4VSolid* pmt_S = new G4Box(name, pmt_sizeX/2., pmt_sizeY/2., pmt_sizeZ/2.);
  G4LogicalVolume* pmt_LV = new G4LogicalVolume(pmt_S, pmt_material, name);
  new G4PVPlacement(pmt_rm, G4ThreeVector(pmt_posX,pmt_posY,pmt_posZ), pmt_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

  name = "pmt_Window";
  G4VSolid* pmt_Window_S = new G4Box(name, pmt_Window_sizeX/2., pmt_Window_sizeY/2., pmt_Window_sizeZ/2.);
  G4LogicalVolume* pmt_Window_LV = new G4LogicalVolume(pmt_Window_S, pmt_Window_material, name);
  new G4PVPlacement(0, G4ThreeVector(0,0,pmt_sizeZ/2. - pmt_Window_sizeZ/2.), pmt_Window_LV, name, pmt_LV, false, 0, fCheckOverlaps);

  name = "pmt_Body";
  G4VSolid* pmt_Body_S = new G4Box(name, pmt_Body_sizeX/2., pmt_Body_sizeY/2., pmt_Body_sizeZ/2.);
  G4LogicalVolume* pmt_Body_LV = new G4LogicalVolume(pmt_Body_S, pmt_Body_material, name);
  new G4PVPlacement(0, G4ThreeVector(0,0,-pmt_Window_sizeZ/2.), pmt_Body_LV, name, pmt_LV, false, 0, fCheckOverlaps);

  name = "pmt_interior";
  G4VSolid* pmt_interior_S = new G4Box(name, pmt_interior_sizeX/2., pmt_interior_sizeY/2., pmt_interior_sizeZ/2.);
  G4LogicalVolume* pmt_interior_LV = new G4LogicalVolume(pmt_interior_S, pmt_interior_material, name);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), pmt_interior_LV, name, pmt_Body_LV, false, 0, fCheckOverlaps);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.25));
  G4VisAttributes* Synthetic_Silica_Vis= new G4VisAttributes(G4Colour(1.0,1.0,0.25,0.75));
  G4VisAttributes* redTVA = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
  simpleBoxVisAtt->SetVisibility(true);

  pmt_LV->SetVisAttributes (G4VisAttributes::Invisible);
  pmt_Window_LV->SetVisAttributes (Synthetic_Silica_Vis);
  pmt_Body_LV->SetVisAttributes (redTVA);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::setFarSideAngle(G4double fs_angle)
{
    if(!fConstructed){
        return;
    }
    else if(fConstructed){
        fs_head_inner_LV->RemoveDaughter(fs_head_outer_PV);
        delete fs_head_inner_PV;
        WorldLV->RemoveDaughter(fs_head_inner_PV);
        delete fs_head_outer_PV;

        G4Material* fs_head_outer_material = G4Material::GetMaterial("Al");
        G4Material* fs_head_inner_material = G4Material::GetMaterial("BC-501A");

        G4double fs_head_outer_rMin = 0.0*mm;
        G4double fs_head_outer_rMax = 66.675*mm;
        G4double fs_head_outer_Dz = 133.35*mm;
        G4double fs_head_outer_SPhi = 0.0*deg;
        G4double fs_head_outer_DPhi = 360.0*deg;

        G4double fs_head_inner_rMin = 0.0*mm;
        G4double fs_head_inner_rMax = 63.5*mm;
        G4double fs_head_inner_Dz = 12.7*cm;
        G4double fs_head_inner_SPhi = 0.0*deg;
        G4double fs_head_inner_DPhi = 360.0*deg;


        G4double fs_placement_angle = fs_angle*deg;

        G4double fs_placement_height = -10.0*cm;
        G4double fs_placement_distance = 140*cm;

        G4double fs_head_outer_posX = 0.0*mm;
        G4double fs_head_outer_posY = 0.0*mm;
        G4double fs_head_outer_posZ = 0.0*mm;

        G4double fs_head_inner_posX = 140.0 * cos(fs_placement_angle) * cm;
        G4double fs_head_inner_posY = 140.0 * sin(fs_placement_angle) * cm;
        G4double fs_head_inner_posZ = fs_placement_height;

        G4RotationMatrix fs_head_inner_rm = G4RotationMatrix();
        fs_head_inner_rm.rotateZ(fs_placement_angle);

        G4String name;

        name = 'fs_head_inner';
        G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
        G4LogicalVolume* fs_head_inner_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
        G4Transform3D fs_head_inner_transform(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ));
        new G4PVPlacement(fs_head_inner_transform, fs_head_inner_LV, name, WorldLV, false, 0, fCheckOverlaps);

        name = 'fs_head_outer';
        G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
        G4LogicalVolume* fs_head_outer_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
        new G4PVPlacement(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ), fs_head_outer_LV, name, fs_head_inner_LV, false, 0, fCheckOverlaps);

        G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
        container_vis->SetVisibility(true);
        G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
        fs_head_outer_LV->SetVisAttributes(container_vis);
        fs_head_inner_LV->SetVisAttributes(greenTVA);

        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}
