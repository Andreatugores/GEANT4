#include "B2aDetectorConstruction.hh"
#include "B2aDetectorMessenger.hh"
#include "B2TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

//-------------------------------------------------------------------------------
G4ThreadLocal
G4GlobalMagFieldMessenger* B2aDetectorConstruction::fMagFieldMessenger = 0;

B2aDetectorConstruction::B2aDetectorConstruction()
	:G4VUserDetectorConstruction(),
	fNbOfChambers(0),
	fLogicTarget(NULL), fLogicChamber(NULL),
	fTargetMaterial(NULL), fChamberMaterial(NULL),
	fStepLimit(NULL),
	fCheckOverlaps(true)
{
	fMessenger = new B2aDetectorMessenger(this);

	fNbOfChambers = 2;
	fLogicChamber = new G4LogicalVolume * [fNbOfChambers];
}

//--------------------------------------------------------------------------------

B2aDetectorConstruction::~B2aDetectorConstruction()
{
	delete[] fLogicChamber;
	delete fStepLimit;
	delete fMessenger;
}

//-------------------------------------------------------------------------------

G4VPhysicalVolume* B2aDetectorConstruction::Construct()
{
	// Define materials
	DefineMaterials();

	// Define volumes
	return DefineVolumes();

}

//------------------------------------------------------------------------------

void B2aDetectorConstruction::DefineMaterials()
{
	// Material definition 

	G4NistManager* nistManager = G4NistManager::Instance();

	// Air defined using NIST Manager
	nistManager->FindOrBuildMaterial("G4_AIR");

	// Water defined using NIST Manager
	nistManager->FindOrBuildMaterial("G4_WATER");

	//
	nistManager->FindOrBuildMaterial("G4_A-150_TISSUE");

	// Lead defined using NIST Manager
	fTargetMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

	// Xenon gas defined using NIST Manager
	fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//---------------------------------------------------------------------------------

G4VPhysicalVolume* B2aDetectorConstruction::DefineVolumes()
{
	G4Material* air = G4Material::GetMaterial("G4_AIR");
	G4Material* water = G4Material::GetMaterial("G4_WATER");
	G4Material* tissue = G4Material::GetMaterial("G4_A-150_TISSUE");

	//Tamaño de los solidos

	G4double chamberSpacing = 55.0 * cm; //del centro de la camara al centro

	G4double chamberWidth = 25.0 * cm; //ancho de la camara
	G4double targetLength = 7.0 * cm; //longitud del target

	G4double trackerRadius = (fNbOfChambers + 1) * chamberSpacing;

	G4double worldLength = 1.2 * (2 * targetLength + trackerRadius);

	G4double targetRadius = 0.5 * targetLength; //radio del blanco
	targetLength = 0.5 * targetLength; //longitud media del target
	G4double trackerSize = 0.5 * trackerRadius; //longitud media del tracker

	//definición de cada sóido, volumen sólido y volumen físico

	//definición del world volume

	G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
	G4cout << "Compute tolerance ="
		<< G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
		<< "mm" << G4endl;

	G4Box* solidWorld = new G4Box("World", 0.5 * worldLength, 0.5 * worldLength, 0.5 * worldLength);

	//Volumen lógico del world volume

	G4LogicalVolume* worldLogic
		= new G4LogicalVolume(solidWorld, //el sólido
			air, //material del sólido
			"World"); //nombre

//Volumen físico del world volume sin rotar en (0,0,0)

	G4VPhysicalVolume* worldPhysical
		= new G4PVPlacement(
			0, //sin rotar
			G4ThreeVector(), //(0,0,0)
			worldLogic, //volumen lógico
			"World", //nombre
			0, //volumen madre
			false, //operaciones booleanas
			0, //número de copias
			fCheckOverlaps); //superposiciones

	//definición del target
	G4ThreeVector positionTarget = G4ThreeVector(0, 0, -(targetLength + trackerSize));

	//Creando el solido Target
	G4Tubs* solidTarget = new G4Tubs("target", //nombre
		0, //radio interno
		targetRadius, //radio externo
		targetLength, //longitud 
		0.0 * deg, //comienza el barrido de phi
		360.0 * deg); //termina el barrido de phi

	//Creando el volumen lógico del Target

	fLogicTarget = new G4LogicalVolume(solidTarget, //sólido target
		fTargetMaterial, //material del sólido
		"target", //nombre
		0, //campo 
		0, //sensiblidad
		0); //límites de usuario

	//Volumen físico del Target

	new G4PVPlacement(0, //sin rotación
			positionTarget, //en (x,y,z)
			fLogicTarget, //volumen lógico
			"target", //nombre
			worldLogic, //volumen madre
			false, //operaciones booleanas
			0, //número de copias
			fCheckOverlaps); //superposiciones

	G4cout << "Target is " << 2 * targetLength / cm << " cm of "
		<< fTargetMaterial->GetName() << G4endl;

	//Creando el Tracker
	//sólido del tracker

	G4ThreeVector positionTracker = G4ThreeVector(0, 0, 0);

	G4Sphere* solidTracker
		= new G4Sphere("tracker", //nombre
			0, //radio interno
			0.25 * worldLength, //radio externo
			0.0 * deg,
			360.0 * deg,
			0.0 * deg,
			360.0 * deg);

	//volumen lógico del tracker
	G4LogicalVolume* trackerLogic
		= new G4LogicalVolume(solidTracker, //sólido 
			tissue, //material del sólido
			"tracker", //nombre
			0,
			0,
			0);

	//volumen físico del tracker
	new G4PVPlacement(0, // sin rotación
		positionTracker, //(0,0,0)
		trackerLogic, //volumen lógico
		"tracker", //nombre
		worldLogic, //volumen madre
		false, //operaciones booleanas
		0, //número de copias
		fCheckOverlaps); //superposicones


	//Atributos de visualización
	G4VisAttributes* boxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	G4VisAttributes* targetVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
	G4VisAttributes* trackerVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));

	worldLogic ->SetVisAttributes(boxVisAtt);
	fLogicTarget ->SetVisAttributes(targetVisAtt);
	trackerLogic ->SetVisAttributes(trackerVisAtt);


	//tracker region (camaras)

	G4cout << "There are " << fNbOfChambers << " chambers in the tracker region. "
		<< G4endl
		<< "The chambers are " << chamberWidth / cm << " cm of "
		<< fChamberMaterial->GetName() << G4endl
		<< "The distance between chamber is " << chamberSpacing / cm << " cm"
		<< G4endl;

	G4double firstPosition = -trackerSize + chamberSpacing;
	G4double firstLength = trackerRadius / 10;
	G4double lastLength = trackerRadius / 5;

	G4double halfWidth = 0.5* chamberWidth;
	G4double rmaxFirst = 0.5 * firstLength;

	G4double rmaxIncr = 0.0;
	if (fNbOfChambers > 0) {
		rmaxIncr = 0.5 * (lastLength - firstLength) / (fNbOfChambers - 1);
		if (chamberSpacing < chamberWidth) {
			G4Exception("B2aDetectorConstruction::DefineVolumes()",
				"InvalidSetup", FatalException,
				"Width>Spacing");
		}
	}

	for (G4int copyN = 0; copyN < fNbOfChambers; copyN++) {

		G4double Zposition = firstPosition + copyN * chamberSpacing;
		G4double rmax = rmaxFirst + copyN * rmaxIncr;

		//Creando el sólido para las cámaras

		G4Tubs* chamberSolid
			= new G4Tubs("chamber", 0, rmax, halfWidth, 0. * deg, 360. * deg);

		//Volumen lógico de las cámaras

		fLogicChamber[copyN] =
			new G4LogicalVolume(chamberSolid, //sólido
				fChamberMaterial, //material de las cámaras
				"chamber_LV", //nombre
				0,
				0,
				0);

		//asignando el material
		fLogicChamber[copyN]->SetVisAttributes(chamberVisAtt);

		//volumen físico de las cámaras

		new G4PVPlacement(0, //sin rotación
			G4ThreeVector(0, 0, Zposition), //(x,y,z)
			fLogicChamber[copyN], //volumen lógico
			"chamber", //nombre
			trackerLogic, //volumen madre
			false, //operaciones booleanas
			copyN, //número de copias
			fCheckOverlaps); //superposiciones

	}
	
	//Establecer límites de usuario

	//Establece restricciones de seguimiento en un determinado volumen lógico

	//Usar G4StepLimiter para establecer una longitud máxima de paso en la región del tracker

	G4double maxStep = 0.5 * chamberWidth;
	fStepLimit = new G4UserLimits(maxStep);
	trackerLogic->SetUserLimits(fStepLimit);

	//Siempre se returna el volumen físico

	return worldPhysical;
}
	//------------------------------------------------------------------------------------------------------

	//Construir el dectector con el método: 

