#pragma once

#include <string>

class IEventReciever;

struct Event
{
    std::string type;
    double data;
    IEventReciever * sender;
};

class IEventReciever
{
public:
    virtual ~IEventReciever(){}
    virtual void onEvent(const Event & e) = 0;
};