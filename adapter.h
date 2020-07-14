#pragma once

#include "node.h"
#include <map>

class MapAdapterNode:
    public ANode
{
    std::map<std::string, double> m_Object;
    bool m_RetSelf;
public:
    MapAdapterNode( const std::map<std::string, double> & d ):
        m_Object(d)
    {
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
    virtual void acceptVisitor(Visitor & v) override
    {
        v.handle(*this);
    }
};