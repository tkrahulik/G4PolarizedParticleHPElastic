
#ifndef G4PolarizedParticleHPAnalyzingPower_h
#define G4PolarizedParticleHPAnalyzingPower_h 1

#include "globals.hh"
#include <vector>

class G4PolarizedParticleHPAnalyzingPower
{
public:

  G4PolarizedParticleHPAnalyzingPower();

  ~G4PolarizedParticleHPAnalyzingPower();


  G4double GetAnalyzingPower(G4int targetZ, G4int targetA, G4ParticleDefinition* particle, G4double eKinetic, G4double sTheta, G4String process);
  G4double Evaluate(G4double eKin, G4double sTheta);
  G4double Interpolate(G4double x, G4double x1, G4double x2, G4double y1, G4double y2)
  {return (y1*(x2-x) + y2*(x-x1))/(x2-x1);}

private:

  std::vector<G4double> vecEnergy;
  std::vector<std::vector<G4double> > vecAngle;
  std::vector<std::vector<G4double> > vecAPower;

};
#endif
