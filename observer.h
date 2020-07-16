#pragma once

#include "events.h"

class IObserverSubject
{
public:
    virtual ~IObserverSubject(){}
    virtual void registerListener(IEventReciever * listener) = 0;
    virtual void notify(const Event & ev) = 0;
};