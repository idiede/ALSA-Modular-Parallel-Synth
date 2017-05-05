#include <math.h>
#include <qstring.h>
#include <qslider.h>   
#include <qcheckbox.h>  
#include <qlabel.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qpainter.h>
#include "synthdata.h"
#include "midicombobox.h"
#include "m_vcf.h"
//#include "port.h"

#include "thread_pool.hpp"

//#include <future>
//#include <functional>


M_vcf::M_vcf(QWidget* parent) 
: Module(M_type_vcf, 1, parent, tr("VCF"))
{
	QString qs;

	setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_VCF_WIDTH, MODULE_VCF_HEIGHT);
	gain = 1.0;
	freq = 5;
	resonance = 0.8;
	dBgain = 3.0;
	initBuf(0);
	freq_const = 2.85f / 20000.0f;
	fInvertRandMax= 1.0f/(float)RAND_MAX ;
	b_noise = 19.1919191919191919191919191919191919191919;
	pi2_rate = 2.0f * M_PI / synthdata->rate; // how often changes the rate? I guess once on init, or?
	inv2_rate = 2.0 / (double)synthdata->rate;// this double seems unnecessary

	port_M_in = new Port(tr("In"), PORT_IN, 0, this);
	port_M_freq = new Port(tr("Freq"), PORT_IN, 1, this);
	port_M_exp = new Port(tr("Exp. FM"), PORT_IN, 2, this);
	port_M_lin = new Port(tr("Lin. FM"), PORT_IN, 3, this);
	port_M_resonance = new Port(tr("Resonance"), PORT_IN, 4, this);
	cv.out_off = 130;
	port_out = new Port(tr("Out"), PORT_OUT, 0, this);

	QStringList vcfTypeNames;
	vcfTypeNames << tr("Resonant Lowpass");
	vcfTypeNames << tr("Lowpass");
	vcfTypeNames << tr("Highpass");
	vcfTypeNames << tr("Bandpass I");
	vcfTypeNames << tr("Bandpass II");
	vcfTypeNames << tr("Notch");
	vcfTypeNames << tr("24 dB Lowpass I");
	vcfTypeNames << tr("24 dB Lowpass II");
	configDialog->addComboBox(tr("VCF Type"), vcfType, vcfTypeNames);
	QObject::connect(configDialog->midiComboBoxList.at(0)->comboBox, SIGNAL(highlighted(int)), this, SLOT(initBuf(int)));
	configDialog->addSlider(tr("Input Gain"), gain, 0, 10);
	configDialog->addSlider(tr("Frequency"), freq, 0, 10);
	configDialog->addSlider(tr("Exp. FM Gain"), vcfExpFMGain, 0, 10);
	configDialog->addSlider(tr("Lin. FM Gain"), vcfLinFMGain, 0, 10);
	configDialog->addSlider(tr("Resonance"), resonance, 0.01, 1);
	configDialog->addSlider(tr("Resonance Gain"), resonanceGain, 0, 1);


}

void M_vcf::initBuf(int) {

	int l1, l2;

	for (l1 = 0; l1 < MAXPOLY; ++l1) {
		for (l2 = 0; l2 < 5; ++l2) {
			buf[l2][l1] = 0;
			in[l2][l1] = 0;
		}
	}
}

