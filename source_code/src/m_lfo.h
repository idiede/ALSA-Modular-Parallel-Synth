#ifndef M_LFO_H
#define M_LFO_H

#include "module.h"


#define MODULE_LFO_WIDTH                 85
#define MODULE_LFO_HEIGHT               180

class M_lfo : public Module
{
    Q_OBJECT

    float freq, phi0;
    double si[MAXPOLY], old_si[MAXPOLY];
    double sa[MAXPOLY], old_sa[MAXPOLY];
    double t[MAXPOLY], old_t[MAXPOLY];
    double r[MAXPOLY], old_r[MAXPOLY];
    double sh[MAXPOLY], old_sh[MAXPOLY];
    int state[MAXPOLY];
    double dt[MAXPOLY], wave_period;
    bool trigger[MAXPOLY];
    Port *port_M_trigger, *port_sine, *port_tri, *port_sawup, *port_sawdown, *port_rect, *port_sh;
                                
  public:
    M_lfo(QWidget* parent=0);

    void generateCycle(int );
};
  
#endif
