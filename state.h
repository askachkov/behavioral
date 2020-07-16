#pragma once

#include "node.h"
#include "mediator.h"

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
    IEventReciever * m_Sender;
    IMediator * m_Mediator;
public:
    StateNode(IMediator * mediator);
	void draw(IDrawer & d) override;
    void onEvent(const Event & e) override;
    virtual void acceptVisitor(Visitor & v) override;
    void resetSender();
    virtual std::shared_ptr<INode> clone() override;
};