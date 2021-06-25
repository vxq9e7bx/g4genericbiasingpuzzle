#include "myDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myDetectorConstruction::myDetectorConstruction()
    : G4VUserDetectorConstruction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myDetectorConstruction::~myDetectorConstruction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* myDetectorConstruction::Construct() {
    // Define materials via NIST manager
    auto nistManager = G4NistManager::Instance();

    G4double atomicNumber = 1.;
    G4double massOfMole = 1.008*g/mole;
    G4double density = 1.e-25*g/cm3;
    G4double temperature = 2.73*kelvin;
    G4double pressure = 3.e-18*pascal;
    G4Material* vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density, kStateGas, temperature, pressure);

    G4Material* scatter = nistManager->FindOrBuildMaterial("G4_Al");

    auto worldMaterial = vacuum;

    // World
    //
    auto sWorld
            = new G4Box("World", 25.*cm, 25.*m, 25.*cm);

    auto fWorldVolume
            = new G4LogicalVolume(sWorld,               //shape
                                  worldMaterial,        //material
                                  "World");             //name

    auto pWorld
            = new G4PVPlacement(0,                      //no rotation
                                G4ThreeVector(),        //at (0,0,0)
                                fWorldVolume,           //logical volume
                                "World",                //name
                                0,                      //mother  volume
                                false,                  //no boolean operation
                                0);                     //copy number

    G4Box *Box = new G4Box("Scatter", 5.*mm, 5.*mm, 5.*mm);
    G4LogicalVolume *l_Box = new G4LogicalVolume(Box, scatter, "Scatter");
//    new G4PVPlacement(nullptr,
//                      G4ThreeVector(0,0,0),
//                      l_Box,
//                      "Box",
//                      fWorldVolume,
//                      false,
//                      0);
//    l_Box->SetVisAttributes(G4Color(1., 1., 0., 0.25));

    //always return the root volume
    return pWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myDetectorConstruction::ConstructSDandField()
{
}

