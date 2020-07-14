#include "state.h"
#include <chrono>

class AState: public IState
{
    typedef std::chrono::steady_clock::time_point TimePoint;
    TimePoint m_Time;
public:
    AState():
        m_Value(0)
    {
    }
    void onEvent(const Event & e)
    {
        m_Time = std::chrono::steady_clock::now();
        m_Value = e.data;
    }

protected:
    double m_Value;
    size_t getTimeDiff()const
    {
        TimePoint x = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(x - m_Time).count();
    }
};

class IdleState: public AState
{
public:
    StateType prepare()
    {
        if ( getTimeDiff() > 5 ){
            m_Value = 0;
        }
        if ( m_Value > 1 ){
            return State_Init;
        }
        return State_Idle;
    }
    void draw(IDrawer & d)
    {
        d.draw("{\"state\":\"idle\"");
    }
};

class InitState: public AState
{
public:
    StateType prepare()
    {
        if ( getTimeDiff() > 5 ){
            m_Value = 0;
            return State_Idle;
        }
        if ( m_Value > 40 && getTimeDiff() > 1 ){
            return State_Low;
        }
        return State_Init;
    }
    void draw(IDrawer & d)
    {
        d.draw("{\"state\":\"init\"");
    }
};

class LowState: public AState
{
public:
    StateType prepare()
    {
        if ( getTimeDiff() > 5 ){
            m_Value = 0;
            return State_Init;
        }
        if ( m_Value > 50 && getTimeDiff() > 1 ){
            return State_High;
        }
        return State_Low;
    }
    void draw(IDrawer & d)
    {
        d.draw("{\"state\":\"low\"");
    }
};

class HighState: public AState
{
public:
    StateType prepare()
    {
        if ( getTimeDiff() > 5 )
            return State_Low;
        return State_High;
    }
    void draw(IDrawer & d)
    {
        d.draw("{\"state\":\"high\"");
    }
};

StateNode::StateNode()
{
    m_CurrentState = State_Idle;
    m_States[State_Idle] = StatePtr(new IdleState);
    m_States[State_Init] = StatePtr(new InitState);
    m_States[State_Low] = StatePtr(new LowState);
    m_States[State_High] = StatePtr(new HighState);
}

void StateNode::draw(IDrawer & d)
{
    m_CurrentState = m_States[m_CurrentState]->prepare();
    m_States[m_CurrentState]->draw(d);
}

void StateNode::onEvent(const Event & e)
{
    m_States[m_CurrentState]->onEvent(e);
    m_States[State_Idle]->onEvent(e);
    m_States[State_Init]->onEvent(e);
    m_States[State_Low]->onEvent(e);
    m_States[State_High]->onEvent(e);
}

void StateNode::acceptVisitor(Visitor & v)
{
    v.handle(*this);
}