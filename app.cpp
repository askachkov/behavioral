#include "app.h"
#include <iostream>
#include "config.h"

using namespace std;

class ACommand: public ICommand
{
public:
    bool saveForHistory()const
    {
        return true;
    }
};

class GroupCmd: public ACommand
{
    std::vector< CommandPtr > m_Commands;
public:
    GroupCmd()
    {
    }
    void trigger() override
    {
        for ( auto cmd: m_Commands ){
            cmd->trigger();
        }
    }
    void add( const CommandPtr & cmd )
    {
        m_Commands.push_back(cmd);
    }
};

class RequestCmd: public ACommand
{
public:
    RequestCmd(App * a, CommandLoop * loop):
        m_App(a),
        m_Loop(loop)
    {
    }
    void trigger() override
    {
        int var = 0;
        cout << "Select option[" << Config::instance().get("APP_NAME") << "]:" << endl;
        cout << "0. Exit" << endl;
        cout << "1. Draw" << endl;
        cout << "2. Event 50.1" << endl;
        cout << "3. Event 49.3" << endl;
        cout << "4. Visitor" << endl;
        cout << "9. Redo last" << endl;
        cin >> var;
        switch ( var ){
            case 1: {
                GroupCmd * group = new GroupCmd();
                group->add(m_App->getDrawCmd());
                group->add(m_App->getRequestCmd());
                m_Loop->push( CommandPtr(group) );
            } break;
            case 0: {
            } break;
            case 2: {
                GroupCmd * group = new GroupCmd();
                group->add(m_App->getEvent1Cmd());
                group->add(m_App->getDrawCmd());
                group->add(m_App->getRequestCmd());
                m_Loop->push( CommandPtr(group) );
            } break;
            case 3: {
                GroupCmd * group = new GroupCmd();
                group->add(m_App->getEvent2Cmd());
                group->add(m_App->getDrawCmd());
                group->add(m_App->getRequestCmd());
                m_Loop->push( CommandPtr(group) );
            } break;
            case 4: {
                GroupCmd * group = new GroupCmd();
                group->add(m_App->getVisitorCmd());
                group->add(m_App->getDrawCmd());
                group->add(m_App->getRequestCmd());
                m_Loop->push( CommandPtr(group) );
            } break;
            case 9: {
                m_Loop->push( m_App->getRedoLastCmd() );
            } break;
        }
    }

private:
    CommandLoop * m_Loop;
    App * m_App;
};

class RedoLastCmd: public ICommand
{
    CommandPtr m_P;
public:
    RedoLastCmd(CommandPtr p):
        m_P(p)
    {
    }
    void trigger() override
    {
        if ( m_P ){
            m_P->trigger();
        }
    }
    bool saveForHistory()const
    {
        return false;
    }

private:
    CommandLoop * m_Loop;
};

class DrawCmd: public ACommand
{
public:
    DrawCmd(NodePtr root, IDrawer * d):
        m_Root(root),
        m_Drawer(d)
    {
    }
    void trigger() override
    {
        m_Root->draw(*m_Drawer);
        m_Drawer->draw("\n\n");
    }

private:
    NodePtr m_Root;
    IDrawer * m_Drawer;
};

class EventCmd: public ACommand
{
public:
    EventCmd(NodePtr root, double value):
        m_Root(root),
        m_Value(value)
    {
    }
    void trigger() override
    {
        Event ev;
        ev.type = "event";
        ev.data = m_Value;
        m_Root->onEvent(ev);
    }

private:
    NodePtr m_Root;
    double  m_Value;
};

class VisitorCmd: public ACommand
{
public:
    VisitorCmd(NodePtr root):
        m_Root(root)
    {
    }
    void trigger() override
    {
        Visitor v;
        m_Root->acceptVisitor(v);
    }

private:
    NodePtr m_Root;
};

App::App():
    m_pFactory(nullptr),
    m_pDrawer(nullptr)
{
    m_Loop.push( getRequestCmd() );
}

void App::setFactory(INodeFactory * f)
{
    m_pFactory = f;
}

void App::setDrawer(IDrawer * d)
{
    m_pDrawer = d;
}


void App::prepare()
{
    std::shared_ptr<Node> arr = m_pFactory->createGroup();
	arr->add( m_pFactory->createNum(3) );
	arr->add( m_pFactory->createNum(4) );
	arr->add( m_pFactory->createNum(6) );

	std::map<std::string, double> mapObject;
	mapObject["height"] = 4.1;
	mapObject["width"] = 2.4;
	mapObject["depth"] = 2.2;

	std::shared_ptr<Node> obj = m_pFactory->createGroup();
    obj->add(m_pFactory->createPair("disp", m_pFactory->createState()) );
	obj->add(m_pFactory->createPair("params", NodePtr(new MapAdapterNode(mapObject))) );
	obj->add(m_pFactory->createPair("key1", m_pFactory->createString("big data")));
	obj->add(m_pFactory->createPair("key2", m_pFactory->createString("big data")));
	obj->add(m_pFactory->createPair("key3", m_pFactory->createString("big data 2")));
	obj->add(m_pFactory->createPair("nums", m_pFactory->createArr(arr)));

	obj->add(m_pFactory->createPair("proxy", NodePtr( new ProxyNode<NumNode, int, 778>() ) ));

	for ( auto it = obj->begin(); it != obj->end(); ++it ){
		it->draw(*m_pDrawer);
		m_pDrawer->draw("\n");
	}
    m_pDrawer->draw("\n");

    m_Root = m_pFactory->createObj(obj);
}

CommandPtr App::getRequestCmd()
{
    return CommandPtr(new RequestCmd(this, &m_Loop));
}

CommandPtr App::getDrawCmd()
{
    return CommandPtr(new DrawCmd(m_Root, m_pDrawer));
}

CommandPtr App::getEvent1Cmd()
{
    return CommandPtr(new EventCmd(m_Root, 50.1));
}

CommandPtr App::getEvent2Cmd()
{
    return CommandPtr(new EventCmd(m_Root, 49.3));
}

CommandPtr App::getRedoLastCmd()
{
    return CommandPtr( new RedoLastCmd( m_Loop.last() ) );
}

CommandPtr App::getVisitorCmd()
{
    return CommandPtr( new VisitorCmd( m_Root ) );
}

int App::exec()
{
    m_Loop.exec();
    return 0;
}