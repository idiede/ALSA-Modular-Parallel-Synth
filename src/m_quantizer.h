#ifndef M_QUANTIZER_H
#define M_QUANTIZER_H

#include "module.h"


#define MODULE_QUANTIZER_WIDTH                 90
#define MODULE_QUANTIZER_HEIGHT               140
#define MAX_LUT                                11

enum quantumType { QUANT_12, QUANT_6, QUANT_MAJOR, QUANT_MINOR, QUANT_CHORD_MAJOR, QUANT_CHORD_MINOR, QUANT_PENTA };

class M_quantizer : public Module
{
    Q_OBJECT

    Port *port_M_in, *port_M_trigger, *port_M_transpose, *port_out, *port_trigger_out;
    int qsig[MAXPOLY], trigCount[MAXPOLY];
    bool trigger[MAXPOLY];
    int quantum;
    int lut[MAX_LUT][12];
    
  public: 
    float **inData, **triggerData, **transposeData;       
                            
  public:
    M_quantizer(QWidget* parent=0);

    void generateCycle();
};
  
#endif
