/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Define materials

     // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Air defined using NIST Manager
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

    // Water defined using NIST Manager
    G4Material* water = nist->FindOrBuildMaterial("G4_WATER");


    // Define volumes
    G4double env_sizeXYZ = 50.0 * cm;

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //  
    // World
    //
    G4double world_sizeXYZ = 1.2 * env_sizeXYZ;


    G4Box* solidWorld = new G4Box("World", 0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ);

    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,
            G4ThreeVector(),
            logicWorld,
            "World",
            0,
            false,
            0,
            checkOverlaps);


    // Sphericall shell section
    G4Sphere* solidSphereMax =
        new G4Sphere("sphere1",
            10.0 * cm,
            15.0 * cm,
            0.0 * deg,
            360.0 * deg,
            0.0 * deg,
            360.0 * deg);


    G4LogicalVolume* logicSphereMax =
        new G4LogicalVolume(solidSphereMax,
            water,
            "sphere1");

    G4ThreeVector positionSphereMax = G4ThreeVector(0, 0, 0);

    new G4PVPlacement(0,
        positionSphereMax,
        logicSphereMax,
        "Sphere1",
        logicWorld,
        false,
        0,
        checkOverlaps);

    // Sphere

    G4Sphere* solidSphereMin =
        new G4Sphere("detector",
            0.0 * cm,
            5.0 * cm,
            0.0 * deg,
            360.0 * deg,
            0.0 * deg,
            360.0 * deg);

    G4LogicalVolume* logicSphereMin =
        new G4LogicalVolume(solidSphereMin,
            water,
            "detectorLV");

    G4ThreeVector positionSphereMin = G4ThreeVector(0, 0, 0);

    new G4PVPlacement(0,
        positionSphereMin,
        logicSphereMin,
        "detector",
        logicWorld,
        false,
        0,
        checkOverlaps);

    // visualiation

    G4VisAttributes* boxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    G4VisAttributes* shellVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    G4VisAttributes* sphereVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));

    shellVisAtt->SetForceWireframe(true);

    logicWorld->SetVisAttributes(boxVisAtt);
    logicSphereMax->SetVisAttributes(shellVisAtt);
    logicSphereMin->SetVisAttributes(sphereVisAtt);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;


    //always return the physical World
    //
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    // declare detector as a MultiFunctionalDetector scorer
    //  
    G4MultiFunctionalDetector* detectorSphere = new G4MultiFunctionalDetector("detector");
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSphere);
    G4VPrimitiveScorer* primitiv1 = new G4PSDoseDeposit("dose");
    detectorSphere->RegisterPrimitive(primitiv1);
    SetSensitiveDetector("detectorLV", detectorSphere);
}