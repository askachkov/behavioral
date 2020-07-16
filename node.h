#pragma once

#include <vector>
#include <memory>
#include "drawer.h"
#include "iterable.h"
#include "events.h"
#include "visitor.h"
#include "clonable.h"

class INode:
	public Iterable<INode>,
	public IEventReciever,
	public IVisitorReciever,
	public IClonable<INode>
{
public:
	virtual ~INode(){}
	virtual void draw(IDrawer & ) = 0;
};

class ANode:
	public INode
{
public:
	virtual ~ANode(){}
	virtual INode* next() override
	{
		return nullptr;
	}
	virtual void resetIteratorCounter() override
	{
	}
	virtual void onEvent(const Event & e) override
    {
    }
	virtual void acceptVisitor(Visitor & v) override
	{
	}
};

typedef std::shared_ptr<INode> NodePtr;
typedef std::vector<NodePtr> NodeList;

class Node:
	public ANode
{
public:
	Node();
	void add(const NodePtr & node);
	virtual void draw(IDrawer & ) override;
	iterator begin();
	iterator end();
	virtual INode* next() override;
	virtual void resetIteratorCounter() override;
	virtual void onEvent(const Event & e) override;
	virtual void acceptVisitor(Visitor & v) override;
	virtual std::shared_ptr<INode> clone() override;

private:
	NodeList m_Children;
	size_t	 m_Id;
};

template<char open, char close>
class WrapperNode:
	public ANode
{
	NodePtr m_Node;
public:
	WrapperNode(const NodePtr & node):
		m_Node(node)
	{
	}
	void draw(IDrawer & d) override
	{
		d.draw(open);
		m_Node->draw(d);
		d.draw(close);
	}
	virtual INode* next() override
	{
		return m_Node->next();
	}
	virtual void resetIteratorCounter() override
	{
		m_Node->resetIteratorCounter();
	}
	virtual void onEvent(const Event & e) override
	{
		m_Node->onEvent(e);
	}
	virtual void acceptVisitor(Visitor & v) override
	{
		m_Node->acceptVisitor(v);
	}
	virtual std::shared_ptr<INode> clone() override
	{
		return std::shared_ptr<INode>(new WrapperNode<open, close>( m_Node->clone() ));
	}
};

template<typename Value>
class ValueNode:
	public ANode
{
	Value m_Value;
public:
	ValueNode(const Value & v):
		m_Value(v)
	{
	}
	void draw(IDrawer & d) override
	{
		d.draw(m_Value);
	}
	virtual std::shared_ptr<INode> clone() override
	{
		return std::shared_ptr<INode>(new ValueNode<Value>(m_Value));
	}
};

typedef WrapperNode<'{', '}'> ObjectNode;
typedef WrapperNode<'[', ']'> ArrayNode;
typedef WrapperNode<'"', '"'> StringNode;
typedef ValueNode<double> NumNode;
typedef std::shared_ptr<StringNode> StringPtr;

class KeyValueNode:
	public ANode
{
	StringPtr m_Key;
	NodePtr m_Value;
public:
	KeyValueNode(const StringPtr & key, const NodePtr & value);
	void draw(IDrawer & d) override;
	iterator begin();
	iterator end();
	virtual INode* next() override;
	virtual void resetIteratorCounter() override;
	virtual void onEvent(const Event & e) override;
	virtual void acceptVisitor(Visitor & v) override;
	virtual std::shared_ptr<INode> clone() override;

private:
	enum RetState { Child, End };
	RetState	 m_RetState;
};

template<typename Node, typename ValueType, ValueType value>
class ProxyNode: public ANode
{
	NodePtr m_Data;
	NodePtr get()
	{
		if ( !m_Data ){
			//d.draw("\nProxy do it job!\n");
			m_Data = NodePtr(new Node(value));
		}
		return m_Data;
	}
public:
	void draw(IDrawer & d) override
	{
		get()->draw(d);
	}
	virtual INode* next() override
	{
		return get()->next();
	}
	virtual void resetIteratorCounter() override
	{
		get()->resetIteratorCounter();
	}
	virtual void onEvent(const Event & e) override
	{
		get()->onEvent(e);
	}
	virtual void acceptVisitor(Visitor & v) override
	{
		get()->acceptVisitor(v);
	}
	virtual std::shared_ptr<INode> clone() override
	{
		ProxyNode<Node, ValueType, value> * proxy = new ProxyNode<Node, ValueType, value>();
		if ( m_Data ){
			proxy->m_Data = m_Data->clone();
		}
		return std::shared_ptr<INode>(proxy);
	}
};
