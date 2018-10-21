#include "lynxdrive.h"

LynxDrive::LynxDrive()
{

}

void LynxDrive::operation(void)
{
    if(getControlBit(eEnableDrive))
    {
        setStatusBit(eEnabled,true);
        if(getControlBit(eONOFF1))
        {
            //Start motor
            setStatusBit(eRunning,true);
        }
        else
        {
            //Stop the motor with predef ramp
            setStatusBit(eRunning,false);
        }

        if(getControlBit(eOFF2))
        {
            //Set zero torque, then switch off
            setStatusBit(eCoastDownEnabled,true);
        }
        if(getControlBit(eOFF3))
        {
            //Set zero torque, then switch off
            setStatusBit(eQuickStopEnabled,true);
        }

    }
    else
    {
        setStatusBit(eEnabled,false);
    }
}

bool LynxDrive::getControlBit(DriveControlWord controlBit)
{

    return (controlWord & controlBit)!=0;
}

void LynxDrive::setControlBit(DriveControlWord controlBit,bool value)
{
    if(value)
        controlWord |= controlBit;
    else
        controlWord &= ~controlBit;
}

bool LynxDrive::getStatusBit(DriveStatusWord statusBit)
{
    return (statusWord & statusBit)!=0;
}

void LynxDrive::setStatusBit(DriveStatusWord statusBit,bool value)
{
    if(value)
        statusWord |= statusBit;
    else
        statusWord &= ~statusBit;
}
