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
#include "m_inv.h"
#include "port.h"

M_inv::M_inv(QWidget* parent) 
  : Module(M_type_inv, 1, parent, tr("Inverter"))
{
  QString qs;

  M_type = M_type_inv;
  setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_INV_WIDTH, MODULE_INV_HEIGHT);
  port_M_in = new Port("In", PORT_IN, 0, this); 
  cv.out_off = 55;
  port_out = new Port("Out", PORT_OUT, 0, this);          
}

void M_inv::generateCycle(int voice_index) {

  int l1;
  unsigned int l2;
    int voice = voice_index;
    inData = port_M_in->getinputdata(voice);

    for (l1 = voice; l1 < voice + synthdata->blocksize; ++l1) {
      for (l2 = 0; l2 < synthdata->cyclesize; l2++) {
        data[0][l1][l2] = -inData[l1-voice][l2];
      }
    }
}

