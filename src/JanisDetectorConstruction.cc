
/// \file JanisDetectorConstruction.cc
/// \brief Implementation of the JanisDetectorConstruction class

#include "JanisDetectorConstruction.hh"

#include "G4tgbVolumeMgr.hh"

#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
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
  fConstructed(false) // Whether the original volumes have been constructed
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDetectorConstruction::~JanisDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* JanisDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* JanisDetectorConstruction::DefineVolumes()
{
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    //===============  Materials ===============//

    G4NistManager* mat_man = G4NistManager::Instance(); //material mananger

    //
    G4Material* vacuum = mat_man -> FindOrBuildMaterial("G4_Galactic");
    G4Material* liquid_he = G4Material::GetMaterial("liquid_helium");
    G4Material* quartz = G4Material::GetMaterial("quartz");


    //===============  Dimensions ===============//

    // World

    G4double world_x = 30.*cm;
    G4double world_y = 30.*cm;
    G4double world_z = 30.*cm;

    // Liquid Helium Size (one of our sensitive volumes)

    G4double liquid_helium_sizeX = 25.7*mm;
    G4double liquid_helium_sizeY = 25.7*mm;
    G4double liquid_helium_sizeZ = 25.7*mm;

    // Quartz cell around liquid helium

    G4double quartz_cell_wall_thickness = 1.0*mm;

    G4double quartz_cell_exterior_sizeX = liquid_helium_sizeX+(2*quartz_cell_wall_thickness);
    G4double quartz_cell_exterior_sizeY = liquid_helium_sizeY+(2*quartz_cell_wall_thickness);
    G4double quartz_cell_exterior_sizeZ = liquid_helium_sizeZ+(2*quartz_cell_wall_thickness);

    //===============  Build Geometry ===============//

    G4String name;

    // World

    name = "world_box";
    G4Box* world_box = new G4Box(name, world_x/2.0, world_y/2.0, world_z/2.0);
    WorldLV = new G4LogicalVolume(world_box, vacuum, "WorldLV");
    WorldPV = new G4PVPlacement(0, G4ThreeVector(0,0,0), WorldLV, name, 0, false, 0, checkOverlaps);

    // Quartz cell

    name = "quartz_cell";
    G4VSolid* quartz_cell_exterior_S = new G4Box(name, quartz_cell_exterior_sizeX/2., quartz_cell_exterior_sizeY/2., quartz_cell_exterior_sizeZ/2.);
    G4LogicalVolume* quartz_cell_LV = new G4LogicalVolume(quartz_cell_exterior_S, quartz, name);
    new G4PVPlacement( 0, G4ThreeVector(), quartz_cell_LV, name, WorldLV, false, 0, fCheckOverlaps);
    quartz_cell_LV->SetVisAttributes( G4Color(1., 1., 0.,0.5) );

    // Liquid Helium

    name = "liquid helium";
    G4VSolid* liquid_helium_S = new G4Box(name, liquid_helium_sizeX/2., liquid_helium_sizeY/2., liquid_helium_sizeZ/2.);
    G4LogicalVolume* liquid_helium_LV = new G4LogicalVolume(liquid_helium_S, liquid_he, name);
    liquid_helium_PV = new G4PVPlacement( 0, G4ThreeVector(), liquid_helium_LV, name, quartz_cell_LV, false, 0, fCheckOverlaps);
    liquid_helium_LV->SetVisAttributes( G4Color(0., 0., 1.,0.5) );

    // World

    WorldLV->SetVisAttributes(G4VisAttributes::Invisible);
    fConstructed = true;

    return WorldPV;
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

    G4Element* elO  = new G4Element(name = "Oxygen"     , symbol = "O"  , z = 8.  , a =  15.999*g/mole);
    G4Element* elSi = new G4Element(name = "Silicon"    , symbol = "Si" , z = 14. , a =  28.086*g/mole);

    // Liquid helium material
    // Density from (assuming 3K LHe)
    // https://www.bnl.gov/magnets/staff/gupta/cryogenic-data-handbook/Section2.pdf

    new G4Material("liquid_helium",   z=2., a= 4.00*g/mole,  density= 0.141*g/cm3, kStateLiquid, temp=3.*kelvin);

    // quartz (SiO2, crystalline)
    G4Material* Quartz = new G4Material("quartz", 2.64 *g/cm3, nComponents= 2);
    Quartz-> AddElement(elSi, nAtoms=1);
    Quartz-> AddElement(elO,  nAtoms=2);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


