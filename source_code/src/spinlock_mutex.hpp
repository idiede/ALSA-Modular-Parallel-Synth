/*
 * spin_lock.hpp
 *
 *  Created on: Sep 7, 2014
 *      Author: ede
 */

#ifndef SPIN_LOCK_HPP_
#define SPIN_LOCK_HPP_

#include <atomic>


//Implementation of a spin-lock mutex using std::atomic_flag not really used


class spinlock_mutex
{
    std::atomic_flag flag;

public:

    spinlock_mutex():
        flag(ATOMIC_FLAG_INIT) {}

    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};


#endif /* SPIN_LOCK_HPP_ */
