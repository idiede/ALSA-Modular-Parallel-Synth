#ifndef M_VCENV_H
#define M_VCENV_H

#include "module.h"


#define MODULE_VCENV_WIDTH                110
#define MODULE_VCENV_HEIGHT               175

class M_vcenv : public Module
{
    Q_OBJECT

    Port *port_M_gate, *port_M_retrigger, *port_M_attack, *port_M_decay, *port_M_sustain, *port_M_release, *port_out;
    
  public: 
    float **gateData, **retriggerData, **attackData, **decayData, **sustainData, **releaseData;        
    float a0, d0, s0, r0, aGain, dGain, sGain, rGain;
    float e[MAXPOLY], old_e[MAXPOLY];
    int timeScale, state[MAXPOLY], decayReleaseMode;
    bool noteActive[MAXPOLY], gate[MAXPOLY], retrigger[MAXPOLY];
                                    
  public:
    M_vcenv(QWidget* parent=0);

    void generateCycle();
};
  
#endif
