#include "G4PolarizedParticleHPAnalyzingPower.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "globals.hh"


G4double G4PolarizedParticleHPAnalyzingPower::GetAnalyzingPower(G4int targetZ, G4int targetA, G4ParticleDefinition* particle, G4double eKinetic, G4double sTheta, G4String process)
{

  //Use input parameters to obtain data file name
  G4String filename = "Z";
  filename += static_cast<ostringstream*>( &(ostringstream() << targetZ)  )->str();
  filename += "A";
  filename += static_cast<ostringstream*>( &(ostringstream() << targetA)  )->str();
  filename += "_";
  filename += process;
  filename += ".dat";

  ifstream infile( filename.c_str() );
  G4String line;

  //Set analyzing power as zero if no data file available
  if(!infile){return 0.0;}

  G4double result;
  G4double dataEnergy, dataAngle, dataAPower;

  std::vector<G4double> tempAngle, tempAPower;

  G4int index = 0;

  while(getline(infile, line))
    {
      std::istringstream theData(line);

      if(line[0] != '#')
	{
	  theData >> dataEnergy >> dataAngle >> dataAPower;

	  //If reading in data for new energy value
	  if(dataEnergy != vecEnergy[index] || vecEnergy.size() == 0)
	    {
	      vecEnergy.push_back(dataEnergy);
	      vecAngle.push_back(tempAngle);
	      vecAPower.push_back(tempAPower);

	      tempAngle.clear();
	      tempAPower.clear();

	      tempAngle.push_back(dataAngle);
	      tempAPower.push_back(dataAPower);
	      index++;
	    }
	  else
	    {
	      tempAngle.push_back(dataAngle);
	      tempAPower.push_back(dataAPower);
	    }
	}
    }

  return Evaluate(eKinetic, sTheta);
}

G4double G4PolarizedParticleHPAnalyzingPower::Evaluate(G4double eKin, G4double sTheta)
{

  G4double interTheta1, interTheta2, result;

  for(G4int i=0; i < vecEnergy.size(); i++)
    {
      if(eKin >= vecEnergy[i] && eKin < vecEnergy[i+1])
	{
	  for(G4int j1=0; j1 < vecAngle[i].size(); j1++)
	    {
	      if(sTheta >= vecAngle[i][j1] && sTheta < vecAngle[i][j1+1])
		{
		  interTheta1 = Interpolate(sTheta, vecAngle[i][j1], vecAngle[i][j1+1], vecAPower[i][j1], vecAPower[i][j1+1]);
		}
	    }
	  for(G4int j2=0; j2 < vecAngle[i].size(); j2++)
	    {
	      if(sTheta >= vecAngle[i][j2] && sTheta < vecAngle[i][j2+1])
		{
		  interTheta2 = Interpolate(sTheta, vecAngle[i][j2], vecAngle[i][j2+1], vecAPower[i][j2], vecAPower[i][j2+1]);
		}
	    }

	  result = Interpolate(eKin, vecEnergy[i], vecEnergy[i+1], interTheta1, interTheta2);
	}
    }

  return result;

}

// G4double G4PolarizedParticleHPAnalyzingPower::Interpolate(G4double x, G4double x1, G4double x2, G4double y1, G4double y2)
// { return ( y1*(x2-x) + y2*(x-x1) ) / (x2 - x1); }
