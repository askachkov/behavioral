#include "node.h"
#include <iostream>

using namespace std;

Node::Node():
    m_Id(0)
{
}

void Node::onEvent(const Event & e)
{
    for ( size_t i = 0; i < m_Children.size(); ++i ){
        m_Children[i]->onEvent(e);
    }
}

void Node::acceptVisitor(Visitor & v)
{
    for ( size_t i = 0; i < m_Children.size(); ++i ){
        m_Children[i]->acceptVisitor(v);
    }
}

void Node::resetIteratorCounter()
{
    m_Id = 0;
    for ( size_t i = 0; i < m_Children.size(); ++i ){
        m_Children[i]->resetIteratorCounter();
    }
}

INode* Node::next()
{
    do {
        if ( m_Id < m_Children.size() ){
            INode * res = m_Children[m_Id]->next();
            if ( res != nullptr ){
                return res;
            }
            //m_Id++;
            return m_Children[m_Id++].get();
        }
    } while( m_Id < m_Children.size() );
    return 0;
}

Node::iterator Node::begin()
{
    cout << "Childrens:" << m_Children.size() << "\n";
    return iterator(this);
}

Node::iterator Node::end()
{
    return iterator(0);
}

void Node::add(const NodePtr & node)
{
    m_Children.push_back(node);
}

void Node::draw(IDrawer & d)
{
    for ( size_t i = 0; i < m_Children.size(); ++i ){
        if ( i > 0 ){
            d.draw(", ");
        }
        m_Children[i]->draw(d);
    }
}

std::shared_ptr<INode> Node::clone()
{
    Node * res = new Node();
    for ( size_t i = 0; i < m_Children.size(); ++i ){
        res->add(m_Children[i]->clone());
    }
    res->m_Id = m_Id;
    return std::shared_ptr<INode>(res);
}

KeyValueNode::KeyValueNode(const StringPtr & key, const NodePtr & value):
    m_Key(key),
    m_Value(value),
    m_RetState(Child)
{
}

void KeyValueNode::draw(IDrawer & d)
{
    m_Key->draw(d);
    d.draw(": ");
    m_Value->draw(d);
}

KeyValueNode::iterator KeyValueNode::begin()
{
    return iterator(this);
}

KeyValueNode::iterator KeyValueNode::end()
{
    return iterator(0);
}

INode* KeyValueNode::next()
{
    switch (m_RetState)
    {
    case Child: {
        INode * res = m_Value->next();
        if ( res != nullptr ){
            return res;
        }
        m_RetState = End;
        return m_Value.get();
    } break;
    default:
        return 0;
        break;
    }
}

void KeyValueNode::resetIteratorCounter()
{
    m_RetState = Child;
    m_Value->resetIteratorCounter();
}

void KeyValueNode::onEvent(const Event & e)
{
    m_Value->onEvent(e);
}

void KeyValueNode::acceptVisitor(Visitor & v)
{
    m_Value->acceptVisitor(v);
}

std::shared_ptr<INode> KeyValueNode::clone()
{
    StringPtr key = dynamic_pointer_cast<StringNode>(m_Key->clone());
    KeyValueNode * res = new KeyValueNode(key, m_Value->clone());
    res->m_RetState = m_RetState;
    return std::shared_ptr<INode>(res);
}