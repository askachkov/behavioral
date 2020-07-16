#pragma once

#include <string>
#include "events.h"

class IMediator
{
public:
    virtual ~IMediator(){}
    virtual void send(const Event & ev) = 0;
    virtual void registerMediatorListener(IEventReciever * ) = 0;
};
