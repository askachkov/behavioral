#pragma once

#include "node.h"

enum StateType
{
    State_Idle,
    State_Init,
    State_Low,
    State_High
};

class IState
{
public:
    virtual ~IState(){}
    virtual StateType prepare() = 0;
    virtual void draw(IDrawer & d) = 0;
    virtual void onEvent(const Event & e) = 0;
};

class StateNode:
	public ANode
{
    typedef std::shared_ptr<IState> StatePtr;
	StateType m_CurrentState;
    StatePtr m_States[4];
public:
    StateNode();
	void draw(IDrawer & d) override;
    void onEvent(const Event & e) override;
    virtual void acceptVisitor(Visitor & v) override;
};