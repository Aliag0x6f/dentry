#include "EventBus.h"

namespace dentry::app::events {

EventBus* EventBus::instance() {
    static EventBus bus;
    return &bus;
}

} // namespace dentry::app::events
