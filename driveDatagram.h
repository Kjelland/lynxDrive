#ifndef DRIVEDATAGRAMS_H
#define DRIVEDATAGRAMS_H
#include "LynxStructure.h"

using namespace LynxStructureSpace;
// **************************************************************************
// the includes


// **************************************************************************
// the defines
#define OPERATION_STRUCT 0x22
#define DRIVE_PARAM_STRUCT 0x23
#define CONTROL_PARAM_STRUCT 0x24
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
    eDriveWord,   //!< 16-bit int with drive bits
    eDriveTorque,    //!< Torque in Nm Q24
    eDriveSpeed,     //!< Speed in krpm Q24
    eDrivePosition,  //!< Position in revolutions Q16. 16bit singleturn - 16bit multiturn
};

static const LynxStructure::StructDefinition driveOperation[] =
{
    { eDriveWord, eUint16 },
    { eDriveTorque, eInt32 },
    { eDriveSpeed, eInt32 },
    { eDrivePosition, eInt32 },
    { 0, eEndOfList }
};

//! \brief Drive datagram for sending/recieving the motor and drive parameters
//!
enum MotorDatagram{
    eMotorParameterControl,
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
    { eMotorParameterControl, eUint16},
    { eMotorPoles, eFloat},
    { eMotorResistance, eFloat },
    { eMotorDInductance, eFloat },
    { eMotorQInductance, eFloat },
    { eMotorBackEmf, eFloat },
    { eMotorMaxAcceleration, eInt32 },
    { eMotorMaxSpeed, eFloat },
    { eMotorMaxCurrent, eFloat },
    { eMotorMaxCurrentOverload, eFloat },
    { eMotorMaxVoltage, eFloat },
    { eMotorPwmFrequency, eFloat },
    { eInverterMaxCurrent, eFloat },
    { eInverterMaxVoltage, eFloat },
    { eEncoderResolution, eUint16 },
    { eEncoderOffset, eUint32 },
    { eEndOfList, eEndOfList }
};

//! \brief Drive datagram for sending/recieving the controller parameters
//!
enum ControlDatagram
{
    eControlParameterControl,
    //eControlPositionGainP,
    //eControlPositionGainI,
    //eControlPositionGainD,
    eControlSpeedGainP,
    eControlSpeedGainI,
    eControlCurrentGainP,
    eControlCurrentGainI,
    //eControlEnablePosition,
    //eControlEnableSpeed,
    //eControlEnableCurrent
};

static const LynxStructure::StructDefinition driveControlParams[] =
{
    { eControlParameterControl, eUint16},
    //{ eControlPositionGainP, eIQ},
    //{ eControlPositionGainI, eIQ},
    //{ eControlPositionGainD, eIQ},
    { eControlSpeedGainP, eIQ},
    { eControlSpeedGainI, eIQ},
    { eControlCurrentGainP, eIQ},
    { eControlCurrentGainI, eIQ},
    //{ eControlEnablePosition, eUint8},
    //{ eControlEnableSpeed, eUint8},
    //{ eControlEnableCurrent, eUint8},
    { eEndOfList, eEndOfList }
};
//! \brief Enumerator for parameter control word
//!
enum ParameterControlWord{
    eReadParamters      = (1<<0),   //!< Send back parameter status without writing new
    eWriteParamters     = (1<<1),   //!< Write parameters and send back new status
};
//! \brief Enumerator for drive control word
//!
enum DriveControlWord{
    eONOFF1             = (1U<<0),   //!< Starts the motor with defined ramps
    eOFF2               = (1U<<1),   //!< Coast stop the motor (zero torque)
    eFreeControl3       = (1U<<2),   //!< Spare control bit
    eEnableDrive        = (1U<<3),   //!< Enable drive bit. Needed to start/idenfity
    eSpeedMode          = (1U<<4),   //!< Enable speed mode
    eEnableTorqueLimit  = (1U<<5),   //!< Enable the torque limiter in speed mode. Torque ref.
    eAckFaults          = (1U<<6),   //!< Acknowledge faults
    eIdentify           = (1U<<7),   //!< Enable identification of motor and drive parameters
    eUserParams         = (1U<<8),   //!< Use predefined motor parameters
    eUserOffset         = (1U<<9),   //!< Use predefined offset values instead of measuring
    eSensored           = (1U<<10),  //!< Use encoder for sensored configuration
    eFieldWeakening     = (1U<<11),  //!< Enable field weakening for higher speeds
    eWriteToFlash       = (1U<<12),  //!< Save data in flash
    eReadFromFlash      = (1U<<13),  //!< Read data from flash
    eFreeControl1       = (1U<<14),  //!< Spare control bit
    eFreeControl2       = (1U<<15),  //!< Spare control bit
};

//! \brief Enumerator for drive status word
//!
enum DriveStatusWord{
    eRunning             = (1U<<0),   //!< Motor running
    eCoastDownEnabled    = (1U<<1),   //!< OFF2 enabled. Coast down to stop
    eIdenfifying         = (1U<<2),   //!< Idenfifying has been started
    eEnabled             = (1U<<3),   //!< Drive is enabled
    eSpeedModeEnabled    = (1U<<4),   //!< SpeedMode enabled
    eMotorIdentified     = (1U<<5),   //!< Motor is identified
    eUsingUserParams     = (1U<<6),   //!< Using the stored user parameters
    eUsingOffset         = (1U<<7),   //!< Using the stored offset values
    eUsingSensored       = (1U<<8),   //!< Using encoder for sensored configuration
    eUsingFieldWeak      = (1U<<9),   //!< Using field weakening
    eSavedFlashSuccess   = (1U<<10),  //!< Saving to flash was successfull
    eReadFlashSuccess    = (1U<<11),  //!< Reading from flash successfull, e.g. new data available
    eFlashFault          = (1U<<12),  //!< Flash read/write error
    eUserParamFault      = (1U<<13),  //!< Parameter error
    eDriveFault          = (1U<<14),  //!< Fault
    eEncoderFault        = (1U<<15),  //!< Encoder fault
};

//! \brief Enumerator for drive state
//!
enum DriveState{
    eDriveOff,
    eReady,
    eOperation,
    eRampStop,
    eCoastDown,
    eFault,
    eIdentification
};

#endif // DATAGRAMS_H
