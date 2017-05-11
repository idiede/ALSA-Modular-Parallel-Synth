/*
 * m_fsin.h
 *
 *  Created on: May 9, 2017
 *      Author: ede
 */

#ifndef M_FSIN_H_
#define M_FSIN_H_

#include "module.h"

#define MODULE_INV_WIDTH                 90
#define MODULE_INV_HEIGHT                140


class M_fsin : public Module {
public:

	Q_OBJECT

	float freq, phi0;
	float wave_period;
	float phi[MAXPOLY];
	float edge;

	float wave_period_2,wave_period_3,Pi2Times,PKonst;

	Port *port_M_freq,  *port_M_exp, *port_M_lin, *port_out;

	float vcoExpFMGain, vcoLinFMGain;
	int harmonic, subharmonic, octave;
  public:
	float **freqData;
	float **expFMData;        // Frequency modulation exp characteristic
	float **linFMData;

	M_fsin(QWidget* parent=0);


	void generateCycle(int );
	virtual ~M_fsin();
};

#endif /* SOURCE_CODE_SRC_M_FSIN_H_ */
