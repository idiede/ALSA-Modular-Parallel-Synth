#include "colorwidget.h"
#include "prefwidget.h"

ColorWidget::ColorWidget(const QString& l, QGridLayout *layout, QColor &color,
        int pos, PrefWidget *pw, const char *slot)
: label(l), button(tr("Change..."))
{
    setAutoFillBackground(true);
    setPalette(QPalette(color, color));
    layout->addWidget(&label, pos - 1, 0);
    layout->addWidget(this, pos, 0);
    layout->addWidget(&button, pos, 1);
    connect(&button, SIGNAL(clicked()), pw, slot);
}

