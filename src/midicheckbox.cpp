#include "midicheckbox.h"


MidiCheckBox::MidiCheckBox(MidiControllable<float> &mcAble)
  : MidiGUIcomponent(mcAble)
{
  componentType = GUIcomponentType_checkbox;

  //  QWidget *dummy1 = new QWidget(this);
  QHBoxLayout *checkFrame = new QHBoxLayout(this);
  checkFrame->setSpacing(5);
  checkFrame->setMargin(5);

  //  QWidget *dummy2 = new QWidget(this);
//FIXME   setStretchFactor(dummy1, 3);
//   setStretchFactor(checkFrame, 1);
//   setStretchFactor(dummy2, 3);
  checkBox = new QCheckBox();
  checkFrame->addWidget(checkBox);  
  checkFrame->addStretch();  

  checkFrame->addWidget(&nameLabel);  
  QObject::connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
  mcAbleChanged();
}

MidiGUIcomponent *MidiCheckBox::createTwin()
{
  return new MidiCheckBox(static_cast<MidiControllable<float> &>(mcAble));
}

void MidiCheckBox::toggled(bool checked)
{
  static_cast<MidiControllable<float> &>(mcAble).setVal(checked, this);
}

void MidiCheckBox::mcAbleChanged()
{
  checkBox->blockSignals(true);
  checkBox->setChecked(mcAble.getValue() > 0);
  checkBox->blockSignals(false);
}

