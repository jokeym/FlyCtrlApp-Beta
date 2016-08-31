#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "Files/flycontrol.h"
#include "Files/showpid.h"
#include "Files/showsensordata.h"
#include "Files/showwaveform.h"

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();
protected:
    void run();

    void ShowPIDThread();
    void ShowWaveThread();
    void showSensorDataThread();

    void AnalyseData(unsigned char *data_buf);


private:

    ShowPID  showpid;
    ShowSensorData Showsensordata;
    ShowWaveform showsensorwave;


};

#endif // WORKTHREAD_H
