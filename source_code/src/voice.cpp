/*
 * voice.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: ede
 */

#include "voice.h"
#include "synthdata.h"

Voice::Voice() {

	     outdata = (float **)malloc(sizeof(float *));//2-D array
	     int out_size = synthdata->blocksize * synthdata->periodsize * sizeof(float);
	     outdata[0] = (float *)malloc(out_size);
	     memset(outdata[0], 0, out_size);

}

Voice::~Voice() {

//	int l1;
//	// TODO Auto-generated destructor stub
//	 for (l1 = 0; l1 < synthdata->blocksize; ++l1) {
//	       //free(outdata[l1][0]);
//	        free(outdata[l1]);
//	    }

	//free(outdata);
}

