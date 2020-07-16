#pragma once

#include "events.h"

class IObserver
{
public:
    virtual ~IObserver(){}
    virtual void registerListener(IEventReciever * listener) = 0;
    virtual void notify(const Event & ev) = 0;
};