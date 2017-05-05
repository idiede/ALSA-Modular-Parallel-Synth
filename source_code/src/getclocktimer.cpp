/*
 * getclocktimer.cpp
 *
 *  Created on: 2014-02-24
 *      Author: ede
 */

#include "getclocktimer.h"
/*
int main()
{
	timespec time1, time2;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	//garbage work goes here....

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	cout<<diff(time1,time2).tv_sec<<":"<<diff(time1,time2).tv_nsec<<endl;

}
*/

getclocktimer::getclocktimer() {
	// TODO Auto-generated constructor stub

}
timespec getclocktimer::diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}



getclocktimer::~getclocktimer() {
	// TODO Auto-generated destructor stub
}

