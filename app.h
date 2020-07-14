#pragma once

#include "factory.h"
#include "drawer.h"
#include "factory.h"
#include "adapter.h"
#include "action.h"

class App
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

private:
    INodeFactory    * m_pFactory;
    IDrawer         * m_pDrawer;
    std::shared_ptr<INode> m_Root;
    CommandLoop m_Loop;
};