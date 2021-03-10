/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fCollID_detectorSphere(-1)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{   
	//Hits collections
    //  
	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	if (!HCE) return;

	// Get hits collections ID
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	fCollID_detectorSphere = SDMan->GetCollectionID("detector/dose");

	//Dose deposit in patient
    //
	G4double dose = 0.;

	std::map<G4int, G4double*>::iterator itr;

	G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_detectorSphere));

	for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
		///G4int copyNb  = (itr->first);
		dose = *(itr->second);
	}
	if (dose > 0.) fRunAction->SumDose(dose);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
