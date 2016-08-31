#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QTimer>
#include "qcustom/qcustomplot.h"

namespace Ui {
class FILTER;
}

class FILTER : public QWidget
{
    Q_OBJECT

public:
    explicit FILTER(QWidget *parent = 0);
    ~FILTER();

    void ShowBaroAlt(QCustomPlot *Baro_Alt_customPlot);

    void ShowAltData();

private slots:

    void Baro_Alt_Slot();

    void Show_Slot();




private:
    Ui::FILTER *ui;

    QTimer *ShowTimer;
};

#endif // FILTER_H
















