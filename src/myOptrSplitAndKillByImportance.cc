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
/// \file src/myOptrSplitAndKillByImportance.cc
/// \brief Implementation of the myOptrSplitAndKillByImportance class
//
#include "myOptrSplitAndKillByImportance.hh"
#include "myOptnSplitAndKillByImportance.hh"

#include "G4BiasingProcessInterface.hh"
#include "G4ParallelGeometriesLimiterProcess.hh"
#include "G4BiasingProcessSharedData.hh"


#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"

#include "G4TransportationManager.hh"
#include "G4TouchableHistoryHandle.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myOptrSplitAndKillByImportance::
myOptrSplitAndKillByImportance(G4String particleName,
                               G4String name)
    : G4VBiasingOperator(name),
      fParallelWorld         ( nullptr ),
      fParallelWorldIndex    ( -1 ),
      fBiasingLimiterProcess ( nullptr )
{
    auto particleToBias = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
    if ( particleToBias == 0 )
    {
        G4ExceptionDescription ed;
        ed << "Particle `" << particleName << "' not found !" << G4endl;
        G4Exception("myOptrSplitAndKillByImportance(...)",
                    "ESRF.01",
                    JustWarning,
                    ed);
    }

    fParticleToBias = particleToBias;
    fSplitAndKillByImportance =
            new myOptnSplitAndKillByImportance("splitterFor_"+particleName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myOptrSplitAndKillByImportance::~myOptrSplitAndKillByImportance()
{
    delete fSplitAndKillByImportance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4ProcessManager.hh"
class G4ProcessManager;

void myOptrSplitAndKillByImportance::StartRun()
{
    // ---------------
    // -- Setup stage:
    // ---------------
    // -- get the particle process manager...
//    G4ProcessManager* processManager = fParticlesToBias[0]->GetProcessManager();
    G4ProcessManager* processManager = fParticleToBias->GetProcessManager();
    // -- ... to obtain the biasing information shared among this particle processes:
    fBiasingSharedData = G4BiasingProcessInterface::GetSharedData( processManager );
    // -- Remember the index of the parallel world:
    fBiasingLimiterProcess = fBiasingSharedData->GetParallelGeometriesLimiterProcess();
    fParallelWorldIndex    = fBiasingLimiterProcess->GetParallelWorldIndex(fParallelWorld);

    // -- Setup the biasing operation:
    fSplitAndKillByImportance->SetBiasingSharedData( fBiasingSharedData  );
    fSplitAndKillByImportance->SetParallelWorldIndex( fParallelWorldIndex );
    fSplitAndKillByImportance->SetImportanceMap( &fImportanceMap     );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation*
myOptrSplitAndKillByImportance::
ProposeNonPhysicsBiasingOperation(const G4Track* track,
                                  const G4BiasingProcessInterface* /*callingProcess*/)
{
    // -- Check if current particle type is the one to bias:
    if ( track->GetDefinition() == fParticleToBias ) return fSplitAndKillByImportance;
    return 0;
}
