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
/// \file GB06/include/myOptrSplitAndKillByImportance.hh
/// \brief Definition of the myOptrSplitAndKillByImportance class
//
//---------------------------------------------------------------
//
// myOptrSplitAndKillByImportance
//
// Class Description: §§§§
//        A G4VBiasingOperator concrete implementation example to
//    illustrate how to bias physics processes cross-section for
//    one particle type.
//        The G4VBiasingOperation G4BOptnChangeImportance is
//    selected by this operator, and is sent to each process
//    calling the operator.
//        A simple constant bias to the cross-section is applied,
//    but more sophisticated changes can be applied.
//
//---------------------------------------------------------------
//

#ifndef myOptrSplitAndKillByImportance_hh
#define myOptrSplitAndKillByImportance_hh 1

#include "G4VBiasingOperator.hh"


class myOptnSplitAndKillByImportance;
class G4BiasingProcessSharedData;
class G4ParallelGeometriesLimiterProcess;
class G4ParticleDefinition;
class G4VPhysicalVolume;
#include <map>


class myOptrSplitAndKillByImportance : public G4VBiasingOperator {
public:
  // ------------------------------------------------------------
  // -- Constructor: takes the name of the particle type to bias:
  // ------------------------------------------------------------
  myOptrSplitAndKillByImportance(G4String particleToBias,
                                 G4String name = "SplitAndKillByImportance");
  virtual ~myOptrSplitAndKillByImportance();

  // -- method called at beginning of run:
  virtual void StartRun();

private:
  // -----------------------------
  // -- Mandatory from base class:
  // -----------------------------
  // -- Not used:
  virtual G4VBiasingOperation*
  ProposeOccurenceBiasingOperation(const G4Track*,
                                   const G4BiasingProcessInterface*) final
  { return 0; }

  // -- Not used:
  virtual G4VBiasingOperation*
  ProposeFinalStateBiasingOperation(const G4Track*,
                                    const G4BiasingProcessInterface*) final
  { return 0; }

  // -- Used method : it will return the biasing operation that will split particles
  // -- with a probabilty depending on the total absorption cross-section.
  virtual G4VBiasingOperation*
  ProposeNonPhysicsBiasingOperation(const G4Track*,
                                    const G4BiasingProcessInterface*) final;


  // ---------------------------------------
  // -- Method specific to this application:
  // ---------------------------------------
public:
  void SetParallelWorld( G4VPhysicalVolume* parallelWorld )
  { fParallelWorld = parallelWorld; }

  // -- The importance map, linking a replica number to an volume importance:
  std::map< G4int, G4int >& GetImportanceMap() { return fImportanceMap; }



private:
  myOptnSplitAndKillByImportance*        fSplitAndKillByImportance;
  const G4ParticleDefinition*                      fParticleToBias;
  G4VPhysicalVolume*                                fParallelWorld;
  G4int                                        fParallelWorldIndex;
  const G4BiasingProcessSharedData*             fBiasingSharedData;
  const G4ParallelGeometriesLimiterProcess* fBiasingLimiterProcess;
  std::map< G4int, G4int >                          fImportanceMap;

};

#endif
