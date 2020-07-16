#include "visitor.h"
#include "adapter.h"
#include "state.h"

void Visitor::handle(StateNode & node)
{
    Event ev;
    ev.type = "event";
    ev.data = 90;
    ev.sender = nullptr;
    node.onEvent(ev);
    node.resetSender();
}

void Visitor::handle(MapAdapterNode & node)
{
    node.removeByKey("event");
    node.removeByKey("observer");
    node.removeByKey("depth");
    node.removeByKey("width");
    node.removeByKey("height");
}