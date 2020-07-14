#pragma once

class StateNode;
class MapAdapterNode;

class Visitor
{
public:
    void handle(StateNode & node);
    void handle(MapAdapterNode & node);
};

class IVisitorReciever
{
public:
    virtual ~IVisitorReciever(){}
    virtual void acceptVisitor(Visitor & v) = 0;
};