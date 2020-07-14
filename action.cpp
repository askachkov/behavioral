#include "action.h"

void CommandLoop::push(const CommandPtr & cmd)
{
    m_Commands.push(cmd);
}

void CommandLoop::exec()
{
    while ( !m_Commands.empty() ){
        auto cmd = m_Commands.front();
        m_Commands.pop();
        if ( cmd->saveForHistory() ){
            m_Hist.push(cmd);
        }
        cmd->trigger();
    }
}

CommandPtr CommandLoop::last()
{
    if ( !m_Hist.empty() ){
        auto cmd = m_Hist.top();
        m_Hist.pop();
        return cmd;
    }
    return CommandPtr();
}