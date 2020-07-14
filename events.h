#pragma once

#include <string>

struct Event
{
    std::string type;
    double data;
};

class IEventReciever
{
public:
    virtual ~IEventReciever(){}
    virtual void onEvent(const Event & e) = 0;
};