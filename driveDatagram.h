#ifndef DRIVEDATAGRAMS_H
#define DRIVEDATAGRAMS_H
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



//! \brief Enumerator for drive datagram for controlling the motor real time (minimal packet)
//!
enum DriveDatagram{
    eDriveControl,   //!< 16-bit int with drive bits
    eDriveTorque,    //!< Torque in Nm Q24
    eDriveSpeed,     //!< Speed in krpm Q24
    eDrivePosition,  //!< Position in revolutions Q16. 16bit singleturn - 16bit multiturn
};

static const LynxStructure::StructDefinition driveOperation[] =
{
    { eDriveControl, eUint16 },
    { eDriveTorque, eUint32 },
    { eDriveSpeed, eUint32 },
    { eDrivePosition, eUint32 },
    { 0, eEndOfList }
};

//! \brief Drive datagram for sending/recieving the motor and drive parameters
//!
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
    eMotorPwmFrequency,
    eInverterMaxCurrent,
    eInverterMaxVoltage,
    eEncoderResolution,
    eEncoderOffset
};

static const LynxStructure::StructDefinition driveParams[] =
{
    { eMotorPoles, eFloat},
    { eMotorResistance, eFloat },
    { eMotorDInductance, eFloat },
    { eMotorQInductance, eFloat },
    { eMotorBackEmf, eFloat },
    { eMotorMaxAcceleration, eFloat },
    { eMotorMaxSpeed, eFloat },
    { eMotorMaxCurrent, eFloat },
    { eMotorMaxCurrentOverload, eFloat },
    { eMotorMaxVoltage, eFloat },
    { eMotorPwmFrequency, eFloat },
    { eInverterMaxCurrent, eFloat },
    { eInverterMaxVoltage, eFloat },
    { eEncoderResolution, eFloat },
    { eEncoderOffset, eFloat },
    { eEndOfList, eEndOfList }
};

//! \brief Drive datagram for sending/recieving the controller parameters
//!
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

static const LynxStructure::StructDefinition driveControlParams[] =
{
    { eControlPositionGainP, eIQ},
    { eControlPositionGainI, eIQ},
    { eControlPositionGainD, eIQ},
    { eControlSpeedGainP, eIQ},
    { eControlSpeedyGainI, eIQ},
    { eControlCurrentGainP, eIQ},
    { eControlCurrentGainI, eIQ},
    { eControlEnablePosition, eUint8},
    { eControlEnableSpeed, eUint8},
    { eControlEnableCurrent, eUint8},
    { eEndOfList, eEndOfList }
};
//! \brief Enumerator for drive control word
//!
enum DriveControlWord{
    eONOFF1             = (1<<0),   //!< Starts the motor with defined ramps
    eOFF2               = (1<<1),   //!< Coast stop the motor (zero torque)
    eOFF3               = (1<<2),   //!< Stops the motor instantly (emg stop)
    eEnableDrive        = (1<<3),   //!< Enable drive bit. Needed to start/idenfity
    eSpeedMode          = (1<<4),   //!< Enable speed mode
    eEnableTorqueLimit  = (1<<5),   //!< Enable the torque limiter in speed mode. Torque ref.
    eAckFaults          = (1<<6),   //!< Acknowledge faults
    eIdentify           = (1<<7),   //!< Enable identification of motor and drive parameters
    eUserParams         = (1<<8),   //!< Use predefined motor parameters
    eUserOffset         = (1<<9),   //!< Use predefined offset values instead of measuring
    eSensored           = (1<<10),  //!< Use encoder for sensored configuration
    eFieldWeakening     = (1<<11),  //!< Enable field weakening for higher speeds
    eWriteToFlash       = (1<<12),  //!< Save data in flash
    eReadFromFlash      = (1<<13),  //!< Read data from flash
    eFreeControl1       = (1<<14),  //!< Spare control word
    eFreeControl2       = (1<<15),  //!< Spare control word
};

//! \brief Enumerator for drive status word
//!
enum DriveStatusWord{
    eRunning             = (1<<0),   //!< Motor running
    eCoastDownEnabled    = (1<<1),   //!< OFF2 enabled. Coast down to stop
    eQuickStopEnabled    = (1<<2),   //!< OFF3 enabled. EMG stop
    eEnabled             = (1<<3),   //!< Drive is enabled
    eIdenfifying         = (1<<4),   //!< Idenfifying has been started
    eMotorIdentified     = (1<<5),   //!< Motor is identified
    eUsingUserParams     = (1<<6),   //!< Using the stored user parameters
    eUsingOffset         = (1<<7),   //!< Using the stored offset values
    eUsingSensored       = (1<<8),   //!< Using encoder for sensored configuration
    eUsingFieldWeak      = (1<<9),   //!< Using field weakening
    eSavedFlashSuccess   = (1<<10),  //!< Saving to flash was successfull
    eReadFlashSuccess    = (1<<11),  //!< Reading from flash successfull, e.g. new data available
    eFlashError          = (1<<12),  //!< Flash read/write error
    eUserParamError      = (1<<13),  //!< Spare status word
    eDriveFault          = (1<<14),  //!< Spare status word
    eEncoderFault        = (1<<15),  //!< Spare status word

};
#endif // DATAGRAMS_H
