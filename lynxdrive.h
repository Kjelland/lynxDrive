#ifndef LYNXDRIVE_H
#define LYNXDRIVE_H
#include "driveDatagram.h"

class LynxDrive
{
public:

    LynxDrive();

    uint16_t controlWord=0;

    uint16_t statusWord=0;

    void operation(void);

    bool getControlBit(DriveControlWord controlBit);

    void setControlBit(DriveControlWord controlBit,bool value);

    bool getStatusBit(DriveStatusWord statusBit);

    void setStatusBit(DriveStatusWord statusBit,bool value);


private:


};

#endif // LYNXDRIVE_H
