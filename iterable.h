#pragma once

template<typename Type>
class Iterable
{
public:
    virtual ~Iterable(){}
    class iterator
    {
    public:
        typedef Type value_type;
        iterator(Iterable<value_type> * target):
            m_Target(target),
            m_Value(nullptr)
        {
            if ( m_Target != nullptr ){
                m_Target->resetIteratorCounter();
                m_Value = m_Target->next();
            }
        }
        value_type* operator*() {
            return m_Value;
        }
        value_type* operator->() {
            return m_Value;
        }
        iterator& operator++(){
            m_Value = m_Target->next();
            return *this;
        }
        operator bool()const
        {
            return m_Value != nullptr;
        }
        bool operator != (iterator & it)
        {
            return m_Value != it->m_Value;
        }
    private:
        value_type           * m_Value;
        Iterable<value_type> * m_Target;
    };
    virtual Type * next() = 0;
    virtual void resetIteratorCounter() = 0;
};