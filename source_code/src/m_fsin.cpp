/*
 * m_fsin.cpp
 *
 *  Created on: May 9, 2017
 *      Author: ede
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <qwidget.h>
#include <qstring.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qlabel.h>


#include <qspinbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qpainter.h>
#include <alsa/asoundlib.h>
#include "synthdata.h"
#include "port.h"
#include "m_fsin.h"

M_fsin::M_fsin(QWidget* parent)
  : Module(M_type_fsin, 1, parent, tr("Fast Sine")) {

	QString qs;
	M_type = M_type_fsin;
    setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_INV_WIDTH, MODULE_INV_HEIGHT);

    wave_period = (float)WAVE_PERIOD;
	wave_period_2 = wave_period * 0.5f;
	wave_period_3 = wave_period / 3.0;
	Pi2Times= (2.0f * M_PI);
	PKonst = wave_period / Pi2Times;

	freq = 0;
	phi0 = 0;
	vcoExpFMGain = 0;
    vcoLinFMGain = 0;
	harmonic = 1;
	subharmonic = 1;
	octave = 3;
	edge = 0.95;

	for (int l1 = 0; l1 < MAXPOLY; ++l1) {
			phi[l1] = 0;
		}


	  port_M_freq = new Port(tr("Freq"), PORT_IN, 0, this);
	  port_M_exp = new Port(tr("Exp. FM"), PORT_IN, 2, this);
	  port_M_lin = new Port(tr("Lin. FM"), PORT_IN, 3, this);
	  cv.out_off = 55;
	  port_out = new Port("Out", PORT_OUT, 0, this);

	  configDialog->initTabWidget();
	  QVBoxLayout *freqTab = configDialog->addVBoxTab(tr("Frequency"));
	  configDialog->addIntSlider(tr("Octave"), octave, 0, 6, freqTab);
	  configDialog->addSlider(tr("Tune"), freq, 0, 1, false, freqTab);
	  configDialog->addIntSlider(tr("Harmonic"), harmonic, 1, 16, freqTab);
	  configDialog->addIntSlider(tr("Subharmonic"), subharmonic, 1, 16, freqTab);

	  QVBoxLayout *modulationTab = configDialog->addVBoxTab(tr("Modulation"));
	  configDialog->addSlider(tr("Exp. FM Gain"), vcoExpFMGain, 0, 10, false, modulationTab);
	  configDialog->addSlider(tr("Lin. FM Gain"), vcoLinFMGain, 0, 10, false, modulationTab);

}

M_fsin::~M_fsin() {
	// TODO Auto-generated destructor stub
}

void M_fsin::generateCycle(int voice_index){
	    int l1, phint;
	    unsigned int l2;
	    int voice = voice_index;
	    float dphi, phi1, phi_const;
	    float freq_const, freq_tune, gain_linfm;

	    freq_const = wave_period / (float)synthdata->rate * (float)harmonic / (float)subharmonic;
	    freq_tune = 4.0313842f + octave + freq;
	    gain_linfm = 1000.0f * vcoLinFMGain;
	    phi_const = phi0 * PKonst;
	    edge = 0.01f + 1.8f * synthdata->edge;

	    freqData = port_M_freq->getinputdata(voice);
	    expFMData = port_M_exp->getinputdata(voice);
	    linFMData = port_M_lin->getinputdata(voice);

//	   if (phi0 > 0.0f) {
//
//	    for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
//	      for (l2 = 0; l2 < synthdata->cyclesize; l2++) {
//	    	  dphi = freq_const * (synthdata->exp2_table(freq_tune + freqData[l1-voice][l2]
//							+ vcoExpFMGain * expFMData[l1-voice][l2])
//							+ gain_linfm * linFMData[l1-voice][l2]);
//					if (dphi > wave_period_2) dphi = wave_period_2;
//					phi1 = phi[l1] + phi_const;
//					if (phi1 < 0.0f) phi1 += wave_period;
//					else if (phi1 >= wave_period) phi1 -= wave_period;
//					phint=(int)phi1;
//	    	  	    data[0][l1][l2] = synthdata->wave_sine[phint];
//	        //data[0][l1][l2] = -freqData[l1-voice][l2];
//	      }
//	    }
//	   } else {
		   //std::cerr << "running" << std::endl;
		   for (l1 = voice; l1 < voice+synthdata->blocksize; ++l1) {
				for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
					dphi = freq_const * (synthdata->exp2_table(freq_tune + freqData[l1-voice][l2]
						+ vcoExpFMGain * expFMData[l1-voice][l2]) + gain_linfm * linFMData[l1-voice][l2]);
					if (dphi > wave_period_2) dphi = wave_period_2;
					phint=(int)phi[l1];
					data[0][l1][l2] = synthdata->wave_sine[phint];

					phi[l1] += dphi;
					while (phi[l1] < 0.0f) phi[l1] += wave_period;
					while (phi[l1] >= wave_period) phi[l1] -= wave_period;
	     }
      }
}

