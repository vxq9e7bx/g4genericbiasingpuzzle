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
/// \file myOptnSplitAndKillByImportance.cc
/// \brief Implementation of the myOptnSplitAndKillByImportance class

#include "myOptnSplitAndKillByImportance.hh"
#include "Randomize.hh"


#include "G4BiasingProcessInterface.hh"
#include "G4ParallelGeometriesLimiterProcess.hh"
#include "G4BiasingProcessSharedData.hh"
#include "G4ProcessManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myOptnSplitAndKillByImportance::myOptnSplitAndKillByImportance(G4String name)
: G4VBiasingOperation (name),
  fParallelWorldIndex ( -1 ),
  fBiasingSharedData  ( nullptr ),
  fParticleChange(),
  fDummyParticleChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myOptnSplitAndKillByImportance::~myOptnSplitAndKillByImportance()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double myOptnSplitAndKillByImportance::
DistanceToApplyOperation( const G4Track* track,
                          G4double,
                          G4ForceCondition* condition)
{

  // -- Remember the touchable history (ie geometry state) at the beginning of the step:
  // -- Start by getting the process handling the step limitation in parallel
  // -- geometries for the generic biasing:
  auto biasingLimiterProcess =  fBiasingSharedData->GetParallelGeometriesLimiterProcess();
  fPreStepTouchableHistory   =
    biasingLimiterProcess
    ->GetNavigator( fParallelWorldIndex )  // -- get the navigator of the geometry...
    ->CreateTouchableHistoryHandle();      // -- ...and collect the geometry state.

  // -- We request to be "forced" : meaning GenerateBiasingFinalState(...) will be called
  // -- anyway at the PostStepDoIt(...) stage (ie, when the track will have been moved to
  // -- its end point position) and, there, we will have to handle the decision to
  // -- split/kill if we are on a volume boundary, or do nothing, if we're not:
  *condition = Forced;

  G4String previousVolume = "void";
  if(fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetPreviousVolume( fParallelWorldIndex ))
      previousVolume = fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetPreviousVolume( fParallelWorldIndex )->GetName();

  G4cerr
          << track->GetTrackID() << " / " << track->GetCurrentStepNumber()
          << " DistanceToApply: "
          <<previousVolume
          << " -> "
          << fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetCurrentVolume( fParallelWorldIndex )->GetName()
          << " Current Volume: "
          << fPreStepTouchableHistory->GetVolume()->GetName()
          << G4endl;


  // -- As we're forced, we make this returned length void:
  return DBL_MAX;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange*
myOptnSplitAndKillByImportance::GenerateBiasingFinalState( const G4Track* track,
                                                              const G4Step* step)
{
  // -- Given we used the "Forced" condition, this method is always called.
  // -- We check the status of the step in the parallel geometry, and apply
  // -- splitting/killing if the step has been limited by the geometry.

  // -- We first check if we limit the step in the parallel geometry:
  G4bool isLimiting = fBiasingSharedData
    ->GetParallelGeometriesLimiterProcess()
    ->GetIsLimiting( fParallelWorldIndex );



//  if(fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetWasLimiting( fParallelWorldIndex ))
//      G4cerr
//              << track->GetTrackID()
//              << " WAS LIMITING" << G4endl;

//  if(fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetIsLimiting( fParallelWorldIndex ))
//      G4cerr
//              << track->GetTrackID()
//              << " IS LIMITING" << G4endl;

  // -- if not limiting, we leave the track unchanged:
  if ( !isLimiting )
    {
      G4cerr
              << track->GetTrackID()
              << " should return." << G4endl;
      fDummyParticleChange.Initialize( *track );
//      return &fDummyParticleChange;
    }

  // -- We collect the geometry state at the end point step:
  // -- Note this is the same call than in the DistanceToApplyOperation(...) for the
  // -- fPreStepTouchableHistory, but the navigator has pushed the track in the next
  // -- volume since then (even if the track is still on the boundary), and hence the
  // -- geometry state has changed.
  auto biasingLimiterProcess = fBiasingSharedData->GetParallelGeometriesLimiterProcess();
  fPostStepTouchableHistory  =
    biasingLimiterProcess
    ->GetNavigator( fParallelWorldIndex )
    ->CreateTouchableHistoryHandle();

  G4cerr
          << track->GetTrackID() << " / " << track->GetCurrentStepNumber()
          << " GenerateBiasingFinalState: "
          << fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetPreviousVolume( fParallelWorldIndex )->GetName()
          << " -> "
          << fBiasingSharedData->GetParallelGeometriesLimiterProcess()->GetCurrentVolume( fParallelWorldIndex )->GetName()
          << " Current Volume: "
          << fPostStepTouchableHistory->GetVolume()->GetName()
          << " Pos: "
          << step->GetPostStepPoint()->GetPosition()
          << G4endl;

  G4int  preIndex =  fPreStepTouchableHistory->GetVolume()->GetLogicalVolume()->GetInstanceID();
  G4int postIndex = fPostStepTouchableHistory->GetVolume()->GetLogicalVolume()->GetInstanceID();

  G4cerr
          << track->GetTrackID()
          << " from id: " << preIndex << " "
          << " to id: " << postIndex << " "
          << fPreStepTouchableHistory->GetVolume()->GetName() << " -> "<< fPostStepTouchableHistory->GetVolume()->GetName() << G4endl;

  if ( !isLimiting )
  {
      G4cerr
              << track->GetTrackID()
              << " not doing anything" << G4endl;
      fDummyParticleChange.Initialize( *track );
      return &fDummyParticleChange;
  }

  // -- and get the related importance we defined in the importance map:
  G4double  preImportance = (*fImportanceMap)[ preIndex];
  G4double postImportance = (*fImportanceMap)[postIndex];

//  G4cerr << "pre: " << preIndex << " " << preImportance << G4endl;
//  G4cerr << "post: " << postIndex << " " << postImportance << G4endl;
  // -- Get track weight:
  G4double initialWeight = track->GetWeight();

  // -- Initialize the "particle change" (it will communicate the new track state to
  // -- the tracking):
  fParticleChange.Initialize(*track);

  if ( postImportance > preImportance )
    {
      G4double splittingFactor = G4double(postImportance) / G4double(preImportance);
      G4cerr
              << track->GetTrackID()
              << " should split: " << splittingFactor << G4endl;
      // Define the tracks weight:
      G4double weightOfTrack = initialWeight/splittingFactor;

      // Ask currect track weight to be changed to the new value:
      fParticleChange.ProposeParentWeight( weightOfTrack );
      fParticleChange.SetNumberOfSecondaries( G4int(splittingFactor)-1 );
      for ( G4int iSplit = 1 ; iSplit <  splittingFactor ; iSplit++ )
      {
          G4Track* clone = new G4Track( *track );
//          clone->SetPosition(track->GetPosition()+10.*CLHEP::um*track->GetMomentumDirection());
          clone->SetWeight( weightOfTrack );
          fParticleChange.AddSecondary( clone );
      }
      fParticleChange.SetSecondaryWeightByProcess(true); // -- tricky
      // -- take it as is ;) [though not necessary here, put for safety]
    }
  else
    {
      // -- We apply Russian roulette:
      G4double survivingProbability = G4double(postImportance) / G4double(preImportance);
      G4cerr
              << track->GetTrackID()
              << " killing: " << survivingProbability << G4endl;
      // Shoot a random number (in ]0,1[ segment):
      G4double random = G4UniformRand();
      if ( random > survivingProbability )
        {
          // We ask for the the track to be suspended (otherwise, prior secondaries also get killed):
          fParticleChange.ProposeTrackStatus(fStopAndKill);
          G4cerr
                  << track->GetTrackID()
                  << "killed" << G4endl;
        }
      else
        {
          // In this case, the track survives. We change its weight
          // to conserve weight among killed and survival tracks:
          fParticleChange.ProposeParentWeight( initialWeight/survivingProbability );
        }
    }

  return &fParticleChange;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
