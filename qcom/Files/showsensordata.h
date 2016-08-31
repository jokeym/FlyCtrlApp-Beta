#ifndef SHOWSENSORDATA_H
#define SHOWSENSORDATA_H

#include <qtimer.h>
#include <QWidget>
#include <QVector>
#include "qcustom/qcustomplot.h"
#include <QStatusBar>
#include "Files/showwaveform.h"


namespace Ui {
class ShowSensorData;
}

class ShowSensorData : public QWidget
{
    Q_OBJECT
    
public:
    explicit ShowSensorData(QWidget *parent = 0);
    ~ShowSensorData();

    QStatusBar *statusBar;

    void ShowAllSensorData(void);


//////////////////////////////////////////////////////////////////

    
private slots:

    void ShowData_Slot();

    void on_pushButton_clicked();   //显示、隐藏数据
    void on_SaveSensorDataBtn_clicked();    //保存数据

private:
    Ui::ShowSensorData *ui;

    QString demoName;
    QTimer *ShowSensorData_Timer;
    QTimer *ShowDataTimer;
    ShowWaveform showSensorCurve;

};

#endif // SHOWSENSORDATA_H
