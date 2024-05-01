#ifndef WPP_GUARD_H
#define WPP_GUARD_H

namespace wpp {

/**
 * @class WppGuard
 * @brief The WppGuard class provides a linker callback class that must be implemented by the user.
 * 
 * The WppGuard class is responsible for providing synchronization mechanisms for accessing shared resources.
 * It provides methods to lock, unlock, and try_lock the guard object. The user must implement these methods
 * according to their specific synchronization requirements.
 */
class WppGuard {
public:
    /**
     * @brief Constructs a WppGuard object.
     */
    WppGuard();

    /**
     * @brief Destructs the WppGuard object.
     */
    ~WppGuard();

    /**
     * @brief Locks the guard object.
     * 
     * This method is used to acquire the lock on the guard object. If the lock is already acquired by another thread,
     * the calling thread will be blocked until the lock is released.
     */
    void lock();

    /**
     * @brief Unlocks the guard object.
     * 
     * This method is used to release the lock on the guard object. It should be called after the critical section
     * is completed to allow other threads to acquire the lock.
     */
    void unlock();

    /**
     * @brief Tries to lock the guard object.
     * 
     * This method is used to try to acquire the lock on the guard object. If the lock is already acquired by another thread,
     * this method will return false immediately without blocking the calling thread. If the lock is successfully acquired,
     * this method will return true.
     * 
     * @return true if the lock is successfully acquired, false otherwise.
     */
    bool try_lock();

private:
    void *_guard; /**< Pointer to the guard object. */
};

} // namespace wpp

#endif // WPP_GUARD_H
