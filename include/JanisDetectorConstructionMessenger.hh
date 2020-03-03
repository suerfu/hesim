/// \file JanisDetectorConstructionMessenger.hh
/// \brief Definition of the JanisDetectorConstructionMessenger class

#ifndef JanisDetectorConstructionMessenger_h
#define JanisDetectorConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class JanisDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class JanisDetectorConstructionMessenger: public G4UImessenger
{
    public:
        JanisDetectorConstructionMessenger(JanisDetectorConstruction* );
        virtual ~JanisDetectorConstructionMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

      private:
        JanisDetectorConstruction* DetectorPlacement;

        G4UIdirectory* DetectorPlacementDir;
        G4UIcmdWithADouble* AngleCmd;
        G4UIcmdWithADouble* FSDistanceCmd;
        G4UIcmdWithADouble* GeneratorDistanceCmd;
        G4UIcmdWithADouble* PMTAngleCmd;
        G4UIcmdWithADouble* NewAngle1Cmd;
        G4UIcmdWithADouble* NewAngle2Cmd;
        G4UIcmdWithADouble* NewAngle3Cmd;
        G4UIcmdWithADouble* NewAngle4Cmd;
        G4UIcmdWithADouble* NewAngle5Cmd;
        G4UIcmdWithADouble* NewAngle6Cmd;
        G4UIcmdWithADouble* NewAngle7Cmd;
        G4UIcmdWithADouble* NewAngle8Cmd;
        G4UIcmdWithADouble* NewNaIAngle1Cmd;
        G4UIcmdWithADouble* NewNaIAngle2Cmd;
        G4UIcmdWithADouble* NewNaIAngle3Cmd;
        G4UIcmdWithADouble* NewNaIAngle4Cmd;
        G4UIcmdWithADouble* NewNaIAngle5Cmd;
        G4UIcmdWithADouble* NewNaIAngle6Cmd;
        G4UIcmdWithADouble* NewNaIAngle7Cmd;
        G4UIcmdWithADouble* NewNaIAngle8Cmd;
};

#endif
