#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include "mainwindow.h"
#define u8 unsigned char

typedef struct {
    qint16 X;
    qint16 Y;
    qint16 Z;
}Sensor;

typedef struct {
    float Roll;
    float Pitch;
    float Yaw;
}status;


typedef struct{
    qint16 Throt;
    qint16 Roll;
    qint16 Pitch;
    qint16 Yaw;
    qint16 Aux_1;
    qint16 Aux_2;
    qint16 Aux_3;
    qint16 Aux_4;
    qint16 Aux_5;
    qint16 Aux_6;
    qint16 Aux_7;
    qint16 Aux_8;

}Radio;


typedef struct{     //原先是int类型的 后来改成float类型
    float byteRC_RATE;
    float byteRC_EXPO;
    float byteRollPitchRate;
    float byteYawRate;
    float byteDynThrPID;
    float byteThrottle_EXPO;
    float byteThrottle_MID;

}U30_RC_TUNING;

typedef struct {
    int x;
    int y;
    int Width;
    int Height;

}Coordinate;


typedef struct {
    int ShowPID;
    int ShowWave;
    int ShowSensorData;
}SensorFlag;

typedef struct{
    unsigned char *RevData;
}mySerialData;


typedef struct {
    int fix;
    int numSat;
    int distanceToHome;
    int directionToHome;
    int update;
    int altitude;
    int speed;
    int latitude;
    int longitude;
}MyGPS;

typedef struct{
    float alt;
    float Pressure;
    float Temperature;

    float Original_Alt;
    float EKF_AltHold;
    float Add_AltHold;

}U30_BARO;

typedef struct{
    float mot[9];
}U30_Motor;


typedef struct {
    int version;
    int MSP_version;
    int multiType;
    int cycleTime;
    int i2cError;
    int mode;
    int intPowerTrigger;
    int bytevbat;

    float alt;
    float head;
    float angx;
    float angy;

    unsigned char *Rve_Ptr;
    int Rev_Flag;
    int Rev_Size;

}U30_IDENT;


typedef struct {
    unsigned char FlyType;     //飞控类型 U30
    unsigned char Bat;         //电池电压
    unsigned char FlyMode;     //飞行模式
    unsigned char Sensors;     //传感器
    qint16 Throttle;//油门
    qint16 Roll;
    qint16 Pitch;
    qint16 Yaw;

    float Temperature; //气压计温度
    qint16 Baro_High;   //气压计高度

    qint16 CheckSum1;   //6~14的校验和
    qint16 CheckSum2;   //15~23的校验和

    qint16 High_HoldStar ;      //定高时的高度
    qint16 High_Current;        //当前高度
    qint16 Thorttle_HoldStar;   //定高时的油门
    qint16 Throttle_Current;    //当前高度



    u8 Heading;
    u8 magHold;
    struct angle;

}U18Data;









#endif // CONFIGDATA_H
