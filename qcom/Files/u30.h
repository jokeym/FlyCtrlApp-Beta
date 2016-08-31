#ifndef U30_H
#define U30_H

#include <QWidget>
#include <QString>
//#include <QTimer>

#include "Files/gyroscope.h"

namespace Ui {
class U30;
}

class U30 : public QWidget
{
    Q_OBJECT

public:
    explicit U30(QWidget *parent = 0);
    ~U30();


    void AnalyseU30Data(unsigned char *data_buf);

    void infoTable_1();
    void infoTable_2();
//    void infoTable_3();


    int U30_Flag=0;

private slots:

    void ShowU30Information(void);

private:
    Ui::U30 *ui;

    QTimer *UpdateU30Timer;

    GyroScope gyroscope;

    int version,MSP_version,multiType,multiCapability=0;
    int cycleTime, i2cError,mode,present=0;
    float mot[9],servo[8];
    int GPS_distanceToHome, GPS_directionToHome,
        GPS_numSat, GPS_fix, GPS_update, GPS_altitude, GPS_speed,
        GPS_latitude, GPS_longitude,pMeterSum,intPowerTrigger,bytevbat;
    float alt, head, angx, angy;
    int byteMP[];
    float debug1, debug2, debug3, debug4;
    int  byteRC_RATE,byteRC_EXPO, byteRollPitchRate, byteYawRate, byteDynThrPID,byteThrottle_EXPO, byteThrottle_MID, byteSelectSetting;
    float  byteP[10],byteI[10],byteD[10];
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
        MSP_DEBUG                =254,
        MSP_BARO                 =130,  //气压计数据
        MSP_FILTER               =230,   //滤波后的数据
        ADD_FILTER               =231
      ;

};

#endif // U30_H
