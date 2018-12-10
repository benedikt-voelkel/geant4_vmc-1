//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file MEMCMultiStack.cxx
/// \brief Implementation of the MEMCMultiStack class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \date 10/12/2018
/// \author B. Volkel; University Heidelberg
#include <iostream>

#include "MEMCMultiStack.h"
#include "MEParticle.h"

#include <TParticle.h>
#include <TObjArray.h>
#include <TError.h>

/// \cond CLASSIMP
ClassImp(MEMCMultiStack)
/// \endcond

//_____________________________________________________________________________
MEMCMultiStack::MEMCMultiStack(Int_t size)
  : TVirtualMCMultiStack(),
    fParticles(0),
    fNPrimary(0)
{
/// Standard constructor
/// \param size  The stack size
  fParticles = new TObjArray(size);
}

//_____________________________________________________________________________
MEMCMultiStack::MEMCMultiStack()
  : TVirtualMCMultiStack(),
    fParticles(0),
    fNPrimary(0)
{
/// Default constructor
}

//_____________________________________________________________________________
MEMCMultiStack::~MEMCMultiStack()
{
/// Destructor

  if (fParticles) fParticles->Delete();
  delete fParticles;
}

// private methods

//_____________________________________________________________________________
MEParticle*  MEMCMultiStack::GetParticle(Int_t id) const
{
/// \return   The \em id -th particle in fParticles
/// \param id The index of the particle to be returned

  if (id < 0 || id >= fParticles->GetEntries())
    Fatal("GetParticle", "Index out of range");

  return dynamic_cast<MEParticle*>(fParticles->At(id));
}


// public methods

//_____________________________________________________________________________
void MEMCMultiStack::Reset()
{
   // /fParticles->Delete();
   fParticles->Clear();
   fNPrimary = 0;
}

//_____________________________________________________________________________
TParticle* MEMCMultiStack::PushUserTrack(Int_t parent, Int_t pdg,
                                         Double_t px,Double_t py, Double_t pz,
                                         Double_t e, Double_t vx, Double_t vy,
                                         Double_t vz, Double_t tof, Double_t polx,
                                         Double_t poly, Double_t polz,
                                         Int_t geoStateIndex,
                                         ETrackTransportStatus transportStatus,
                                         TMCProcess mech, Int_t& ntr,
                                         Double_t weight, Int_t is)
    {
/// Create a new particle and push into stack;
/// adds it to the particles array (fParticles) and if not done to the
/// stack (fStack).
/// \param toBeDone  1 if particles should go to tracking, 0 otherwise
/// \param parent    number of the parent track, -1 if track is primary
/// \param pdg       PDG encoding
/// \param px        particle momentum - x component [GeV/c]
/// \param py        particle momentum - y component [GeV/c]
/// \param pz        particle momentum - z component [GeV/c]
/// \param e         total energy [GeV]
/// \param vx        position - x component [cm]
/// \param vy        position - y component  [cm]
/// \param vz        position - z component  [cm]
/// \param tof       time of flight [s]
/// \param polx      polarization - x component
/// \param poly      polarization - y component
/// \param polz      polarization - z component
/// \param mech      creator process VMC code
/// \param ntr       track number (is filled by the stack
/// \param weight    particle weight
/// \param is        generation status code





  const Int_t kFirstDaughter=-1;
  const Int_t kLastDaughter=-1;

  TParticle* particleDef
    = new TParticle(pdg, is, parent, -1, kFirstDaughter, kLastDaughter,
         px, py, pz, e, vx, vy, vz, tof);

  particleDef->SetPolarisation(polx, poly, polz);
  particleDef->SetWeight(weight);
  particleDef->SetUniqueID(mech);

  MEParticle* mother = 0;
  if ( parent >= 0 )
    mother = GetParticle(parent);
  else
    fNPrimary++;

  ntr = fParticles->GetEntries();

  MEParticle* particle = new MEParticle(ntr, particleDef, mother);
  if (mother) mother->AddDaughter(particle);
  fParticles->Add(particle);

  return particleDef;
}

//_____________________________________________________________________________
void MEMCMultiStack::PrintStack() const
{
  std::cerr << "Print tracks on stack\n"
            << "\t#particles: " << fParticles->GetEntriesFast() << "\n";
  for(Int_t i = 0; i < fParticles->GetEntriesFast(); i++) {
    std::cerr << "+++++++++++++ particle " << i << "+++++++++++++\n";
    (dynamic_cast<MEParticle*>(fParticles->At(i)))->GetParticle()->Print();
    std::cerr << "\n";
  }
}
