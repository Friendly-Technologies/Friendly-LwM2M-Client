#include "WppGuard.h"

#include <mutex>

wpp::WppGuard::WppGuard() {
    _guard = new std::mutex();
}

wpp::WppGuard::~WppGuard() {
    delete (std::mutex *)_guard;
}

void wpp::WppGuard::lock() {
    ((std::mutex *)_guard)->lock();
}

void wpp::WppGuard::unlock() {
    ((std::mutex *)_guard)->unlock();
}

bool wpp::WppGuard::try_lock() {
    return ((std::mutex *)_guard)->try_lock();
}