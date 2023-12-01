#include "WppGuard.h"

#include <mutex>

WppGuard::WppGuard() {
    _guard = new std::mutex();
}

WppGuard::~WppGuard() {
    delete (std::mutex *)_guard;
}

void WppGuard::lock() {
    ((std::mutex *)_guard)->lock();
}

void WppGuard::unlock() {
    ((std::mutex *)_guard)->unlock();
}

bool WppGuard::try_lock() {
    return ((std::mutex *)_guard)->try_lock();
}