#include "lynxdrive.h"
#include <QDebug>
LynxDrive::LynxDrive()
{

}

void LynxDrive::operation(void)
{
    switch (state) {
    case eDriveOff:
        if(getBit(&controlWord,eAckFaults))
            setBit(&statusWord,eDriveFault,false);

        if(getBit(&controlWord,eEnableDrive) && !getBit(&controlWord,eONOFF1) && !getBit(&controlWord,eOFF2))
        {
            state=eReady;
            setBit(&statusWord,eEnabled,true);
            qDebug()<<"Going to enable";
        }
        break;
    case eReady:
        if(!getBit(&controlWord,eEnableDrive)| getBit(&controlWord,eOFF2))
        {
            state=eDriveOff;
            setBit(&statusWord,eEnabled,false);
            qDebug()<<"Going to off";
        }
        if(getBit(&controlWord,eONOFF1))
        {
            state=eOperation;
            setBit(&statusWord,eRunning,true);
            qDebug()<<"Going to run";
        }
        if(getBit(&controlWord,eIdentify))
        {
            qDebug()<<"Going to identify";
            state=eIdentification;
            setBit(&statusWord,eIdenfifying,true);
        }
        break;
    case eOperation:
        speedFeedback=speedSetpoint;
        if(!getBit(&controlWord,eONOFF1))
        {
            state=eRampStop;
            qDebug()<<"Going to ramp stop";
            setBit(&statusWord,eRunning,false);
        }
        if(getBit(&controlWord,eOFF2))
        {
            qDebug()<<"Going to coast down";
            state=eCoastDown;
            setBit(&statusWord,eRunning,false);
            setBit(&statusWord,eCoastDownEnabled,true);
        }

        break;

    case eRampStop:
        //If abs(speed)<0.01
        state=eReady;
        break;

    case eCoastDown:
        //If abs(speed)<0.01
        state=eDriveOff;
        setBit(&statusWord,eCoastDownEnabled,false);
        setBit(&statusWord,eEnabled,false);
        qDebug()<<"Going to off";
        break;

    case eFault:
        state=eCoastDown;
        break;

    case eIdentification:
        //if(identified)
        setBit(&statusWord,eIdenfifying,false);
        setBit(&statusWord,eMotorIdentified,true);
        state=eReady;
        break;
    }//end case
    setBit(&statusWord,eSpeedModeEnabled,getBit(&controlWord,eSpeedMode));
}

void LynxDrive::setBit(uint16_t *word,uint16_t bitMask,bool value)
{
    if(value)
        *word |= bitMask;
    else
        *word &= ~bitMask;
}

bool LynxDrive::getBit(uint16_t *word,uint16_t bitMask)
{
    return (*word & bitMask)!=0;
}
