#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <qwidget.h>
#include <qstring.h>
#include "synthdata.h"
#include "m_vco.h"
//#include "port.h"
#include "thread_pool.hpp"

//#include <future>
//#include <functional>

M_vco::M_vco(QWidget* parent) 
: Module(M_type_vco, 5, parent, tr("VCO"))
{
	QString qs;
	int l1;

	setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_VCO_WIDTH, MODULE_VCO_HEIGHT);
	wave_period = (float)WAVE_PERIOD;
	wave_period_2 = wave_period * 0.5f;
	wave_period_3 = wave_period / 3.0;
	Pi2Times= (2.0f * M_PI);
	PKonst = wave_period / Pi2Times;

	freq = 0;
	vcoExpFMGain = 0;
	vcoLinFMGain = 0;
	pwGain = 0;
	phi0 = 0;
	harmonic = 1;
	subharmonic = 1;
	octave = 3;
	edge = 0.95;
	for (l1 = 0; l1 < MAXPOLY; ++l1) {
		phi[l1] = 0;
	}
	pw0 = 0.5;
	waveForm = WAVE_SAW;

	cv.in_index = 1;
	cv.in_off -= cv.step;
	port_M_freq = new Port(tr("Freq"), PORT_IN, 1, this);
	port_M_exp = new Port(tr("Exp. FM"), PORT_IN, 2, this);
	port_M_lin = new Port(tr("Lin. FM"), PORT_IN, 3, this);
	port_M_pw = new Port(tr("PW"), PORT_IN, 4, this);

	cv.out_off = 115;
	port_sine = new Port(tr("Sine"), PORT_OUT, 0, this);
	port_tri = new Port(tr("Triangle"), PORT_OUT, 1, this);
	port_saw = new Port(tr("Saw"), PORT_OUT, 2, this);
	port_rect = new Port(tr("Rectangle"), PORT_OUT, 3, this);
	port_aux = new Port(tr("Aux"), PORT_OUT, 4, this);

	configDialog->initTabWidget();
	QVBoxLayout *freqTab = configDialog->addVBoxTab(tr("Frequency"));
	configDialog->addIntSlider(tr("Octave"), octave, 0, 6, freqTab);
	configDialog->addSlider(tr("Tune"), freq, 0, 1, false, freqTab);
	configDialog->addIntSlider(tr("Harmonic"), harmonic, 1, 16, freqTab);
	configDialog->addIntSlider(tr("Subharmonic"), subharmonic, 1, 16, freqTab);

	QVBoxLayout *pulseTab = configDialog->addVBoxTab(tr("Pulse Width / Phase"));
	configDialog->addSlider(tr("PW"), pw0, 0.1, 0.9, false, pulseTab);
	configDialog->addSlider(tr("PW Gain"), pwGain, 0, 1, false, pulseTab);
	configDialog->addSlider(tr("Phi0"), phi0, 0, 6.283, false, pulseTab);

	QVBoxLayout *modulationTab = configDialog->addVBoxTab(tr("Modulation / Aux Waveform"));
	QStringList waveFormNames;
	waveFormNames << tr("Saw");
	waveFormNames << tr("Saw 1");
	waveFormNames << tr("Saw 2");
	configDialog->addComboBox(tr("Aux Wave Form"), waveForm, waveFormNames, modulationTab);
	configDialog->addSlider(tr("Exp. FM Gain"), vcoExpFMGain, 0, 10, false, modulationTab);
	configDialog->addSlider(tr("Lin. FM Gain"), vcoLinFMGain, 0, 10, false, modulationTab);

}