void B2aDetectorConstruction::ConstructSDandField()
{
	//Detectores sensibles

	G4String trackerChamberSDname = "B2/TrackerChamberSD";
	B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
									  "TrackerHitsCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
		
	//Configuración de aTrackerSD para todos los volumenes lógicos con el mismo nombre

	SetSensitiveDetector("chamber_LV", aTrackerSD, true);

	//Crear un campo magnético global
	//Se crea el campo magnético automaticamente si el valor del campo no es cero

	G4ThreeVector fieldValue = G4ThreeVector();
	fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
	fMagFieldMessenger->SetVerboseLevel(1);

	G4AutoDelete::Register(fMagFieldMessenger);
}

//----------------------------------------------------------------------------------------------------------

//Para poder cambiar el material del target

void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
{
	G4NistManager* nistManager = G4NistManager::Instance();

	G4Material* pttoMaterial =
			nistManager->FindOrBuildMaterial(materialName);

	if (fTargetMaterial != pttoMaterial) {
		if (pttoMaterial) {
			fTargetMaterial = pttoMaterial;
			if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
			G4cout
				<< G4endl
				<< "----> The target is made of " << materialName << G4endl;
		} else {
				G4cout
					<< G4endl
					<< "-->  WARNING from SetTargetMaterial : "
					<< materialName << " not found" << G4endl;
		
		}
	}
}

//--------------------------------------------------------------------------------------

//Para poder cambiar el material de la cámara

void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)
{
	G4NistManager* nistManager = G4NistManager::Instance();

	G4Material* pttoMaterial =
		nistManager->FindOrBuildMaterial(materialName);

	if (fChamberMaterial != pttoMaterial) {
		if (pttoMaterial) {
			fChamberMaterial = pttoMaterial;
			for (G4int copyNo = 0; copyNo < fNbOfChambers; copyNo++) {
				if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
					SetMaterial(fChamberMaterial);
			}
			G4cout
				<< G4endl
				<< "----> The chambers are made of " << materialName << G4endl;
		}
		else {
			G4cout
				<< G4endl
				<< "-->  WARNING from SetChamberMaterial : "
				<< materialName << " not found" << G4endl;
		}
	}
}

//-------------------------------------------------------------------------------------------------

void B2aDetectorConstruction::SetMaxStep(G4double maxStep)
{
	if ((fStepLimit) && (maxStep > 0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//---------------------------------------------------------------------------

void B2aDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
	fCheckOverlaps = checkOverlaps;
}






