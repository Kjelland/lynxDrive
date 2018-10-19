#ifndef CONTROLLERSTRUCT_H
#define CONTROLLERSTRUCT_H
#include "LynxStructure.h"

using namespace LynxStructureSpace;

enum ControllerContents
{
    joy_LX,
    joy_LY,
    joy_RX,
    joy_RY,
    trigger_L,
    trigger_R,
    buttons,
    endOfList
};

LynxStructureSpace::LynxStructure::InitParam initParamsController[8]
    {
        { joy_LX, eInt16 },
        { joy_LY, eInt16 },
        { joy_RX, eInt16 },
        { joy_RY, eInt16 },
        { trigger_L, eInt8 },
        { trigger_R, eInt8 },
        { buttons, eUint32 },
        { endOfList, eEndOfList }
    };
#endif // CONTROLLERSTRUCT_H