void M_vco::generateCycle(int v) {

	int l1, phint;
	unsigned int l2;
	float dphi, phi1, phi_const, pw, d, dd, dsaw, half_wave, third_wave;
	float freq_const, freq_tune, gain_linfm,  pw_low, pw_high;
    int voice = v;

	edge = 0.01f + 1.8f * synthdata->edge;

	    freqData = port_M_freq->getinputdata(voice);
	    expFMData = port_M_exp->getinputdata(voice);
	    linFMData = port_M_lin->getinputdata(voice);
	    pwData = port_M_pw->getinputdata(voice);

	    freq_const = wave_period / (float)synthdata->rate * (float)harmonic / (float)subharmonic;
	    freq_tune = 4.0313842f + octave + freq;
	    gain_linfm = 1000.0f * vcoLinFMGain;
	    phi_const = phi0 * PKonst;
	    pw_low = 0.1f * wave_period;
	    pw_high = 0.9f * wave_period;

	    if (phi0 > 0.0f) {
	      for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {//break up into blocks
	            for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	                dphi = freq_const * (synthdata->exp2_table(freq_tune + freqData[l1-voice][l2]
					        + vcoExpFMGain * expFMData[l1-voice][l2])
	                        + gain_linfm * linFMData[l1-voice][l2]);
	                if (dphi > wave_period_2) dphi = wave_period_2;
	                phi1 = phi[l1] + phi_const;
	                if (phi1 < 0.0f) phi1 += wave_period;
	                else if (phi1 >= wave_period) phi1 -= wave_period;
	                phint=(int)phi1;
	                data[0][l1][l2] = synthdata->wave_sine[phint];
	                data[1][l1][l2] = synthdata->wave_tri[phint];
	                switch (waveForm) {
	                    case WAVE_SAW:
	                        data[4][l1][l2] = synthdata->wave_saw2[phint];
	                        break;
	                    case WAVE_SAW2:
	                        half_wave = wave_period_2;// * 0.5f;// / 2.0;
	                        data[4][l1][l2] = (phi1 < half_wave)
	                            ? synthdata->wave_saw2[(int)(2.0f * phi1)]
	                            : 0.0f;
	                        break;
	                    case WAVE_SAW3:
	                        third_wave = wave_period_3;// * 0.3333333333334f;// / 3.0f;
	                        data[4][l1][l2] = (phi1 < third_wave)
	                            ? synthdata->wave_saw2[(int)(3.0f * phi1)]
	                            : 0.0f;
	                        break;
	                }
	                pw = (pw0 + pwGain * pwData[l1-voice][l2]) * wave_period;
	                if (pw < pw_low) pw = pw_low;
	                else if (pw > pw_high) pw = pw_high;
	                d = edge * dphi;
	                dd = 1.0f / d;
	                dsaw = 2.0f / (wave_period - 2.0f * d);
	                if (phi1 <= d) {
	                    data[3][l1][l2] = phi1 * dd;
	                    data[2][l1][l2] = phi1 * dd;
	                } else {
	                    if (phi1 <= pw - d) {
	                        data[3][l1][l2] = 1.0f;
	                        data[2][l1][l2] = 1.0f - (phi1 - d) * dsaw;
	                    } else {
	                        if (phi1 <= pw + d) {
	                            data[3][l1][l2] = (pw - phi1) * dd;
	                            data[2][l1][l2] = 1.0f - (phi1 - d) * dsaw;
	                        } else {
	                            if (phi1 <= wave_period - d) {
	                                data[3][l1][l2] = -1.0f;
	                                data[2][l1][l2] = 1.0f - (phi1 - d) * dsaw;
	                            } else {
	                                data[3][l1][l2] = (phi1 - wave_period) * dd;
	                                data[2][l1][l2] = data[3][l1][l2];  //(phi1 - wave_period) * dd;
	                            }
	                        }
	                    }
	                }
	                phi[l1] += dphi;
	                while (phi[l1] < 0.0f) phi[l1] += wave_period;
	                while (phi[l1] >= wave_period) phi[l1] -= wave_period;
	            }
	       }
	    } else {
	    	 for (l1 = voice; l1 < voice+synthdata->blocksize; ++l1) {
	            for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	                dphi = freq_const * (synthdata->exp2_table(freq_tune + freqData[l1-voice][l2]
						+ vcoExpFMGain * expFMData[l1-voice][l2]) + gain_linfm * linFMData[l1-voice][l2]);
	                if (dphi > wave_period_2) dphi = wave_period_2;
	                phint=(int)phi[l1];
	                data[0][l1][l2] = synthdata->wave_sine[phint];
	                data[1][l1][l2] = synthdata->wave_tri[phint];
	                switch (waveForm) {
	                    case WAVE_SAW:
	                        data[4][l1][l2] = synthdata->wave_saw2[phint];
	                        break;
	                    case WAVE_SAW2:
	                        half_wave = wave_period_2;// * 0.5f; // / 2.0;
	                        data[4][l1][l2] = (phi[l1] < half_wave)
	                            ? synthdata->wave_saw2[(int)(2.0f * phi[l1])]
	                            : 0.0f;
	                        break;
	                    case WAVE_SAW3:
	                        third_wave = wave_period_3;// * 0.3333333333334f;// / 3.0f;
	                        data[4][l1][l2] = (phi[l1] < third_wave)
	                            ? synthdata->wave_saw2[(int)(3.0f * phi[l1])]
	                            : 0.0f;
	                        break;
	                }
	                pw = (pw0 + pwGain * pwData[l1-voice][l2]) * wave_period;
	                if (pw < pw_low) pw = pw_low;
	                else if (pw > pw_high) pw = pw_high;
	                d = edge * dphi;
	                dd = 1.0f / d;
	                dsaw = 2.0f / (wave_period - 2.0f * d);
	                if (phi[l1] <= d) {
	                    data[3][l1][l2] = phi[l1] * dd;
	                    data[2][l1][l2] = data[3][l1][l2];//phi[l1] * dd;
	                } else {
	                    if (phi[l1] <= pw - d) {
	                        data[3][l1][l2] = 1.0f;
	                        data[2][l1][l2] = 1.0f - (phi[l1] - d) * dsaw;
	                    } else {
	                        if (phi[l1] <= pw + d) {
	                            data[3][l1][l2] = (pw - phi[l1]) * dd;
	                            data[2][l1][l2] = 1.0f - (phi[l1] - d) * dsaw;
	                        } else {
	                            if (phi[l1] <= wave_period - d) {
	                                data[3][l1][l2] = -1.0;
	                                data[2][l1][l2] = 1.0f - (phi[l1] - d) * dsaw;
	                            } else {
	                                data[3][l1][l2] = (phi[l1] - wave_period) * dd;
	                                data[2][l1][l2] = data[3][l1][l2];//(phi[l1] - wave_period) * dd;
	                            }
	                        }
	                    }
	                }
	                phi[l1] += dphi;
	                while (phi[l1] < 0.0f) phi[l1] += wave_period;
	                while (phi[l1] >= wave_period) phi[l1] -= wave_period;
	            }
	        }
	    }
}