void M_vcf::generateCycle(int v) {

	int l1;
	unsigned int l2;
	double t1, t2, fa, fb, q0, f, q, p, iv_sin, iv_cos, iv_alpha, a0, a1, a2, b0, b1, b2;

	int voice = v;

	    inData = port_M_in->getinputdata(voice);
	    freqData = port_M_freq->getinputdata(voice);
	    expFMData = port_M_exp->getinputdata(voice);
	    linFMData = port_M_lin->getinputdata(voice);
	    resonanceData = port_M_resonance->getinputdata(voice);

	    switch (vcfType) {
	      case VCF_LR:
	       {
	        double b_noiseout;
		int i_noise;
	        q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = freq_const * (synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                               + gain_linfm * linFMData[l1-voice][l2]);
	            if (f < 0.0) f = 0.0;
	            else if (f > 0.99) f = 0.99;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            fa = 1.0 - f;
	            fb = q * (1.0 + (1.0 / fa));
			// generate dither (?) noise
			b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;
			b_noiseout = b_noise - 1.5;// was 0.5
			b_noise = b_noise + 19.0;

	            buf[0][l1] = fa * buf[0][l1] + f * (gain * inData[l1-voice][l2] + fb * (buf[0][l1] - buf[1][l1]) + 0.00001 * b_noiseout);// ((float)rand() * fInvertRandMax - 1.0f));
	            buf[1][l1] = fa * buf[1][l1] + f * buf[0][l1];
	            data[0][l1][l2] = buf[1][l1];
	          }
	        }
	        break;
		}
	      case VCF_LPF:
	      {
	      double temp;

	        q0 = resonance;

	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                    + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ) f = MAX_FREQ;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            iv_sin = sin(pi2_rate * f);
	            iv_cos = cos(pi2_rate * f);
	            iv_alpha = iv_sin/(64.0 * q);
	            b0 = (1.0 - iv_cos) * 0.5;
	            b1 = 1.0 - iv_cos;
	            b2 = b0;
	            a0 = 1.0 + iv_alpha;
	            a1 = -2.0 * iv_cos;
	            a2 = 1.0 - iv_alpha;
	            temp = 1.0 / a0 * (b0 * gain * inData[l1-voice][l2] + b1 * buf[0][l1] + b2 * buf[1][l1]
	                                       - a1 * buf[2][l1] - a2 * buf[3][l1]);
	            data[0][l1][l2]=temp;
		    buf[1][l1] = buf[0][l1];
	            buf[0][l1] = gain * inData[l1-voice][l2];
	            buf[3][l1] = buf[2][l1];
	            buf[2][l1] = temp;//data[0][l1][l2];
	          }
	        }
	        break;
		}
	      case VCF_HPF:
	      {
	      double temp;

	        q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                    + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ) f = MAX_FREQ;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            iv_sin = sin(pi2_rate * f);
	            iv_cos = cos(pi2_rate * f);
	            iv_alpha = iv_sin/(64.0 * q);
	            b0 = (1.0 + iv_cos) * 0.5;
	            b1 = - 1.0 - iv_cos;
	            b2 = b0;
	            a0 = 1.0 + iv_alpha;
	            a1 = -2.0 * iv_cos;
	            a2 = 1.0 - iv_alpha;
	            temp = 1.0 / a0 * (gain * b0 * inData[l1-voice][l2] + b1 * buf[0][l1] + b2 * buf[1][l1]
	                                       - a1 * buf[2][l1] - a2 * buf[3][l1]);
	            data[0][l1][l2]=temp;
		    buf[1][l1] = buf[0][l1];
	            buf[0][l1] = gain * inData[l1-voice][l2];
	            buf[3][l1] = buf[2][l1];
	            buf[2][l1] = temp;//data[0][l1][l2];
	          }
	        }
	        break;
		}
	      case VCF_BPF_I:
	      	{
		double temp;

	        q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                    + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ) f = MAX_FREQ;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            iv_sin = sin(pi2_rate * f);
	            iv_cos = cos(pi2_rate * f);
	            iv_alpha = iv_sin/(64.0 * q);
	            b0 = q * iv_alpha;
	            b1 = 0.0;
	            b2 = -q * iv_alpha;
	            a0 = 1.0 + iv_alpha;
	            a1 = -2.0 * iv_cos;
	            a2 = 1.0 - iv_alpha;

		    temp = 1.0 / a0 * (gain * b0 * inData[l1-voice][l2] + b1 * buf[0][l1] + b2 * buf[1][l1]
	                                       - a1 * buf[2][l1] - a2 * buf[3][l1]);
	            data[0][l1][l2]=temp;
		    buf[1][l1] = buf[0][l1];
	            buf[0][l1] = gain * inData[l1-voice][l2];
	            buf[3][l1] = buf[2][l1];
	            buf[2][l1] = temp;//data[0][l1][l2];
	          }
	        }
	        break;
		}
	      case VCF_BPF_II:
	       {
	       double temp;
	        q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                    + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ) f = MAX_FREQ;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            iv_sin = sin(pi2_rate * f);
	            iv_cos = cos(pi2_rate * f);
	            iv_alpha = iv_sin/(64.0 * q);
	            b0 = iv_alpha;
	            b1 = 0.0;
	            b2 = -iv_alpha;
	            a0 = 1.0 + iv_alpha;
	            a1 = -2.0 * iv_cos;
	            a2 = 1.0 - iv_alpha;
	            temp = 1.0 / a0 * (gain * b0 * inData[l1-voice][l2] + b1 * buf[0][l1] + b2 * buf[1][l1]
	                                       - a1 * buf[2][l1] - a2 * buf[3][l1]);
	            data[0][l1][l2] = temp;
		    buf[1][l1] = buf[0][l1];
	            buf[0][l1] = gain * inData[l1-voice][l2];
	            buf[3][l1] = buf[2][l1];
	            buf[2][l1] = temp;//data[0][l1][l2];
	          }
	        }
	        break;
		}
	      case VCF_NF:
	       {
	       double temp;
	        q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                    + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ) f = MAX_FREQ;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            iv_sin = sin(pi2_rate * f);
	            iv_cos = cos(pi2_rate * f);
	            iv_alpha = iv_sin/(64.0 * q);
	            b0 = 1.0;
	            b1 = -2.0 * iv_cos;
	            b2 = 1.0;
	            a0 = 1.0 + iv_alpha;
	            a1 = -2.0 * iv_cos;
	            a2 = 1.0 - iv_alpha;

		    temp = 1.0 / a0 * (gain * b0 * inData[l1-voice][l2] + b1 * buf[0][l1] + b2 * buf[1][l1]
	                                       - a1 * buf[2][l1] - a2 * buf[3][l1]);
		    data[0][l1][l2] = temp;// conversion
	            buf[1][l1] = buf[0][l1];
	            buf[0][l1] = gain * inData[l1-voice][l2];
	            buf[3][l1] = buf[2][l1];
	            buf[2][l1] = temp;//data[0][l1][l2];
	          }
	        }
	        break;
		}
	      case VCF_MOOG1:
	      {// Timo Tossavainen version
		double b_noiseout;
	        int i_noise;
		q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                     + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ2) f = MAX_FREQ2;
	            q = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (q < 0.01) q = 0.01;
	            else if (q > 1.0) q = 1.0;
	            fa = inv2_rate * f;
	            moog_f = fa * 1.16f;
		    revMoog = 1.0f - moog_f;
		    moog2times= moog_f * moog_f;
	            fb = 4.1 * q * (1.0 - 0.15 * moog2times);
		    // generate dither (?) noise
		     	b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;

			b_noiseout = b_noise - 1.5; // was - 0.5 now with - 1.0

			b_noise = b_noise + 19.0;
	            in[0][l1] = gain * inData[l1-voice][l2] + 0.000001 * b_noiseout;//((float)rand() * fInvertRandMax - 1.0);
	            in[0][l1] -= fb * buf[4][l1];
	            in[0][l1] *=0.35013 * (moog2times * moog2times);
	            buf[1][l1] = in[0][l1] + 0.3 * in[1][l1] + revMoog * buf[1][l1];
	            in[1][l1] = in[0][l1];
	            buf[2][l1] = buf[1][l1] + 0.3 * in[2][l1] + revMoog * buf[2][l1];
	            in[2][l1] = buf[1][l1];
	            buf[3][l1] = buf[2][l1] + 0.3 * in[3][l1] + revMoog * buf[3][l1];
	            in[3][l1] = buf[2][l1];
	            buf[4][l1] = buf[3][l1] + 0.3 * in[4][l1] + revMoog * buf[4][l1];
	            in[4][l1] = buf[3][l1];
	            data[0][l1][l2] = buf[4][l1];
	          }
	        }
	        break;
		}
	      case VCF_MOOG2:                       // Paul Kellet version
	       {
	       	double b_noiseout;
	        int i_noise;

	       q0 = resonance;
	        freq_tune = 5.0313842f + freq;
	        gain_linfm = 1000.0f * vcfLinFMGain;
	        for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
	          for (l2 = 0; l2 < synthdata->cyclesize; ++l2) {
	            f = synthdata->exp2_table(freq_tune + freqData[l1-voice][l2] + vcfExpFMGain * expFMData[l1-voice][l2])
	                     + gain_linfm * linFMData[l1-voice][l2];
	            if (f < MIN_FREQ) f = MIN_FREQ;
	            else if (f > MAX_FREQ2) f = MAX_FREQ2;
	            qr = q0 + resonanceGain * resonanceData[l1-voice][l2];
	            if (qr < 0.01) qr = 0.01;
	            else if (qr > 1.0) qr = 1.0;
	            fb = inv2_rate * f;
	            q = 1.0 - fb;
	            p = fb + 0.8 * fb * q;
	            fa = p + p - 1.0;
	            q = qr * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));
		     // generate dither (?) noise
		     	b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;

			b_noiseout = b_noise - 1.5;// was 0.5

			b_noise = b_noise + 19.0;

	            in[0][l1] = gain * inData[l1-voice][l2] + 0.000001 * b_noiseout;//* ((float)rand() * fInvertRandMax - 1.0);
	            in[0][l1] -= q * buf[4][l1];
	            if (in[0][l1] < -1.0) in[0][l1] = -1.0;
	            if (in[0][l1] > 1.0) in[0][l1] = 1.0;
	            t1 = buf[1][l1];
	            buf[1][l1] = (in[0][l1] + buf[0][l1]) * p - buf[1][l1] * fa;
	            t2 = buf[2][l1];
	            buf[2][l1] = (buf[1][l1] + t1) * p - buf[2][l1] * fa;
	            t1 = buf[3][l1];
	            buf[3][l1] = (buf[2][l1] + t2) * p - buf[3][l1] * fa;
	            buf[4][l1] = (buf[3][l1] + t1) * p - buf[4][l1] * fa;
	            buf[4][l1] -= buf[4][l1] * buf[4][l1] * buf[4][l1] * 0.166667;
	            buf[0][l1] = in[0][l1];
	            data[0][l1][l2] = buf[4][l1];
	          }
	        }
	        break;
		}


	    }
	}


