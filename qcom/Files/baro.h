#ifndef BARO_H
#define BARO_H

#include <QWidget>
#include <QTimer>
#include "qcustom/qcustomplot.h"


namespace Ui {
class Baro;
}

class Baro : public QWidget
{
    Q_OBJECT

public:
    explicit Baro(QWidget *parent = 0);
    ~Baro();

    void Start_Init();

    void ShowBaroData();

    void ShowAcc_X_Curve(QCustomPlot *Acc_X_customPlot);
    void ShowAcc_Y_Curve(QCustomPlot *Acc_Y_customPlot);
    void ShowAcc_Z_Curve(QCustomPlot *Acc_Z_customPlot);

    void ShowBaro_Alt_Curve(QCustomPlot *Baro_Alt_customPlot);
    void ShowBaro_Pres_Curve(QCustomPlot *Baro_Pres_customPlot);
    void ShowBaro_Temp_Curve(QCustomPlot *Baro_Temp_customPlot);



private slots:
    void ShowDataSlot();

    void Acc_X_DataSlot();    //Acc_X
    void Acc_Y_DataSlot();
    void Acc_Z_DataSlot();

    void Baro_Alt_Slot();
    void Baro_Pres_Slot();
    void Baro_Temp_Slot();


private:
    Ui::Baro *ui;

    QTimer *BaroTimer;



};

#endif // BARO_H
