#ifndef PORT_POPUP_H
#define PORT_POPUP_H

#include <QAction>
#include <QMenu>

class PopupMenu: public QMenu {
  Q_OBJECT

public:
    enum portAction {
        paNone = 0,
        paDisconnect,
        paDefaultCable,
        paGrayCable,
        paRedCable,
        paGreenCable,
        paBlueCable,
        paYellowCable,
        paSetJackColor,
        paSetCableColor
    };

  PopupMenu(QWidget* parent = NULL);
  PopupMenu::portAction runAt(const QPoint&);

private:
  QAction* acDisconnect;
  QAction* acDefaultCable;
  QAction* acGrayCable;
  QAction* acRedCable;
  QAction* acGreenCable;
  QAction* acBlueCable;
  QAction* acYellowCable;
  QAction* acSetJackColor;
  QAction* acSetCableColor;


/*
public slots:
  void disconnectClicked() {
    port->disconnectClicked();
  }
  void cableGrayClicked() {
    port->cableGrayClicked();
    port->module->modularSynth()->update();
  }
  void cableRedClicked() {
    port->cableRedClicked();
    port->module->modularSynth()->update();
  }
  void cableGreenClicked() {
    port->cableGreenClicked();
    port->module->modularSynth()->update();
  }
  void cableBlueClicked() {
    port->cableBlueClicked();
    port->module->modularSynth()->update();
  }
  void cableYellowClicked() {
    port->cableYellowClicked();
    port->module->modularSynth()->update();
  }
  void cableDefaultClicked() {
    port->cableDefaultClicked();
    port->module->modularSynth()->update();
  }
  void jackColorClicked() {
    port->jackColorClicked();
    port->module->modularSynth()->update();
  }
  void cableColorClicked() {
    port->cableColorClicked();
    port->module->modularSynth()->update();
  }
  */
};

#endif
