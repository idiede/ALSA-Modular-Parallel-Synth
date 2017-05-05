#ifndef M_VCA_H
#define M_VCA_H

#include "module.h"
#include "port.h"

#define MODULE_VCA_WIDTH                 85
#define MODULE_VCA_HEIGHT               140

class M_vca : public Module
{
    Q_OBJECT

    float gain1, gain2, in1, in2, out;
    float **gainData1, **gainData2, **inData1, **inData2;
    Port *port_M_gain1, *port_M_in1, *port_M_gain2, *port_M_in2, *port_out;

  public: 

    bool expMode;
    M_vca(bool p_expMode, QWidget* parent=0);
    void generateCycle(int );
};
  
#endif
