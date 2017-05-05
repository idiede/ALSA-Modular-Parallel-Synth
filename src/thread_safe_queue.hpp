/*
 * threadsafe_queue.hpp
 *
 *  Created on: 2013-09-02
 *      Author: C++ Concurrency in Action
 *      listing 6.4 - 6.8
 */


/*
 * listing_6.5 C++ Concurrency in Action
 *
 *
 * #include <memory>
template<typename T>
class queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;
    node* tail;

public:
    queue():
        head(new node),tail(head.get())
    {}

    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;

    std::shared_ptr<T> try_pop()
    {
        if(head.get()==tail)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> const old_head=std::move(head);
        head=std::move(old_head->next);
        return res;
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        tail->data=new_data;
        node* const new_tail=p.get();
        tail->next=std::move(p);
        tail=new_tail;
    }
};*/
#ifndef THREADSAFE_QUEUE_HPP_
#define THREADSAFE_QUEUE_HPP_
#include <memory>
#include <thread>
#include <mutex>
#include <utility>
#include <iostream>

//#include <condition_variable>
//#define NULL 0

template<typename T>
class thread_safe_queue
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mutex;
	std::unique_ptr<node> head;
	std::mutex tail_mutex;
	node* tail;
//	std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get()==get_tail())
        {
            return nullptr;
        }
        std::unique_ptr<node> old_head=std::move(head);
        head=std::move(old_head->next);
        return old_head;

    }
/*	node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	std::unique_ptr<node> pop_head()
    		{
		std::unique_ptr<node> old_head=std::move(head);
		head=std::move(old_head->next);
		return old_head;
    		}

	std::unique_lock<std::mutex> wait_for_data()
    		{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		data_cond.wait(head_lock,[&]{return head.get()!=get_tail();});
		return std::move(head_lock);
    		}

	std::unique_ptr<node> wait_pop_head()
    		{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		return pop_head();
    		}

	std::unique_ptr<node> wait_pop_head(T& value)
    		{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		value=std::move(*head->data);
		return pop_head();
    		}*/


public:
	thread_safe_queue():
		head(new node),tail(head.get())
{}

	thread_safe_queue(const thread_safe_queue& other)=delete;
	thread_safe_queue& operator=(const thread_safe_queue& other)=delete;


/*	std::shared_ptr<T> wait_and_pop()
    		{
		std::unique_ptr<node> const old_head=wait_pop_head();
		return old_head->data;
    		}

	void wait_and_pop(T& value)
	{
		std::unique_ptr<node> const old_head=wait_pop_head(value);
	}
	 std::shared_ptr<T> try_pop()
	    	    {
		   	std::lock_guard<std::mutex> head_lock(head_mutex);
	    	if(head.get()==get_tail()){
	    		return std::shared_ptr<T>();

	    	} else {
	    		std::unique_ptr<node> old_head=std::move(head);
	    	    head=std::move(old_head->next);
	    		return old_head->data;

	    	}
	    	std::unique_ptr<node> old_head=std::move(head);
	        head=std::move(old_head->next);
	        return old_head;//std::cout << "popool" << std::endl;
	    	  std::unique_ptr<node> old_head=pop_head();

	     if(!old_head)
	    	          {
	    	              return std::shared_ptr<T>();
	    	          }else{
	    	  //std::shared_ptr<T> const res(old_head->data);
	          //delete old_head;
	          return old_head->data; // return old_head?old_head->data:std::shared_ptr<T>();
	    	    }
*/
   std::shared_ptr<T> try_pop()
    	    {
    	//std::cout << "popool" << std::endl;
    	        std::unique_ptr<node> old_head=pop_head();
    	        return old_head?old_head->data:std::shared_ptr<T>();
    	    }


	/*
	 * This while notify one when there is a value in the queue
	 */
/*	void push(T new_value)
	{
		std::shared_ptr<T> new_data(
				std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data=new_data;
			node* const new_tail=p.get();
			tail->next=std::move(p);
			tail=new_tail;
		}
		data_cond.notify_one();
	}*/


	 void push(T new_value)
    {
        std::shared_ptr<T> new_data(
        std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node* const new_tail=p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data=new_data;
        tail->next=std::move(p);
        tail=new_tail;
       // std::cout << "pushpool" << std::endl;
    }

    bool empty()
       {
           std::lock_guard<std::mutex> head_lock(head_mutex);
           return (head.get()==get_tail());
       }

};


#endif /* THREADSAFE_QUEUE_HPP_ */
