#ifndef M_ENV_H
#define M_ENV_H

#include "module.h"


#define MODULE_ENV_WIDTH                100
#define MODULE_ENV_HEIGHT               120
#define ENVELOPE_RESPONSE               256

class M_env : public Module
{
    Q_OBJECT

    float delay, attack, hold, decay, sustain, release;
    float e_noteOff[MAXPOLY], de[MAXPOLY];
    Port *port_gate, *port_retrigger, *port_inverse_out, *port_gain_out;

  public: 
    float e[MAXPOLY];
    float timeScale;
    bool gate[MAXPOLY], retrigger[MAXPOLY];
    int noteOnOfs[MAXPOLY];
    float **gateData, **retriggerData;
                
  public:
    M_env(QWidget* parent=0);

    void generateCycle(int );
};
  
#endif
