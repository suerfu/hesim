
// $Id: JanisDDGeneratorMessenger.hh $
//
/// \file JanisDDGeneratorMessenger.hh
/// \brief Definition of the JanisDDGeneratorMessenger class

#ifndef JanisDDGeneratorMessenger_h
#define JanisDDGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

class JanisDDGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

class JanisDDGeneratorMessenger : public G4UImessenger
{
public:

	JanisDDGeneratorMessenger(JanisDDGeneratorAction*);
	virtual ~JanisDDGeneratorMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);

private:

	JanisDDGeneratorAction* primaryGenerator;

	G4UIdirectory* primaryGeneratorDir;

	G4UIcmdWithADouble* generatorDistanceCmd;
	G4UIcmdWithADouble* generatorAngleCmd;
	G4UIcmdWithAString* generatorModeCmd;

};

#endif
