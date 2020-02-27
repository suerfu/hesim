
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
  fConstructed(false), // Whether the original volumes have been constructed
  fReady1(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady2(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady3(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady4(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady5(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady6(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady7(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fReady8(false), // fReadyX is the flag that shows whether a new far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady1(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady2(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady3(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady4(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady5(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady6(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady7(false), // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
  fNaIReady8(false)  // fNaIReadyX is the flag that shows whether a new Sodium Iodine far-side detector has been placed so that it is ready to be determined the distance
{
  fDetectorMessenger = new JanisDetectorConstructionMessenger(this);

  mylar_surf_density = 0.009 * g/cm2;
  mylar_thickness = 0.0254 * cm;
  mylar_density = mylar_surf_density /mylar_thickness;

  src_dia = 2.38 * cm;
  src_thickness = 0.076 * cm;
  src_active_dia = 0.3 * cm;

  kapton_surf_density = 0.0009 * g/cm2;
  kapton_thickness = src_thickness - mylar_thickness;
  kapton_density = kapton_surf_density /  kapton_thickness;

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

    G4Element* elH  = new G4Element(name = "Hydrogen"   , symbol = "H"  , z = 1.  , a =   1.008*g/mole);
    G4Element* elC  = new G4Element(name = "Carbon"     , symbol = "C"  , z = 6.  , a =  12.011*g/mole);
    G4Element* elN  = new G4Element(name = "Nitrogen"   , symbol = "N"  , z = 7.  , a =  14.007*g/mole);
    G4Element* elO  = new G4Element(name = "Oxygen"     , symbol = "O"  , z = 8.  , a =  15.999*g/mole);
    G4Element* elNa = new G4Element(name = "Sodium"     , symbol = "Na" , z = 11. , a =  22.990*g/mole);
    G4Element* elAl = new G4Element(name = "Aluminum"   , symbol = "Al" , z = 13. , a =  26.981*g/mole);
    G4Element* elSi = new G4Element(name = "Silicon"    , symbol = "Si" , z = 14. , a =  28.086*g/mole);
    G4Element* elP  = new G4Element(name = "Phosphorus" , symbol = "P"  , z = 15. , a =  30.974*g/mole);
    G4Element* elS  = new G4Element(name = "Sulfur"     , symbol = "S"  , z = 16. , a =  32.065*g/mole);
    G4Element* elCr = new G4Element(name = "Chromium"   , symbol = "Cr" , z = 24. , a =  51.996*g/mole);
    G4Element* elMn = new G4Element(name = "Manganese"  , symbol = "Mn" , z = 25. , a =  54.938*g/mole);
    G4Element* elFe = new G4Element(name = "Iron"       , symbol = "Fe" , z = 26. , a =  55.845*g/mole);
    G4Element* elNi = new G4Element(name = "Nickel"     , symbol = "Ni" , z = 28. , a =  58.693*g/mole);
    G4Element* elCu = new G4Element(name = "Copper"     , symbol = "Cu" , z = 29. , a =  63.546*g/mole);
    G4Element* elMo = new G4Element(name = "Molybdenum" , symbol = "Mo" , z = 42. , a =  95.940*g/mole);
    G4Element* elSn = new G4Element(name = "Tin"        , symbol = "Sn" , z = 50. , a = 118.710*g/mole);
    G4Element* elI  = new G4Element(name = "Iodine"     , symbol = "I"  , z = 53. , a = 126.904*g/mole);

    // Liquid helium material
    // Density from (assuming 3K LHe)
    // https://www.bnl.gov/magnets/staff/gupta/cryogenic-data-handbook/Section2.pdf

    new G4Material("liquid_helium",   z=2., a= 4.00*g/mole,  density= 0.141*g/cm3, kStateLiquid, temp=3.*kelvin);
    new G4Material("liquid_nitrogen", z=7., a= 14.00*g/mole, density= 0.807*g/cm3, kStateLiquid, temp=77.*kelvin);

    // Vacuum
    G4Material* Galactic  = new G4Material("galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

    // Stainless Steel Type 304
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

    // PMT base FR4
    G4Material* fr4 = new G4Material(name = "fr4" , density = 1.85*g/cm3 , nComponents = 6);
    fr4 -> AddElement(elC  , fractionMass =   5*perCent);
    fr4 -> AddElement(elO  , fractionMass =  28*perCent);
    fr4 -> AddElement(elAl , fractionMass =  45*perCent);
    fr4 -> AddElement(elSi , fractionMass =  12*perCent);
    fr4 -> AddElement(elCu , fractionMass =   5*perCent);
    fr4 -> AddElement(elSn , fractionMass =   5*perCent);

    // Naphthalene
    G4Material* BC501A = new G4Material(name = "BC-501A" , density = 0.874*g/cm3 , nComponents = 2);
    BC501A -> AddElement(elH , nAtoms = 482);
    BC501A -> AddElement(elC  , nAtoms = 398);

    G4Material* Synthetic_Silica = new G4Material(name = "synthetic_silica" , density = 2.65*g/cm3 , nComponents = 2);
    Synthetic_Silica -> AddElement(elSi , nAtoms = 1);
    Synthetic_Silica -> AddElement(elO  , nAtoms = 2);

    G4Material* NaI = new G4Material(name = "NaI" , density = 3.67*g/cm3 , nComponents = 2);
    NaI -> AddElement(elNa , nAtoms = 1);
    NaI -> AddElement(elI  , nAtoms = 1);

    G4Material* Generic_Acrylic = new G4Material(name = "generic_acrylic", 1.18*g/cm3, nComponents = 3);
    Generic_Acrylic -> AddElement(elH , nAtoms=8);
    Generic_Acrylic -> AddElement(elC , nAtoms=5);
    Generic_Acrylic -> AddElement(elO , nAtoms=2);

    // quartz (SiO2, crystalline)
    G4Material* Quartz = new G4Material("quartz", 2.64 *g/cm3, nComponents= 2);
    Quartz-> AddElement(elSi, nAtoms=1);
    Quartz-> AddElement(elO,  nAtoms=2);

    G4Material* Generic_PMT = new G4Material(name = "generic_pmt", 0.1*g/cm3, nComponents = 2);
    Generic_PMT -> AddMaterial(Galactic , fractionMass=80.0*perCent);
    Generic_PMT -> AddMaterial(ss_t316 , fractionMass=20.0*perCent);

    // By Suerfu on 2020-02-27
    // Materials used on Cs137 radioactive source
    G4Material* mylar_material = new G4Material( name = "mylar", mylar_density, nComponents = 3);
    mylar_material->AddElement(elC, nAtoms=10);
    mylar_material->AddElement(elH, nAtoms=8);
    mylar_material->AddElement(elO, nAtoms=4);

    G4Material* kapton_material = new G4Material( name = "kapton", kapton_density, nComponents = 4);
    kapton_material->AddElement(elC, nAtoms=22);
    kapton_material->AddElement(elH, nAtoms=10);
    kapton_material->AddElement(elN, nAtoms=2);
    kapton_material->AddElement(elO, nAtoms=5);

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
    G4Material* floor_material = mat_man -> FindOrBuildMaterial("G4_CONCRETE");

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

    // PMT base
    G4Material* pmt_base_material = G4Material::GetMaterial("fr4");

    // Far-side Detector

    G4Material* fs_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fs_head_inner_material = G4Material::GetMaterial("BC-501A");

    // PMT Related

    G4Material* quartz_cell_material = G4Material::GetMaterial("quartz");
    G4Material* pmt_array_material = G4Material::GetMaterial("liquid_helium");

    G4Material* source_material = G4Material::GetMaterial("kapton");
    G4Material* src_window_material = G4Material::GetMaterial("mylar");

    if ( ! default_material || ! quartz_cell_material || ! liquid_helium_material ) {
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

    // Floor

    G4double floor_rMin = 0.0*cm;
    G4double floor_rMax = 200.0*cm;
    G4double floor_Dz = 30.0*cm;
    G4double floor_SPhi = 0.0*deg;
    G4double floor_DPhi = 360.0*deg;

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

    // Quartz cell around liquid helium

    G4double quartz_cell_wall_thickness = 1.0*mm;

    G4double quartz_cell_exterior_sizeX = liquid_helium_sizeX+(2*quartz_cell_wall_thickness);
    G4double quartz_cell_exterior_sizeY = liquid_helium_sizeY+(2*quartz_cell_wall_thickness);
    G4double quartz_cell_exterior_sizeZ = liquid_helium_sizeZ+(2*quartz_cell_wall_thickness);

    // PMT base

    G4double pmt_base_sizeX = 46.0*mm;
    G4double pmt_base_sizeY = 52.5*mm;
    G4double pmt_base_sizeZ = 1.6*mm;

    // PMT

    G4double pmt_sizeX = 25.7*mm;
    G4double pmt_sizeY = 25.7*mm;
    G4double pmt_sizeZ = 29.45*mm;

    // PMT Array (Mother volume for the six PMTs)

    G4double pmt_array_size_xyz = 1*mm + (pmt_sizeZ*2 + quartz_cell_exterior_sizeZ);

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


    //===============  Positions ===============//

    // Floor

    G4double floor_posX = 0.0*mm;
    G4double floor_posY = 0.0*mm;
    G4double floor_posZ = - (can_vacuum_outer_Dz + 68.9*cm + 0.5*floor_Dz); // 68.9cm is the measured distance between the bottom and the floor

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

    // PMT base Positions

    G4double pmt_base1_posX = (pmt_array_size_xyz - 1)/2.0*mm + pmt_base_sizeZ + 8*mm;
    G4double pmt_base1_posY = 0;
    G4double pmt_base1_posZ = (-25.33+2.75)*mm;

    G4double pmt_base2_posX = -((pmt_array_size_xyz - 1)/2.0*mm + pmt_base_sizeZ + 8*mm);
    G4double pmt_base2_posY = 0;
    G4double pmt_base2_posZ = (-25.33+2.75)*mm;

    G4double pmt_base3_posX = 0;
    G4double pmt_base3_posY = -((pmt_array_size_xyz - 1)/2.0*mm + pmt_base_sizeZ + 8*mm);
    G4double pmt_base3_posZ = (-25.33+2.75)*mm;

    G4double pmt_base4_posX = 0;
    G4double pmt_base4_posY = (pmt_array_size_xyz - 1)/2.0*mm + pmt_base_sizeZ + 8*mm;
    G4double pmt_base4_posZ = (-25.33+2.75)*mm;

    // PMT Array Position

    G4double pmt_array_posX = 0*mm;
    G4double pmt_array_posY = 0*mm;
    G4double pmt_array_posZ = -25.33*mm;

    // PMT Positions

    G4double pmt1_posX = quartz_cell_exterior_sizeX/2.0 + pmt_sizeZ/2.0*mm;
    G4double pmt1_posY = 0*mm;
    G4double pmt1_posZ = 0*mm;

    G4double pmt2_posX = -(quartz_cell_exterior_sizeX/2.0 + pmt_sizeZ/2.0)*mm;
    G4double pmt2_posY = 0*mm;
    G4double pmt2_posZ = 0*mm;

    G4double pmt3_posX = 0*mm;
    G4double pmt3_posY = quartz_cell_exterior_sizeY/2.0 + pmt_sizeZ/2.0*mm;
    G4double pmt3_posZ = 0*mm;

    G4double pmt4_posX = 0*mm;
    G4double pmt4_posY = -(quartz_cell_exterior_sizeY/2.0 + pmt_sizeZ/2.0)*mm;
    G4double pmt4_posZ = 0*mm;

    G4double pmt5_posX = 0*mm;
    G4double pmt5_posY = 0*mm;
    G4double pmt5_posZ = quartz_cell_exterior_sizeZ/2.0 + pmt_sizeZ/2.0*mm;

    G4double pmt6_posX = 0*mm;
    G4double pmt6_posY = 0*mm;
    G4double pmt6_posZ = -(quartz_cell_exterior_sizeZ/2.0 + pmt_sizeZ/2.0)*mm;


    // Far-side Detector

    // Please modify here for default Far-side detector placement
    fs_placement_angle = 0.0*deg;
    G4double fs_placement_height = -10.0*cm;
    fs_placement_distance = -40*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle);
    G4double fs_head_outer_posZ = fs_placement_height;


    //===============  Rotations ===============//

    // Cryostat

    G4RotationMatrix* floor_rm = new G4RotationMatrix;

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

    // PMT base
    G4RotationMatrix pmt_base1_rm = G4RotationMatrix();
    G4RotationMatrix pmt_base2_rm = G4RotationMatrix();
    G4RotationMatrix pmt_base3_rm = G4RotationMatrix();
    G4RotationMatrix pmt_base4_rm = G4RotationMatrix();

    pmt_base1_rm.rotateY(90.0*deg);
    pmt_base1_rm.rotateX(90.0*deg);
    pmt_base2_rm.rotateY(90.0*deg);
    pmt_base2_rm.rotateX(90.0*deg);
    pmt_base3_rm.rotateX(90.0*deg);
    pmt_base4_rm.rotateX(90.0*deg);

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
    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();

    cryo_4k_inner_rm->rotateZ(45*deg);

    // Rotate the various PMTs so that they all point inward
    pmt1_rm->rotateY(90.0*deg);
    pmt2_rm->rotateY(-90.0*deg);
    pmt3_rm->rotateX(-90.0*deg);
    pmt4_rm->rotateX(90.0*deg);
    pmt5_rm->rotateX(180.0*deg);
    pmt6_rm->rotateX(0*deg);

    // Rotate the Far-side Detector
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle);

    //===============  Build Geometry ===============//

    G4String name;

    // World

    name = "world_box";
    G4Box* world_box = new G4Box(name, world_x/2.0, world_y/2.0, world_z/2.0);
    WorldLV = new G4LogicalVolume(world_box, world_material, "WorldLV");

    WorldPV = new G4PVPlacement(0, G4ThreeVector(0,0,0), WorldLV, name, 0, false, 0, checkOverlaps);

    // Floor

    name = "floor";
    G4Tubs* floor_S = new G4Tubs(name, floor_rMin, floor_rMax, floor_Dz/2.0, floor_SPhi, floor_DPhi);
    G4LogicalVolume* floor_LV = new G4LogicalVolume(floor_S, floor_material, name);
    new G4PVPlacement(floor_rm, G4ThreeVector(floor_posX, floor_posY, floor_posZ), floor_LV, name, WorldLV, false, 0, fCheckOverlaps);

    // Cryostat

    name = "can_vacuum_outer";
    G4Tubs* can_vacuum_outer_S = new G4Tubs(name, can_vacuum_outer_rMin, can_vacuum_outer_rMax, can_vacuum_outer_Dz/2.0, can_vacuum_outer_SPhi, can_vacuum_outer_DPhi);
    G4LogicalVolume* can_vacuum_outer_LV = new G4LogicalVolume(can_vacuum_outer_S, can_vacuum_outer_material, name);
    new G4PVPlacement(can_vacuum_outer_rm, G4ThreeVector(can_vacuum_outer_posX,can_vacuum_outer_posY,can_vacuum_outer_posZ), can_vacuum_outer_LV, name, WorldLV, false, 0, fCheckOverlaps);
    G4cerr << "vacuum can outer at " << G4ThreeVector(can_vacuum_outer_posX,can_vacuum_outer_posY,can_vacuum_outer_posZ)/cm << G4endl;

    name = "can_vacuum_inner";
    G4Tubs* can_vacuum_inner_S = new G4Tubs(name, can_vacuum_inner_rMin, can_vacuum_inner_rMax, can_vacuum_inner_Dz/2.0, can_vacuum_inner_SPhi, can_vacuum_inner_DPhi);
    G4LogicalVolume* can_vacuum_inner_LV = new G4LogicalVolume(can_vacuum_inner_S, can_vacuum_inner_material, name);
    new G4PVPlacement(can_vacuum_inner_rm, G4ThreeVector(can_vacuum_inner_posX,can_vacuum_inner_posY,can_vacuum_inner_posZ), can_vacuum_inner_LV, name, can_vacuum_outer_LV, false, 0, fCheckOverlaps);
    G4cerr << "vacuum can inner at " << G4ThreeVector(can_vacuum_inner_posX,can_vacuum_inner_posY,can_vacuum_inner_posZ)/cm << G4endl;

    name = "can_77k_outer";
    G4Tubs* can_77k_outer_S = new G4Tubs(name, can_77k_outer_rMin, can_77k_outer_rMax, can_77k_outer_Dz/2.0, can_77k_outer_SPhi, can_77k_outer_DPhi);
    G4LogicalVolume* can_77k_outer_LV = new G4LogicalVolume(can_77k_outer_S, can_77k_outer_material, name);
    new G4PVPlacement(can_77k_outer_rm, G4ThreeVector(can_77k_outer_posX,can_77k_outer_posY,can_77k_outer_posZ), can_77k_outer_LV, name, can_vacuum_inner_LV, false, 0, fCheckOverlaps);
    G4cerr << "77K outer at " << G4ThreeVector(can_77k_outer_posX,can_77k_outer_posY,can_77k_outer_posZ)/cm << G4endl;

    name = "can_77k_inner";
    G4Tubs* can_77k_inner_S = new G4Tubs(name, can_77k_inner_rMin, can_77k_inner_rMax, can_77k_inner_Dz/2.0, can_77k_inner_SPhi, can_77k_inner_DPhi);
    G4LogicalVolume* can_77k_inner_LV = new G4LogicalVolume(can_77k_inner_S, can_77k_inner_material, name);
    new G4PVPlacement(can_77k_inner_rm, G4ThreeVector(can_77k_inner_posX,can_77k_inner_posY,can_77k_inner_posZ), can_77k_inner_LV, name, can_77k_outer_LV, false, 0, fCheckOverlaps);
    G4cerr << "77K can inner at " << G4ThreeVector(can_77k_inner_posX,can_77k_inner_posY,can_77k_inner_posZ)/cm << G4endl;

    name = "can_4k_outer";
    G4Tubs* can_4k_outer_S = new G4Tubs(name, can_4k_outer_rMin, can_4k_outer_rMax, can_4k_outer_Dz/2.0, can_4k_outer_SPhi, can_4k_outer_DPhi);
    G4LogicalVolume* can_4k_outer_LV = new G4LogicalVolume(can_4k_outer_S, can_4k_outer_material, name);
    new G4PVPlacement(can_4k_outer_rm, G4ThreeVector(can_4k_outer_posX,can_4k_outer_posY,can_4k_outer_posZ), can_4k_outer_LV, name, can_77k_inner_LV, false, 0, fCheckOverlaps);
    G4cerr << "can 4K outer at " << G4ThreeVector(can_4k_outer_posX,can_4k_outer_posY,can_4k_outer_posZ)/cm << G4endl;

    name = "can_4k_inner";
    G4Tubs* can_4k_inner_S = new G4Tubs(name, can_4k_inner_rMin, can_4k_inner_rMax, can_4k_inner_Dz/2.0, can_4k_inner_SPhi, can_4k_inner_DPhi);
    G4LogicalVolume* can_4k_inner_LV = new G4LogicalVolume(can_4k_inner_S, can_4k_inner_material, name);
    new G4PVPlacement(can_4k_inner_rm, G4ThreeVector(can_4k_inner_posX,can_4k_inner_posY,can_4k_inner_posZ), can_4k_inner_LV, name, can_4k_outer_LV, false, 0, fCheckOverlaps);
    G4cerr << "4K can inner at " << G4ThreeVector(can_4k_inner_posX,can_4k_inner_posY,can_4k_inner_posZ)/cm << G4endl;

    name = "can_sample_outer";
    G4Tubs* can_sample_outer_S = new G4Tubs(name, can_sample_outer_rMin, can_sample_outer_rMax, can_sample_outer_Dz/2.0, can_sample_outer_SPhi, can_sample_outer_DPhi);
    G4LogicalVolume* can_sample_outer_LV = new G4LogicalVolume(can_sample_outer_S, can_sample_outer_material, name);
    new G4PVPlacement(can_sample_outer_rm, G4ThreeVector(can_sample_outer_posX,can_sample_outer_posY,can_sample_outer_posZ), can_sample_outer_LV, name, can_4k_inner_LV, false, 0, fCheckOverlaps);
    G4cerr << "can sample outer at " << G4ThreeVector(can_sample_outer_posX,can_sample_outer_posY,can_sample_outer_posZ)/cm << G4endl;
    name = "can_sample_inner";
    G4Tubs* can_sample_inner_S = new G4Tubs(name, can_sample_inner_rMin, can_sample_inner_rMax, can_sample_inner_Dz/2.0, can_sample_inner_SPhi, can_sample_inner_DPhi);
    G4LogicalVolume* can_sample_inner_LV = new G4LogicalVolume(can_sample_inner_S, can_sample_inner_material, name);
    new G4PVPlacement(can_sample_inner_rm, G4ThreeVector(can_sample_inner_posX,can_sample_inner_posY,can_sample_inner_posZ), can_sample_inner_LV, name, can_sample_outer_LV, false, 0, fCheckOverlaps);
    G4cerr << "can sample inner at " << G4ThreeVector(can_sample_inner_posX,can_sample_inner_posY,can_sample_inner_posZ)/cm << G4endl;
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

    // PMT base

    name = "pmt_base1";
    G4VSolid* pmt_base_S = new G4Box(name, pmt_base_sizeX/2., pmt_base_sizeY/2., pmt_base_sizeZ/2.);
    G4LogicalVolume* pmt_base1_LV = new G4LogicalVolume(pmt_base_S, pmt_base_material, name);
    G4Transform3D pmt_base1_transform(pmt_base1_rm, G4ThreeVector(pmt_base1_posX,pmt_base1_posY,pmt_base1_posZ));
    new G4PVPlacement(pmt_base1_transform, pmt_base1_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

    name = "pmt_base2";
    G4LogicalVolume* pmt_base2_LV = new G4LogicalVolume(pmt_base_S, pmt_base_material, name);
    G4Transform3D pmt_base2_transform(pmt_base2_rm, G4ThreeVector(pmt_base2_posX,pmt_base2_posY,pmt_base2_posZ));
    new G4PVPlacement(pmt_base2_transform, pmt_base2_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

    name = "pmt_base3";
    G4LogicalVolume* pmt_base3_LV = new G4LogicalVolume(pmt_base_S, pmt_base_material, name);
    G4Transform3D pmt_base3_transform(pmt_base3_rm, G4ThreeVector(pmt_base3_posX,pmt_base3_posY,pmt_base3_posZ));
    new G4PVPlacement(pmt_base3_transform, pmt_base3_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

    name = "pmt_base4";
    G4LogicalVolume* pmt_base4_LV = new G4LogicalVolume(pmt_base_S, pmt_base_material, name);
    G4Transform3D pmt_base4_transform(pmt_base4_rm, G4ThreeVector(pmt_base4_posX,pmt_base4_posY,pmt_base4_posZ));
    new G4PVPlacement(pmt_base4_transform, pmt_base4_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);

    // PMT Array (Uses function PlacePMT to place individual PMTs)

    name = "pmt_array";
    G4VSolid* pmt_array_S = new G4Box(name, pmt_array_size_xyz/2., pmt_array_size_xyz/2., pmt_array_size_xyz/2.);
    G4LogicalVolume* pmt_array_LV = new G4LogicalVolume(pmt_array_S, pmt_array_material, name);
    new G4PVPlacement(pmt_array_rm, G4ThreeVector(pmt_array_posX,pmt_array_posY,pmt_array_posZ), pmt_array_LV, name, can_sample_inner_LV, false, 0, fCheckOverlaps);
    G4cerr << "pmt array at " << G4ThreeVector(pmt_array_posX,pmt_array_posY,pmt_array_posZ)/cm << G4endl;
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt1_posX,pmt1_posY,pmt1_posZ,pmt1_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt2_posX,pmt2_posY,pmt2_posZ,pmt2_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt3_posX,pmt3_posY,pmt3_posZ,pmt3_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt4_posX,pmt4_posY,pmt4_posZ,pmt4_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt5_posX,pmt5_posY,pmt5_posZ,pmt5_rm);
    JanisDetectorConstruction::PlacePMT(pmt_array_LV,pmt6_posX,pmt6_posY,pmt6_posZ,pmt6_rm);

    // Quartz cell

    name = "quartz_cell";
    G4VSolid* quartz_cell_exterior_S = new G4Box(name, quartz_cell_exterior_sizeX/2., quartz_cell_exterior_sizeY/2., quartz_cell_exterior_sizeZ/2.);
    G4LogicalVolume* quartz_cell_LV = new G4LogicalVolume(quartz_cell_exterior_S, quartz_cell_material, name);
    new G4PVPlacement(NO_ROT,G4ThreeVector(), quartz_cell_LV, name, pmt_array_LV, false, 0, fCheckOverlaps);

    // Liquid Helium

    name = "liquid helium";
    G4VSolid* liquid_helium_S = new G4Box(name, liquid_helium_sizeX/2., liquid_helium_sizeY/2., liquid_helium_sizeZ/2.);
    G4LogicalVolume* liquid_helium_LV = new G4LogicalVolume(liquid_helium_S, liquid_helium_material, name);
    liquid_helium_PV = new G4PVPlacement(NO_ROT, G4ThreeVector(), liquid_helium_LV, name, quartz_cell_LV, false, 0, fCheckOverlaps);

    //===============  Visualization ===============//

    G4VisAttributes* yellowTVA = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
    G4VisAttributes* redTVA = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    G4VisAttributes* greyTVA = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.25));
    G4VisAttributes* blueTVA = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5));
    G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0, 0.5));

    // World

    WorldLV->SetVisAttributes(G4VisAttributes::Invisible);

    // Floor

    floor_LV->SetVisAttributes(container_vis);

    // Cryostat

    quartz_cell_LV->SetVisAttributes(yellowTVA);
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

    // PMT bases
    pmt_base1_LV->SetVisAttributes(greenTVA);
    pmt_base2_LV->SetVisAttributes(greenTVA);
    pmt_base3_LV->SetVisAttributes(greenTVA);
    pmt_base4_LV->SetVisAttributes(greenTVA);

    // Far-side Detector

    //fs_head_outer_LV->SetVisAttributes(container_vis);
    //fs_head_inner_LV->SetVisAttributes(greenTVA);
    /*
    fs_neck_outer_LV->SetVisAttributes(container_vis);
    fs_body_outer_LV->SetVisAttributes(container_vis);
    fs_pants_outer_LV->SetVisAttributes(container_vis);
    fs_leg_outer_LV->SetVisAttributes(container_vis);
    fs_foot_outer_LV->SetVisAttributes(container_vis);
    */

    // Radioactive source
    G4ThreeVector source_pos = G4ThreeVector(-176.07*mm,176.07*mm,-95.322*mm);
    G4RotationMatrix* source_rot = new G4RotationMatrix();
    source_rot->rotateX( (360-90)*degree);
    source_rot->rotateY( (360-45)*degree);

    G4Tubs* source_solid = new G4Tubs("source solid", 0, src_dia/2, src_thickness/2, 0, 360*degree);
    G4LogicalVolume* source_log = new G4LogicalVolume( source_solid, source_material, "source logic" );
        // WARNING: note that this source position is hard-coded
        // since at the time of geometry construction, we do not know if it is gamma or neutron, and where gamma is originating from.
    new G4PVPlacement( source_rot, source_pos , source_log, "source", WorldLV, false, 0, 0);

    G4Tubs* src_window_solid = new G4Tubs("source window solid", 0, src_dia/2, mylar_thickness/2, 0, 360*degree);
    G4LogicalVolume* src_window_log = new G4LogicalVolume( src_window_solid, src_window_material, "source window logic" );
    new G4PVPlacement( 0, G4ThreeVector(0, 0, src_thickness/2-mylar_thickness/2), src_window_log, "source window", source_log, false, 0, 0);

    G4cerr << "Source position is at " << source_pos + (src_thickness/2-mylar_thickness)*G4ThreeVector(1,-1,0)/G4ThreeVector(1,-1,0).mag() << G4endl;

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

void JanisDetectorConstruction::setPMTAngle(G4double pmt_angle)
{
  angle_corr = pmt_angle * deg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::setFarSideDistance(G4double fs_distance)
{
  G4String name_in;
  G4String name_out;
  G4String name_scin;

  if(fReady1){
    fs_head_inner_1_LV->ClearDaughters();
    delete fs_head_outer_1_PV;
    WorldLV->RemoveDaughter(fs_head_outer_1_PV);
    delete fs_head_inner_1_PV;
    name_in = "fs1_head_inner_1";
    name_out = "fs1_head_outer_1";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_1_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_1_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_1_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_1_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_1_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_1_LV, name_in, fs_head_outer_1_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_1_LV->SetVisAttributes(container_vis);
    fs_head_inner_1_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady1 = false;
  }

  if(fReady2){
    fs_head_inner_2_LV->ClearDaughters();
    delete fs_head_outer_2_PV;
    WorldLV->RemoveDaughter(fs_head_outer_2_PV);
    delete fs_head_inner_2_PV;
    name_in = "fs2_head_inner_2";
    name_out = "fs2_head_outer_2";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_2_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_2_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_2_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_2_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_2_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_2_LV, name_in, fs_head_outer_2_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_2_LV->SetVisAttributes(container_vis);
    fs_head_inner_2_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady2 = false;
  }

  if(fReady3){
    fs_head_inner_3_LV->ClearDaughters();
    delete fs_head_outer_3_PV;
    WorldLV->RemoveDaughter(fs_head_outer_3_PV);
    delete fs_head_inner_3_PV;
    name_in = "fs3_head_inner_3";
    name_out = "fs3_head_outer_3";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_3_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_3_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_3_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_3_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_3_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_3_LV, name_in, fs_head_outer_3_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_3_LV->SetVisAttributes(container_vis);
    fs_head_inner_3_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady3 = false;
  }

  if(fReady4){
    fs_head_inner_4_LV->ClearDaughters();
    delete fs_head_outer_4_PV;
    WorldLV->RemoveDaughter(fs_head_outer_4_PV);
    delete fs_head_inner_4_PV;
    name_in = "fs4_head_inner_4";
    name_out = "fs4_head_outer_4";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_4_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_4_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_4_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_4_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_4_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_4_LV, name_in, fs_head_outer_4_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_4_LV->SetVisAttributes(container_vis);
    fs_head_inner_4_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady4 = false;
  }

  if(fReady5){
    fs_head_inner_5_LV->ClearDaughters();
    delete fs_head_outer_5_PV;
    WorldLV->RemoveDaughter(fs_head_outer_5_PV);
    delete fs_head_inner_5_PV;
    name_in = "fs5_head_inner_5";
    name_out = "fs5_head_outer_5";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_5_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_5_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_5_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_5_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_5_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_5_LV, name_in, fs_head_outer_5_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_5_LV->SetVisAttributes(container_vis);
    fs_head_inner_5_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady5 = false;
  }

  if(fReady6){
    fs_head_inner_6_LV->ClearDaughters();
    delete fs_head_outer_6_PV;
    WorldLV->RemoveDaughter(fs_head_outer_6_PV);
    delete fs_head_inner_6_PV;
    name_in = "fs6_head_inner_6";
    name_out = "fs6_head_outer_6";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_6_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_6_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_6_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_6_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_6_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_6_LV, name_in, fs_head_outer_6_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_6_LV->SetVisAttributes(container_vis);
    fs_head_inner_6_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady6 = false;
  }

  if(fReady7){
    fs_head_inner_7_LV->ClearDaughters();
    delete fs_head_outer_7_PV;
    WorldLV->RemoveDaughter(fs_head_outer_7_PV);
    delete fs_head_inner_7_PV;
    name_in = "fs7_head_inner_7";
    name_out = "fs7_head_outer_7";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_7_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_7_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_7_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_7_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_7_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_7_LV, name_in, fs_head_outer_7_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_7_LV->SetVisAttributes(container_vis);
    fs_head_inner_7_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady7 = false;
  }

  if(fReady8){
    fs_head_inner_8_LV->ClearDaughters();
    delete fs_head_outer_8_PV;
    WorldLV->RemoveDaughter(fs_head_outer_8_PV);
    delete fs_head_inner_8_PV;
    name_in = "fs8_head_inner_8";
    name_out = "fs8_head_outer_8";

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

    fs_placement_distance = fs_distance*cm;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fs_head_outer_S = new G4Tubs(name_out, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_8_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name_out);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_8_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_8_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fs_head_inner_S = new G4Tubs(name_in, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_8_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name_in);
    fs_head_inner_8_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_8_LV, name_in, fs_head_outer_8_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_8_LV->SetVisAttributes(container_vis);
    fs_head_inner_8_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady8 = false;
  }

  if(fNaIReady1){
    fsNaI_head_inner_1_LV->ClearDaughters();
    delete fsNaI_head_outer_1_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_1_PV);
    delete fsNaI_head_inner_1_PV;
    name_scin = "NaI1";
    name_in = "fsNaI1_head_inner_1";
    name_out = "fsNaI1_head_outer_1";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm; // Uncertain

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_1_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_1_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_1_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_1_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_1_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_1_LV, name_in, fsNaI_head_outer_1_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_1_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_1_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_1_LV, name_scin, fsNaI_head_inner_1_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_1_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_1_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_1_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady1 = false;
  }

  if(fNaIReady2){
    fsNaI_head_inner_2_LV->ClearDaughters();
    delete fsNaI_head_outer_2_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_2_PV);
    delete fsNaI_head_inner_2_PV;
    name_scin = "NaI2";
    name_in = "fsNaI2_head_inner_2";
    name_out = "fsNaI2_head_outer_2";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_2_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_2_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_2_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_2_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_2_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_2_LV, name_in, fsNaI_head_outer_2_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_2_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_2_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_2_LV, name_scin, fsNaI_head_inner_2_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_2_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_2_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_2_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady2 = false;
  }

  if(fNaIReady3){
    fsNaI_head_inner_3_LV->ClearDaughters();
    delete fsNaI_head_outer_3_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_3_PV);
    delete fsNaI_head_inner_3_PV;
    name_scin = "NaI3";
    name_in = "fsNaI3_head_inner_3";
    name_out = "fsNaI3_head_outer_3";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_3_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_3_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_3_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_3_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_3_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_3_LV, name_in, fsNaI_head_outer_3_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_3_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_3_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_3_LV, name_scin, fsNaI_head_inner_3_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_3_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_3_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_3_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady3 = false;
  }

  if(fNaIReady4){
    fsNaI_head_inner_4_LV->ClearDaughters();
    delete fsNaI_head_outer_4_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_4_PV);
    delete fsNaI_head_inner_4_PV;
    name_scin = "NaI4";
    name_in = "fsNaI4_head_inner_4";
    name_out = "fsNaI4_head_outer_4";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_4_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_4_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_4_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_4_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_4_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_4_LV, name_in, fsNaI_head_outer_4_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_4_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_4_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_4_LV, name_scin, fsNaI_head_inner_4_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_4_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_4_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_4_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady4 = false;
  }

  if(fNaIReady5){
    fsNaI_head_inner_5_LV->ClearDaughters();
    delete fsNaI_head_outer_5_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_5_PV);
    delete fsNaI_head_inner_5_PV;
    name_scin = "NaI5";
    name_in = "fsNaI5_head_inner_5";
    name_out = "fsNaI5_head_outer_5";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_5_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_5_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_5_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_5_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_5_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_5_LV, name_in, fsNaI_head_outer_5_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_5_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_5_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_5_LV, name_scin, fsNaI_head_inner_5_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_5_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_5_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_5_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady5 = false;
  }

  if(fNaIReady6){
    fsNaI_head_inner_6_LV->ClearDaughters();
    delete fsNaI_head_outer_6_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_6_PV);
    delete fsNaI_head_inner_6_PV;
    name_scin = "NaI6";
    name_in = "fsNaI6_head_inner_6";
    name_out = "fsNaI6_head_outer_6";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_6_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_6_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_6_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_6_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_6_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_6_LV, name_in, fsNaI_head_outer_6_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_6_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_6_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_6_LV, name_scin, fsNaI_head_inner_6_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_6_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_6_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_6_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady6 = false;
  }

  if(fNaIReady7){
    fsNaI_head_inner_7_LV->ClearDaughters();
    delete fsNaI_head_outer_7_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_7_PV);
    delete fsNaI_head_inner_7_PV;
    name_scin = "NaI7";
    name_in = "fsNaI7_head_inner_7";
    name_out = "fsNaI7_head_outer_7";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_7_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_7_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_7_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_7_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_7_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_7_LV, name_in, fsNaI_head_outer_7_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_7_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_7_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_7_LV, name_scin, fsNaI_head_inner_7_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_7_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_7_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_7_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady7 = false;
  }

  if(fNaIReady8){
    fsNaI_head_inner_8_LV->ClearDaughters();
    delete fsNaI_head_outer_8_PV;
    WorldLV->RemoveDaughter(fsNaI_head_outer_8_PV);
    delete fsNaI_head_inner_8_PV;
    name_scin = "NaI8";
    name_in = "fsNaI8_head_inner_8";
    name_out = "fsNaI8_head_outer_8";

    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("galactic");
    G4Material* fsNaI_scintillator_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 31.75*mm;
    G4double fsNaI_head_outer_Dz = 57.15*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 31.24*mm;
    G4double fsNaI_head_inner_Dz = 56.13*mm; // it doesn't actually matter since its just air
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    G4double fsNaI_scintillator_rMin = 0.0*mm;
    G4double fsNaI_scintillator_rMax = 25.4*mm;
    G4double fsNaI_scintillator_Dz = 50.8*mm;
    G4double fsNaI_scintillator_SPhi = 0.0*deg;
    G4double fsNaI_scintillator_DPhi = 360.0*deg;

    fs_placement_distance = fs_distance*cm;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_scintillator_posX = 0.0*mm;
    G4double fsNaI_scintillator_posY = 0.0*mm;
    G4double fsNaI_scintillator_posZ = -2.615 *mm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = 0.0*mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix *fsNaI_scintillator_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name_out, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_8_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name_out);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_8_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_8_LV, name_out, WorldLV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name_in, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_8_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name_in);
    fsNaI_head_inner_8_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_8_LV, name_in, fsNaI_head_outer_8_LV, false, 0, fCheckOverlaps);

    G4Tubs* fsNaI_scintillator_S = new G4Tubs(name_in, fsNaI_scintillator_rMin, fsNaI_scintillator_rMax, fsNaI_scintillator_Dz/2.0, fsNaI_scintillator_SPhi, fsNaI_scintillator_DPhi);
    fsNaI_scintillator_8_LV = new G4LogicalVolume(fsNaI_scintillator_S, fsNaI_scintillator_material, name_scin);
    fsNaI_scintillator_8_PV = new G4PVPlacement(fsNaI_scintillator_rm, G4ThreeVector(fsNaI_scintillator_posX,fsNaI_scintillator_posY,fsNaI_scintillator_posZ), fsNaI_scintillator_8_LV, name_scin, fsNaI_head_inner_8_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_8_LV->SetVisAttributes(container_vis);
    fsNaI_scintillator_8_LV->SetVisAttributes(greenTVA);
    fsNaI_head_inner_8_LV->SetVisAttributes(G4VisAttributes::Invisible);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady8 = false;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add1stFarSideAngle(G4double new_fs_angle_1)
{
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

    fs_placement_angle = new_fs_angle_1*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs1_head_outer_1";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_1_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_1_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_1_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS1";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_1_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_1_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_1_LV, name, fs_head_outer_1_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_1_LV->SetVisAttributes(container_vis);
    fs_head_inner_1_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady1 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add2ndFarSideAngle(G4double new_fs_angle_2)
{
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

    fs_placement_angle = new_fs_angle_2*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle);

    G4String name;

    name = "fs2_head_outer_2";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_2_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_2_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_2_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS2";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_2_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_2_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_2_LV, name, fs_head_outer_2_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_2_LV->SetVisAttributes(container_vis);
    fs_head_inner_2_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady2 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add3rdFarSideAngle(G4double new_fs_angle_3)
{
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

    fs_placement_angle = new_fs_angle_3*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs3_head_outer_3";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_3_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_3_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_3_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS3";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_3_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_3_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_3_LV, name, fs_head_outer_3_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_3_LV->SetVisAttributes(container_vis);
    fs_head_inner_3_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady3 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add4thFarSideAngle(G4double new_fs_angle_4)
{
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

    fs_placement_angle = new_fs_angle_4*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs4_head_outer_4";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_4_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_4_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_4_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS4";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_4_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_4_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_4_LV, name, fs_head_outer_4_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_4_LV->SetVisAttributes(container_vis);
    fs_head_inner_4_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady4 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add5thFarSideAngle(G4double new_fs_angle_5)
{
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

    fs_placement_angle = new_fs_angle_5*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs5_head_outer_5";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_5_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_5_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_5_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS5";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_5_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_5_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_5_LV, name, fs_head_outer_5_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_5_LV->SetVisAttributes(container_vis);
    fs_head_inner_5_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady5 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add6thFarSideAngle(G4double new_fs_angle_6)
{
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

    fs_placement_angle = new_fs_angle_6*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs6_head_outer_6";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_6_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_6_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_6_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS6";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_6_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_6_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_6_LV, name, fs_head_outer_6_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_6_LV->SetVisAttributes(container_vis);
    fs_head_inner_6_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady6 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add7thFarSideAngle(G4double new_fs_angle_7)
{
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

    fs_placement_angle = new_fs_angle_7*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs7_head_outer_7";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_7_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_7_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_7_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS7";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_7_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_7_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_7_LV, name, fs_head_outer_7_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_7_LV->SetVisAttributes(container_vis);
    fs_head_inner_7_LV->SetVisAttributes(greenTVA);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady7 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add8thFarSideAngle(G4double new_fs_angle_8)
{
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

    fs_placement_angle = new_fs_angle_8*deg;

    G4double fs_placement_height = -10.0*cm;

    G4double fs_head_inner_posX = 0.0*mm;
    G4double fs_head_inner_posY = 0.0*mm;
    G4double fs_head_inner_posZ = 0.0*mm;

    G4double fs_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fs_head_outer_posZ = fs_placement_height;

    G4RotationMatrix *fs_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fs_head_outer_rm = G4RotationMatrix();
    fs_head_outer_rm.rotateY(270.0*deg);
    fs_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fs8_head_outer_8";
    G4Tubs* fs_head_outer_S = new G4Tubs(name, fs_head_outer_rMin, fs_head_outer_rMax, fs_head_outer_Dz/2.0, fs_head_outer_SPhi, fs_head_outer_DPhi);
    fs_head_outer_8_LV = new G4LogicalVolume(fs_head_outer_S, fs_head_outer_material, name);
    G4Transform3D fs_head_outer_transform(fs_head_outer_rm, G4ThreeVector(fs_head_outer_posX,fs_head_outer_posY,fs_head_outer_posZ));
    fs_head_outer_8_PV = new G4PVPlacement(fs_head_outer_transform, fs_head_outer_8_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "LS8";
    G4Tubs* fs_head_inner_S = new G4Tubs(name, fs_head_inner_rMin, fs_head_inner_rMax, fs_head_inner_Dz/2.0, fs_head_inner_SPhi, fs_head_inner_DPhi);
    fs_head_inner_8_LV = new G4LogicalVolume(fs_head_inner_S, fs_head_inner_material, name);
    fs_head_inner_8_PV = new G4PVPlacement(fs_head_inner_rm, G4ThreeVector(fs_head_inner_posX,fs_head_inner_posY,fs_head_inner_posZ), fs_head_inner_8_LV, name, fs_head_outer_8_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fs_head_outer_8_LV->SetVisAttributes(container_vis);
    fs_head_inner_8_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fReady8 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add1stNaIFarSideAngle(G4double new_fsNaI_angle_1)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_1*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI1_head_outer_1";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_1_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_1_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_1_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI1_head_inner_1";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_1_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_1_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_1_LV, name, fsNaI_head_outer_1_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_1_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_1_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady1 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add2ndNaIFarSideAngle(G4double new_fsNaI_angle_2)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_2*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI2_head_outer_2";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_2_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_2_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_2_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI2_head_inner_2";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_2_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_2_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_2_LV, name, fsNaI_head_outer_2_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_2_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_2_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady2 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add3rdNaIFarSideAngle(G4double new_fsNaI_angle_3)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_3*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI3_head_outer_3";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_3_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_3_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_3_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI3_head_inner_3";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_3_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_3_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_3_LV, name, fsNaI_head_outer_3_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_3_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_3_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady3 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add4thNaIFarSideAngle(G4double new_fsNaI_angle_4)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_4*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI4_head_outer_4";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_4_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_4_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_4_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI4_head_inner_4";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_4_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_4_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_4_LV, name, fsNaI_head_outer_4_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_4_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_4_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady4 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add5thNaIFarSideAngle(G4double new_fsNaI_angle_5)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_5*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI5_head_outer_5";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_5_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_5_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_5_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI5_head_inner_5";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_5_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_5_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_5_LV, name, fsNaI_head_outer_5_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_5_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_5_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady5 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add6thNaIFarSideAngle(G4double new_fsNaI_angle_6)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_6*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI6_head_outer_6";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_6_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_6_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_6_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI6_head_inner_6";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_6_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_6_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_6_LV, name, fsNaI_head_outer_6_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_6_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_6_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady6 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add7thNaIFarSideAngle(G4double new_fsNaI_angle_7)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_7*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI7_head_outer_7";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_7_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_7_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_7_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI7_head_inner_7";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_7_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_7_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_7_LV, name, fsNaI_head_outer_7_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_7_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_7_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady7 = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDetectorConstruction::add8thNaIFarSideAngle(G4double new_fsNaI_angle_8)
{
    G4Material* fsNaI_head_outer_material = G4Material::GetMaterial("Al");
    G4Material* fsNaI_head_inner_material = G4Material::GetMaterial("NaI");

    G4double fsNaI_head_outer_rMin = 0.0*mm;
    G4double fsNaI_head_outer_rMax = 28.245*mm;
    G4double fsNaI_head_outer_Dz = 41.28*mm;
    G4double fsNaI_head_outer_SPhi = 0.0*deg;
    G4double fsNaI_head_outer_DPhi = 360.0*deg;

    G4double fsNaI_head_inner_rMin = 0.0*mm;
    G4double fsNaI_head_inner_rMax = 25.4*mm;
    G4double fsNaI_head_inner_Dz = 50.8*mm;
    G4double fsNaI_head_inner_SPhi = 0.0*deg;
    G4double fsNaI_head_inner_DPhi = 360.0*deg;

    fs_placement_angle = new_fsNaI_angle_8*deg;

    G4double fsNaI_placement_height = -10.0*cm;

    G4double fsNaI_head_inner_posX = 0.0*mm;
    G4double fsNaI_head_inner_posY = 0.0*mm;
    G4double fsNaI_head_inner_posZ = ((fsNaI_head_inner_Dz - fsNaI_head_outer_Dz)/2 + 0.508) *mm;

    G4double fsNaI_head_outer_posX = fs_placement_distance * cos(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posY = fs_placement_distance * sin(fs_placement_angle - angle_corr);
    G4double fsNaI_head_outer_posZ = fsNaI_placement_height;

    G4RotationMatrix *fsNaI_head_inner_rm = new G4RotationMatrix;
    G4RotationMatrix fsNaI_head_outer_rm = G4RotationMatrix();
    fsNaI_head_outer_rm.rotateY(270.0*deg);
    fsNaI_head_outer_rm.rotateZ(fs_placement_angle - angle_corr);

    G4String name;

    name = "fsNaI8_head_outer_8";
    G4Tubs* fsNaI_head_outer_S = new G4Tubs(name, fsNaI_head_outer_rMin, fsNaI_head_outer_rMax, fsNaI_head_outer_Dz/2.0, fsNaI_head_outer_SPhi, fsNaI_head_outer_DPhi);
    fsNaI_head_outer_8_LV = new G4LogicalVolume(fsNaI_head_outer_S, fsNaI_head_outer_material, name);
    G4Transform3D fsNaI_head_outer_transform(fsNaI_head_outer_rm, G4ThreeVector(fsNaI_head_outer_posX,fsNaI_head_outer_posY,fsNaI_head_outer_posZ));
    fsNaI_head_outer_8_PV = new G4PVPlacement(fsNaI_head_outer_transform, fsNaI_head_outer_8_LV, name, WorldLV, false, 0, fCheckOverlaps);

    name = "fsNaI8_head_inner_8";
    G4Tubs* fsNaI_head_inner_S = new G4Tubs(name, fsNaI_head_inner_rMin, fsNaI_head_inner_rMax, fsNaI_head_inner_Dz/2.0, fsNaI_head_inner_SPhi, fsNaI_head_inner_DPhi);
    fsNaI_head_inner_8_LV = new G4LogicalVolume(fsNaI_head_inner_S, fsNaI_head_inner_material, name);
    fsNaI_head_inner_8_PV = new G4PVPlacement(fsNaI_head_inner_rm, G4ThreeVector(fsNaI_head_inner_posX,fsNaI_head_inner_posY,fsNaI_head_inner_posZ), fsNaI_head_inner_8_LV, name, fsNaI_head_outer_8_LV, false, 0, fCheckOverlaps);

    G4VisAttributes* container_vis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
    container_vis->SetVisibility(true);
    G4VisAttributes* greenTVA = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5));
    fsNaI_head_outer_8_LV->SetVisAttributes(container_vis);
    fsNaI_head_inner_8_LV->SetVisAttributes(greenTVA);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    fNaIReady8 = true;
}
