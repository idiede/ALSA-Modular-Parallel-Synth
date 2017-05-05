/*
 * join_threads.hpp
 *
 *  Created on: 2014-06-17
 *      Author: ede
 */

#ifndef JOIN_THREADS_HPP_
#define JOIN_THREADS_HPP_

#include <thread>
#include <iostream>
class join_threads
{
std::vector<std::thread>& threads;

public:

explicit join_threads(std::vector<std::thread>& threads_):

threads(threads_)
{}

~join_threads()
{
	  std::cout << "terminate \n";
	for (std::vector<std::thread>::iterator it = threads.begin() ; it != threads.end(); ++it){
	    if((*it).joinable())
	    	(*it).join();


}
}
};


#endif /* JOIN_THREADS_HPP_ */
