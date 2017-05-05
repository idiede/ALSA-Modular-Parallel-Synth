/*
 * getclocktimer.h
 *
 *  Created on: 2014-02-24
 *      Author: ede
 */

#ifndef GETCLOCKTIMER_H_
#define GETCLOCKTIMER_H_


#include <iostream>
#include <time.h>

class getclocktimer {
public:
	getclocktimer();
	virtual ~getclocktimer();

	timespec time1, time2;


    timespec diff(timespec , timespec );
};

#endif /* GETCLOCKTIMER_H_ */
