/*
 * threadpool.h
 *
 *  Created on: 2013-07-08
 *      Author: ede
 *      get the URL??
 */

#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_



//#include <boost/lockfree/queue.hpp>
//#include <boost/lockfree/spsc_queue.hpp>


#include <memory>
#include <queue>
#include <thread>
#include <atomic>
#include <iostream>
#include <future>

#include "thread_safe_queue.hpp"
//#include "join_threads.hpp"
#include "port.h"

class thread_pool {

 //boost::lockfree::queue<std::packaged_task<float**(port_ptr) >> lfqueue;//(128);
 thread_safe_queue<std::packaged_task<float**() >> lfqueue;
	// join_threads joiner;
 //spsc_queue<p_module_ptr> sp_queue;

 std::atomic_bool done;

 std::vector<std::thread> threads;
// join_threads joiner;
  unsigned int thread_count;

  void worker_thread()
    {
	  while(!done)
	         {

/*
 *
 *  for  boost::lockfree::queue
 *
 *
         port_ptr m_out;
        	 while (!done) {
        	        while (lfqueue.try_pop(task)){
        	        	task();
        	       this->thread_function(std::move(t_out));
        	        }
         }*/

	        /*
	         * for all others
	         */
	            std::shared_ptr<std::packaged_task<float**() >> task(lfqueue.try_pop());

	                 if(task)
	                     this->thread_function(std::move(*task));


	    }
    }

public:

thread_pool(unsigned int t):

	  //  lfqueue(),
	    done(false),
    //	joiner(threads),
    	 thread_count(t),
      //  ts_queue(),
          sp_queue()

    {
       // unsigned const thread_count=2;
        try
        {
            for(unsigned i=0;i<thread_count;++i)
            {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));

            }
        }
        catch(...)
        {
            done=true;
            throw;
        }
    }

    ~thread_pool()
    {

        done=true;

    }


/*
 *
 * std::future<void> threadpool::run(std::function<void()> && task)
{
    auto promise = std::promise<void>{};
    auto future = promise.get_future();

    // ensures no memory leak if push throws (it shouldn't but to be safe)
    auto package = std::make_unique<task_package>();

    package->completion_promise = std::move(promise);
    package->task = task;

    tasks.push(package.get());

    // no longer in danger, can revoke ownership so
    // tasks is not left with dangling reference
    package.release();

    return future;
};
 *
 */
std::future<float**> run_task(std::function<float**()> in_task)
    {
      /*void task_bind()
{
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();

    task();

    std::cout << "task_bind:\t" << result.get() << '\n';
}*/


        std::packaged_task<float**()> task(std::bind(in_task));//I think this should be a function..
        std::future<float**> res(task.get_future());
        lfqueue.push(std::move(task));

        return res;
    }


void thread_function(std::packaged_task<float**() > task)
	{
		//std::cout<<"task 2 started \n";

		task();//..operator (port_ptr__args);//()(next)//port_ptr args-- we send a port with callable args.. so

		//std::cout<<"task over \n";

	}
};

extern thread_pool *pool;

#endif /* THREAD_POOL_HPP_ */


