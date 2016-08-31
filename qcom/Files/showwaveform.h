#ifndef SHOWWAVEFORM_H
#define SHOWWAVEFORM_H

#include <QWidget>
#include <QTimer>
#include "qcustom/qcustomplot.h"

namespace Ui {
class ShowWaveform;
}

class ShowWaveform : public QWidget
{
    Q_OBJECT

public:
    explicit ShowWaveform(QWidget *parent = 0);
    ~ShowWaveform();

    void setupDemo(int demoIndex);

    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void ShowAcc_Y_DataCurve(QCustomPlot *Acc_Y_customPlot);
    void ShowAcc_Z_DataCurve(QCustomPlot *Acc_Z_customPlot);

    void ShowGyro_X_DataCurve(QCustomPlot *Gyro_X_customPlot);
    void ShowGyro_Y_DataCurve(QCustomPlot *Gyro_Y_customPlot);
    void ShowGyro_Z_DataCurve(QCustomPlot *Gyro_Z_customPlot);

    void ShowSensorData(void);




private slots:

    void ShowDataSlot();    //Acc_X
    void Acc_Y_DataSlot();
    void Acc_Z_DataSlot();

    void Gyro_X_DataSlot();
    void Gyro_Y_DataSlot();
    void Gyro_Z_DataSlot();

    void realtimeDataSlot();

//    void bracketDataSlot();
//    void screenShot();
//    void allScreenShots();

private:
    Ui::ShowWaveform *ui;

    QString demoName;
    QTimer *dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    int currentDemoIndex;

};

#endif // SHOWWAVEFORM_H
