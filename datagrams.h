#ifndef DATAGRAMS_H
#define DATAGRAMS_H
#include "LynxStructure.h"

using namespace LynxStructureSpace;
// **************************************************************************
// the includes


// **************************************************************************
// the defines

// **************************************************************************
// the typedefs


// **************************************************************************
// the globals


// **************************************************************************
// the functions (.c/c++)

// **************************************************************************
// the function prototypes (.h)

//! \brief Enumeration to define the interrupt numbers
//!
enum DriveDatagram{
    eDriveControl,      //!< Denotes interrupt number 1
    eDriveTorqueRef,    //!< Denotes interrupt number 1
    eDriveSpeedRef,     //!< Denotes interrupt number 1
};

static const LynxStructure::InitParam driveParams[] =
{
    { eDriveControl, eUint8 },
    { eDriveTorqueRef, eUint32 },
    { eDriveSpeedRef, eUint32 },
    { 0, eEndOfList }
};

enum MotorDatagram{
    eMotorPoles,
    eMotorResistance,
    eMotorDInductance,
    eMotorQInductance,
    eMotorBackEmf,
    eMotorMaxAcceleration,
    eMotorMaxSpeed,
    eMotorMaxCurrent,
    eMotorMaxCurrentOverload,
    eMotorMaxVoltage,
    eMotorPwmFrequency
};
enum InverterDatagram{
    eInverterMaxCurrent,
    eInverterMaxVoltage,
    eEncoderResolution,
    eEncoderStartup,
    eEncoderOffset
};
enum ControlDatagram
{
    eControlPositionGainP,
    eControlPositionGainI,
    eControlPositionGainD,
    eControlSpeedGainP,
    eControlSpeedyGainI,
    eControlCurrentGainP,
    eControlCurrentGainI,
    eControlEnablePosition,
    eControlEnableSpeed,
    eControlEnableCurrent
};
#endif // DATAGRAMS_H
