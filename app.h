#pragma once

#include "factory.h"
#include "drawer.h"
#include "factory.h"
#include "adapter.h"
#include "action.h"
#include "mediator.h"
#include "observer.h"

class App:
    public IMediator,
    public IObserver
{
public:
    App();
    void setFactory(INodeFactory * f);
    void setDrawer(IDrawer * d);
    void prepare();
    int exec();

    CommandPtr getDrawCmd();
    CommandPtr getRequestCmd();
    CommandPtr getEvent1Cmd();
    CommandPtr getEvent2Cmd();
    CommandPtr getRedoLastCmd();
    CommandPtr getVisitorCmd();
    CommandPtr getObserverCmd();
    CommandPtr getMediatorCmd();

    //IMediator
    virtual void send(const Event & ev) override;
    virtual void registerMediatorListener(IEventReciever * ) override;

    //IObserver
    virtual void notify(const Event & ev) override;
    virtual void registerListener(IEventReciever * listener);

private:
    INodeFactory    * m_pFactory;
    IDrawer         * m_pDrawer;
    std::shared_ptr<INode> m_Root;
    CommandLoop m_Loop;
    CommandPtr        m_MediatorCmd;
    std::vector<IEventReciever*> m_MediatorClients;
    std::vector<IEventReciever*> m_ObserverListeners;
};