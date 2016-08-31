#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <QWidget>
#include <QTimer>
#include "qcustom/qcustomplot.h"

namespace Ui {
class GyroScope;
}

class GyroScope : public QWidget
{
    Q_OBJECT

public:
    explicit GyroScope(QWidget *parent = 0);
    ~GyroScope();

    void  start_Init();

    void ShowAcc_X_DataCurve(QCustomPlot *Acc_X_customPlot);
    void ShowAcc_Y_DataCurve(QCustomPlot *Acc_Y_customPlot);
    void ShowAcc_Z_DataCurve(QCustomPlot *Acc_Z_customPlot);

    void ShowGyro_X_DataCurve(QCustomPlot *Gyro_X_customPlot);
    void ShowGyro_Y_DataCurve(QCustomPlot *Gyro_Y_customPlot);
    void ShowGyro_Z_DataCurve(QCustomPlot *Gyro_Z_customPlot);

    void ShowSensorData(void);



private:
    Ui::GyroScope *ui;

    QTimer *GyroScopeTimer;


private slots:

    void Acc_X_DataSlot();    //Acc_X
    void Acc_Y_DataSlot();
    void Acc_Z_DataSlot();

    void Gyro_X_DataSlot();
    void Gyro_Y_DataSlot();
    void Gyro_Z_DataSlot();


    void ShowDataSlot();


};

#endif // GYROSCOPE_H
