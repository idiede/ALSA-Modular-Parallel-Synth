#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QPushButton>

/*forward declaration*/
class PrefWidget;

class ColorWidget: public QWidget
{
    Q_OBJECT

    QLabel label;
    QPushButton button;

    public:
    ColorWidget(const QString&, QGridLayout*, QColor&, int,
            PrefWidget*, const char *);
};


