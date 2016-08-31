#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include "qextserial/qextserialport.h"
#include "aboutdialog.h"

#include "Files/flycontrol.h"
#include "Files/showpid.h"
#include "Files/showsensordata.h"
#include "Files/showwaveform.h"
#include "Files/u30.h"
#include "Files/serialset.h"
#include "Files/gyroscope.h"
#include "Files/baro.h"
#include "Files/mag.h"
#include "Files/remotectrl.h"
#include "Files/filter.h"
#include "Files/ua18.h"


#define TIME_OUT 10

#define OBO_TIMER_INTERVAL 3000

#define Send_Data_Time_Value   100

#define MAX_FILE_SIZE 10000
#define TIP_FILE_SIZE 1000


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startInit();
    void ShowU30_info();
    void ShowU30_Motor();

    QDesktopWidget* pDw = QApplication::desktop();//获得桌面窗体
    QRect rect = pDw->screenGeometry();//获得分辨率

protected:
    void changeEvent(QEvent *e);
    void setActionsEnabled(bool status);
    void setComboBoxEnabled(bool status);

private:
    Ui::MainWindow *ui;
    AboutDialog aboutdlg;


    QTimer *AnalyseU30Timer;
    QTimer *Send_Data_Timer;
    QTimer *GetCoordTimer;

    FlyControl FlyStatus;
    ShowPID  showpid;
    ShowSensorData myShowsensor;
    ShowWaveform ShowSensorWave;
    U30 u30;
    SerialSet serialset;
    GyroScope gyroscope;
    Baro baro;
    Mag mag;
    RemoteCtrl remotectrl;
    FILTER Filter;
    UA18 ua18;



    unsigned int timerdly;
    unsigned int obotimerdly;
    QString write2fileName;
    unsigned int Send_Data_Timerdly;


/***************
 *
 * U30的一些信息
 *
*****************/

    int version,MSP_version,multiType,multiCapability=0;
    int cycleTime, i2cError,mode,present=0;
    float mot[8],servo[8];
    int GPS_distanceToHome, GPS_directionToHome,
        GPS_numSat, GPS_fix, GPS_update, GPS_altitude, GPS_speed,
        GPS_latitude, GPS_longitude,pMeterSum,intPowerTrigger,bytevbat;
    int alt, head, angx, angy;
    int byteMP[];
    float debug1, debug2, debug3, debug4;
    int  byteRC_RATE,byteRC_EXPO, byteRollPitchRate, byteYawRate, byteDynThrPID,byteThrottle_EXPO, byteThrottle_MID, byteSelectSetting;
    double  byteP[10],byteI[10],byteD[10];
    int activation[];

    static  const int
    IDLE = 0,
    HEADER_START = 1,
    HEADER_M = 2,
    HEADER_ARROW = 3,
    HEADER_SIZE = 4,
    HEADER_CMD = 5,
    HEADER_ERR = 6;

    int c_state = IDLE;
    bool err_rcvd = false;

    unsigned char checksum=0;
    unsigned char cmd;
    int offset=0, dataSize=0;
    int time,time2,time3,time4;
    bool toggleRead = false,toggleReset = false,toggleCalibAcc = false,toggleCalibMag = false,toggleWrite = false,toggleRXbind = false,toggleSetSetting = false;

    static const int
        MSP_IDENT                =100,
        MSP_STATUS               =101,
        MSP_RAW_IMU              =102,
        MSP_SERVO                =103,
        MSP_MOTOR                =104,
        MSP_RC                   =105,
        MSP_RAW_GPS              =106,
        MSP_COMP_GPS             =107,
        MSP_ATTITUDE             =108,
        MSP_ALTITUDE             =109,
        MSP_BAT                  =110,
        MSP_RC_TUNING            =111,
        MSP_PID                  =112,
        MSP_BOX                  =113,
        MSP_MISC                 =114,
        MSP_MOTOR_PINS           =115,
        MSP_BOXNAMES             =116,
        MSP_PIDNAMES             =117,

        MSP_SET_RAW_RC           =200,
        MSP_SET_RAW_GPS          =201,
        MSP_SET_PID              =202,
        MSP_SET_BOX              =203,
        MSP_SET_RC_TUNING        =204,
        MSP_ACC_CALIBRATION      =205,
        MSP_MAG_CALIBRATION      =206,
        MSP_SET_MISC             =207,
        MSP_RESET_CONF           =208,
        MSP_SELECT_SETTING       =210,

        MSP_BIND                 =240,

        MSP_EEPROM_WRITE         =250,

        MSP_DEBUGMSG             =253,
        MSP_DEBUG                =254
      ;





private slots:

    void on_actionAbout_triggered();

    void Show_Rev_Data(void);
    void RealTimeGetCoord(void);
    void AnalyseU30Slot();

    void on_actionPID_triggered();
    void on_actionCurve_triggered();
    void on_actionSensorData_triggered();
    void on_actionU30_triggered();
    void on_actionSerialSet_triggered();
    void on_actionGyro_triggered();
    void on_actionBaro_triggered();
    void on_actionBaro_Alt_triggered();
    void on_actionMag_triggered();
    void on_actionRC_triggered();
    void on_actionUA18_triggered();
};

#endif // MAINWINDOW_H
