#pragma once

#include <memory>

template<class Derived>
class IClonable
{
public:
    virtual ~IClonable(){}
    virtual std::shared_ptr<Derived> clone() = 0;
};