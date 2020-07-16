#pragma once

#include "node.h"
#include "mediator.h"
#include <map>

class MapAdapterNode:
    public ANode
{
    std::map<std::string, double> m_Object;
    IMediator * m_Mediator;
public:
    MapAdapterNode( const std::map<std::string, double> & d, IMediator * mediator ):
        m_Object(d),
        m_Mediator(mediator)
    {
        if ( m_Mediator != nullptr ){
            m_Mediator->registerMediatorListener(this);
        }
    }
    void draw(IDrawer & d) override
    {
        bool isFirst = true;
        d.draw('{');
        for ( auto & i : m_Object ){
            if ( !isFirst ){
                d.draw(", ");
            } else {
                isFirst = false;
            }
            d.draw('"');
            d.draw(i.first);
            d.draw("\": ");
            d.draw(i.second);
        }
        d.draw('}');
    }
    virtual void onEvent(const Event & e) override
    {
        m_Object[e.type] = e.data;
    }
    void removeByKey(const std::string & key)
    {
        auto it = m_Object.find(key);
        if ( it != m_Object.end() ){
            m_Object.erase(it);
        }
    }
    void notifyAboutEvent()
    {
        if ( m_Mediator != nullptr ){
            Event ev = { "mediatorEv", 33.7, this };
            //should not be recieved by this item
            m_Mediator->send(ev);
        }
    }
    virtual void acceptVisitor(Visitor & v) override
    {
        v.handle(*this);
    }
    virtual std::shared_ptr<INode> clone() override
	{
        //do not subscribe it again
		return std::shared_ptr<INode>(new MapAdapterNode(m_Object, nullptr));
	}
};