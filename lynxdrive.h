#ifndef LYNXDRIVE_H
#define LYNXDRIVE_H
#include "driveDatagram.h"
#define Q24 16777216.0
#define Q16 65536.0
class LynxDrive
{
public:

    LynxDrive();

    DriveState state=eDriveOff;

    uint16_t controlWord=0;

    uint16_t parameterControlWord=0;

    double speedSetpoint=0;

    double torqueSetpoint=0;

    uint16_t statusWord=0;

    double speedFeedback=0;

    double torqueFeedback=0;

    double positionFeedback=0;

    void operation(void);

    void setBit(uint16_t *word,uint16_t bitMask,bool value);

    bool getBit(uint16_t *word,uint16_t bitMask);

private:


};

#endif // LYNXDRIVE_H
