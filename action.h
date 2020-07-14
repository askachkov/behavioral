#pragma once

#include <queue>
#include <stack>
#include <memory>

class ICommand
{
public:
    virtual ~ICommand(){}
    virtual void trigger() = 0;
    virtual bool saveForHistory() const = 0;
};

typedef std::shared_ptr<ICommand> CommandPtr;

class CommandLoop
{
public:
    void push(const CommandPtr & cmd);
    CommandPtr last();
    void exec();

private:
    std::queue< CommandPtr > m_Commands;
    std::stack< CommandPtr > m_Hist;
};