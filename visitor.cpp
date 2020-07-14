#include "visitor.h"
#include "adapter.h"
#include "state.h"

void Visitor::handle(StateNode & node)
{
    Event ev;
    ev.type = "event";
    ev.data = 0;
    node.onEvent(ev);
}

void Visitor::handle(MapAdapterNode & node)
{
    node.removeByKey("event");
}