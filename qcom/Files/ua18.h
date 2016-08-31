#ifndef UA18_H
#define UA18_H

#include <QWidget>

#include "qcustom/qcustomplot.h"
#include "QProgressBar"

namespace Ui {
class UA18;
}

class UA18 : public QWidget
{
    Q_OBJECT

public:
    explicit UA18(QWidget *parent = 0);
    ~UA18();
    void ShowBaroHigh(QCustomPlot *Baro_High_customPlot);

    void ShowUa18Info();
    void ShowProcessBar();
    void ShowMode_Sensors();



private slots:
    void ShowBaroHighSlot();
    void ShowU18DataSlot();


private:
    Ui::UA18 *ui;
    QTimer *ShowBaroHighTimer;


};


#endif // UA18_H
