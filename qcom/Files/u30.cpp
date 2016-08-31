#include "u30.h"
#include "ui_u30.h"
#include <QTableWidget>
#include <QAbstractItemView>
#include <qdatetime.h>
#include "Files/ConfigData.h"
#include <QTimer>
//#include "Files/filter.h"

Sensor Gyro;
Sensor Acc;
Sensor MagSensor;
status Status;
Radio RC;
ReadPID PID;
MyGPS GPS;

U30_BARO U30_Baro;
U30_RC_TUNING Rc_Tuning;
U30_Motor Motor;
U30_IDENT U30_ident;
U18Data U18;


static QString MSP_HEADER = "$M<";

U30::U30(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::U30)
{
    ui->setupUi(this);

//    this->show();

    this->setWindowFlags(Qt::SplashScreen);

    UpdateU30Timer = new QTimer(this);
    connect(UpdateU30Timer,SIGNAL(timeout()),this,SLOT(ShowU30Information()));
    UpdateU30Timer->start(10);


}

U30::~U30()
{
    delete UpdateU30Timer;
    delete ui;
}

static int ShowU30Nums=0;
//  24 4D 3C 00 70 70
void U30::ShowU30Information(void)
{
    ShowU30Nums++;

    infoTable_1();
    infoTable_2();

//    U30_Baro.Add_AltHold=Filter::MoveAddFilter( U30_Baro.Original_Alt);


}


/************************
 * AnalyseU30Data()
 * 解析U30发过来的数据
 *
*************************/


#define Bufer_Size 800

static int PIDITEMS=10;
static int CHECKBOXITEMS=0;


//解析串口数据
void U30::AnalyseU30Data(unsigned char *data_buf)
{
    int I=0,sum=120,i;
    int j=0;
    int nums;
    int _frame_Fun=0 , _frame_len=0;
    int n=0,data_num=0;
    n=Bufer_Size;
    QString s;
    if(n>20000)
        return ;
    data_num += n;
    while(I < data_num-4)//??????
    {
        //解析飞控数据
        if(data_buf[I]==0x24 && data_buf[I+1]==0x4D && data_buf[I+2]==0x3E )
        {
            _frame_len = data_buf[I + 3];
            _frame_Fun = data_buf[I + 4];

            if ((data_num - I - 5) >= _frame_len) // ??????
            {

//                for(j=I; j<=I+3+_frame_len; j++)//??sum
//                    sum += data_buf[j];
//                if(sum==data_buf[I+4+_frame_len])//sum????
                if(sum>0)
                {
                    // data_buf[j++] & 0Xff
                    // ( data_buf[j++]  | data_buf[j++]<< 8)
                    //  (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24);
                  switch(_frame_Fun)
                  {

                    case MSP_IDENT: //OK
                     {
                        j=I+5;
                        U30_ident.version    = data_buf[j++] & 0Xff;
                        U30_ident.multiType  = data_buf[j++] & 0Xff;
                        U30_ident.MSP_version=data_buf[j++] & 0Xff; // MSP version
                        // capability
                        multiCapability =( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) );
//                        qDebug()<<"解析的version:"<<U30_ident.version;  //182
//                        qDebug()<<"解析的multiCapability:"<<multiCapability;  //3

//                        infoTable_1();
                      }
                        break;
                    case MSP_STATUS:
                     {
                       j=I+5;
                       U30_ident.cycleTime = ( data_buf[j++]  | data_buf[j++]<< 8);
                       U30_ident.i2cError  = ( data_buf[j++]  | data_buf[j++]<< 8);
                       present             = ( data_buf[j++]  | data_buf[j++]<< 8);
                       U30_ident.mode      = ( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) );
//                       qDebug()<<"解析出的 cycleTime:"<<U30_ident.cycleTime;  //
//                       qDebug()<<"解析出的 i2cError:"<<U30_ident.i2cError;  //

                     }
                        break;
                    case MSP_RAW_IMU://ok
                     {
                        j = I + 5;
                        Acc.X  = ( data_buf[j++]  | data_buf[j++]<< 8);
                        Acc.Y  = ( data_buf[j++]  | data_buf[j++]<< 8 );
                        Acc.Z  = ( data_buf[j++]  | data_buf[j++]<< 8);
                        Gyro.X = ( (data_buf[j++] | data_buf[j++]<< 8)/8);
                        Gyro.Y = ( (data_buf[j++] | data_buf[j++]<< 8)/8);
                        Gyro.Z = ( (data_buf[j++] | data_buf[j++]<< 8)/8);
                        MagSensor.X  = ( (data_buf[j++] | data_buf[j++]<< 8)/3);
                        MagSensor.Y  = ( (data_buf[j++] | data_buf[j++]<< 8)/3);
                        MagSensor.Z  = ( (data_buf[j++] | data_buf[j++]<< 8)/3);

//                        qDebug()<<"Gyro.X"<<Gyro.X;
//                        qDebug()<<"Gyro.Y"<<Gyro.Y;
//                        qDebug()<<"Gyro.Z"<<Gyro.Z;
                      }
                        break;
                    case MSP_SERVO:
                    {
                      j=I+5;
                      for(i=0;i<8;i++)
                          servo[i] = ( data_buf[j++]  | data_buf[j++]<< 8);
                    }
                        break;
                    case MSP_MOTOR:
                    {
                         j=I+5;
                         for(i=1;i<=8;i++)
                         {
                             mot[i] =( data_buf[j++]  | data_buf[j++]<< 8);
                             Motor.mot[i]=mot[i];
                         }
                    }
                        break;
                    case MSP_RC:
                   {
                      j = I + 5;
                      RC.Roll  = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Pitch = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Yaw   = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Throt = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);

                      RC.Aux_1 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Aux_2 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Aux_3 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
                      RC.Aux_4 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
//                      RC.Aux_5 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);
//                      RC.Aux_6 = (qint16)( data_buf[j++]  | data_buf[j++]<< 8);

                    }
                      break;
                    case MSP_RAW_GPS:
                    {
                      j=I+5;
                      GPS.fix       = ( data_buf[j++] & 0xff );
                      GPS.numSat    = ( data_buf[j++] & 0xff );
                      GPS.latitude  = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24);
                      GPS.longitude = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24);
                      GPS.altitude  = ( data_buf[j++] | data_buf[j++]<< 8 );
                      GPS.speed     = ( data_buf[j++] | data_buf[j++]<< 8 );
                     }
                      break;
                    case MSP_COMP_GPS:
                    {
                      j=I+5;
                      GPS.distanceToHome  = ( data_buf[j++] | data_buf[j++]<< 8 );
                      GPS.directionToHome = ( data_buf[j++] & 0xff );
                      GPS.update = ( data_buf[j++] & 0xff );
                    }
                      break;
                    case MSP_ATTITUDE:
                    {
                      j=I+5;
                      U30_ident.angx = ( data_buf[j++] & 0xff )/10;
                      U30_ident.angy = ( data_buf[j++] & 0xff )/10;
                      U30_ident.head = ( data_buf[j++] & 0xff );
//                      qDebug()<<"解析出的 angx :"<<U30_ident.angx;
                    }
                      break;
                    case MSP_ALTITUDE:  //经过气压计、GPS计算得出的
                    {
                      j=I+5;
                      alt = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24);

//                      qDebug()<<" alt:"<<alt;

                      U30_Baro.alt=alt;
                      U30_ident.alt=alt;
//                      qDebug()<<"Analysed U30_Baro.alt:"<<U30_Baro.alt;
                    }
                      break;
                    case MSP_BAT:
                    {
                      j=I+5;
                      U30_ident.bytevbat  = ( data_buf[j++] & 0xff);
                      pMeterSum = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8) ;
                    }
                      break;
                    case MSP_RC_TUNING:
                    {
                      j=I+5;
                      Rc_Tuning.byteRC_RATE        = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteRC_EXPO        = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteRollPitchRate  = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteYawRate        = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteDynThrPID      = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteThrottle_MID   = ( data_buf[j++] & 0xff);
                      Rc_Tuning.byteThrottle_EXPO  = ( data_buf[j++] & 0xff);
//                      qDebug()<<"byteRC_RATE"<<byteRC_RATE;

                    }
                      break;
                    case MSP_PID:
                     {
                       j=I+5;
                       for(i=0;i<PIDITEMS;i++)
                       {
                            byteP[i] = ( data_buf[j++] & 0xff);
                            byteI[i] = ( data_buf[j++] & 0xff);
                            byteD[i] = ( data_buf[j++] & 0xff);
//                            qDebug()<<"Analyse byteP[9]"<<byteP[9];

//                            qDebug()<<"ANALYSE byteD"<<i<<":"<<byteD[i];
//                            ui->infoTable_2->show();
//                            infoTable_2();

                       }
                     }
                      break;
                    case MSP_BOX:
                    {
                      j=I+5;
                      for( i=0;i<CHECKBOXITEMS;i++)
                      {
                        activation[i] = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);
                      }
                    }
                      break;
                    case MSP_MISC:
                    {
                      j=I+5;
                      U30_ident.intPowerTrigger = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);
//                      qDebug()<<"intPowerTrigger"<<intPowerTrigger;

                    }
                      break;
                    case MSP_MOTOR_PINS:
                    {
                      j=I+5;
                      for( i=0;i<8;i++)
                      {
                        byteMP[i] = ( data_buf[j++] & 0xff);
                      }
                    }
                      break;
                    case MSP_DEBUGMSG:
                   {
                      j=I+5;
                      while(dataSize-- > 0)
                      {
                        char DebugMsg = (char)( data_buf[j++] & 0xff);
//                        qDebug()<<"DebugMsg"<<DebugMsg;

                        if(DebugMsg!=0)
//                            qDebug("DebugMsg:%c",DebugMsg);
                            ;
                      }
                    }
                      break;
                    case MSP_DEBUG:
                    {
                      j=I+5;
                      debug1 = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);
                      debug2 = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);
                      debug3 = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);
                      debug4 = ( data_buf[j++] & 0xff) | ((data_buf[j++]&0xff)<<8);

//                      qDebug()<<"解析出的debug1"<<debug1;
//                      qDebug()<<"       debug2"<<debug2;
//                      qDebug()<<"       debug3"<<debug4;

                    }
                      break;

                  case MSP_BARO:    //气压计高度
                  {
                      j=I+5;
//                     MSP_Baro_Num++;
                     U30_Baro.Original_Alt= ( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) );
                     U30_Baro.Pressure    = ( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) );
                     U30_Baro.Temperature = ( data_buf[j++]  | data_buf[j++]<< 8)/100.0f;

//                       qDebug()<<"Baro Original_Alt :"<<U30_Baro.Original_Alt; //354752

//                       qDebug()<<"Pressure :"<<U30_Baro.Pressure<<"Pa";       //9706
//                       qDebug()<<"Temperature :"<<U30_Baro.Temperature<<"°C"; //1380
//                       qDebug()<<"MSP_Baro_Num :"<<MSP_Baro_Num;

                   }
                     break;

                  case MSP_FILTER :
                   {
                      j=I+5;

//                      MSP_Baro_Num++;
                      U30_Baro.EKF_AltHold = ( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) ) ;

//                       qDebug()<<"EKF_AltHold :"<<U30_Baro.EKF_AltHold; //450

//                        qDebug()<<"MSP_Baro_Num :"<<MSP_Baro_Num;

                     }
                      break;

                  case ADD_FILTER:
                  {
                      j=I+5;
                      U30_Baro.Add_AltHold = ( (data_buf[j++]&0xff) | ((data_buf[j++]&0xff)<<8) | ((data_buf[j++]&0xff)<<16) | ((data_buf[j++]&0xff)<<24) ) ;

//                      qDebug()<<"Add_AltHold :"<<U30_Baro.Add_AltHold; //1553

                  }

                      break;

                   default:
                      break;
                  }
                    I = I + 5 + _frame_len; // I???????
                }

                #if 1
                else//sum?????
                {
                    I++;
                }
                #endif
            }

            else//HEAD FUN LEN????,?????????
            {
                for (j = I; j <= data_num - 1; j++)
                {
                    data_buf[j - I] = data_buf[j];
                }
                data_num = data_num - I;
                return;
            }
        }

        //解析遥控数据
        if(data_buf[I]==0x21 && data_buf[I+1]==0x22)
        {
            _frame_Fun = data_buf[I+1];

            if ((data_num - I - 5) >= _frame_len) // ??????
            {

                if(sum>0)
                {
                  switch(_frame_Fun)
                  {

                    case 0x22: //OK
                    {
                          j=I+2;
                          U18.FlyType = data_buf[j++] &0xff;   //类型
//                          qDebug("FlyType : %x",U18.FlyType);
                          U18.Bat     = data_buf[j++] &0xff;   //电压
//                          qDebug("Bat : %x",U18.Bat);

                          U18.FlyMode = data_buf[j++] &0xff;   //飞行模式
//                          qDebug("FlyMode : %x",U18.FlyMode);

                          U18.Sensors = data_buf[j++] &0xff;   //传感器状态

                          U18.Throttle= ( data_buf[j++] &0xff )<<3 ;  //油门
//                          qDebug("Throttle : %d",U18.Throttle);     //1509

                          U18.Roll    = ( data_buf[j++] &0xff )<<3 ;

                          U18.Temperature= ( data_buf[j++]<<8  | data_buf[j++] );
                          qDebug("Temperature : %f °C ",U18.Temperature/100.0f);   //2736

                          U18.Baro_High  = ( data_buf[j++]<<8  | data_buf[j++] );
                          qDebug("Baro_High : %d cm",U18.Baro_High);   //12586


                          U18.CheckSum1  =  data_buf[j++] &0xff;

                          U18.High_HoldStar=( data_buf[j++]<<8  | data_buf[j++] );
//                          qDebug("High_HoldStar : %d",U18.High_HoldStar);

                          U18.High_Current =( data_buf[j++]<<8  | data_buf[j++] );
//                          qDebug("High_Current : %d",U18.High_Current);

                          U18.Thorttle_HoldStar = ( data_buf[j++]<<8  | data_buf[j++] );
                          U18.Throttle_Current  = ( data_buf[j++]<<8  | data_buf[j++] );
//                          qDebug("Throttle_Current : %d",U18.Throttle_Current);


                          U18.CheckSum2 = data_buf[j++] &0xff;
                      }
                      break;


                   default:
                      break;
                  }

                   I = I + 5 + _frame_len; // I???????
                }

                #if 1
                else//sum?????
                {
                    I++;
                }
                #endif
            }

            else//HEAD FUN LEN????,?????????
            {
                for (j = I; j <= data_num - 1; j++)
                {
                    data_buf[j - I] = data_buf[j];
                }
                data_num = data_num - I;
                return;
            }
        }

        else//HEAD FUN LEN ?????
        {
            I++;
        }
    }
    #if 1
    if (I < data_num) // ?????????
    {
        for(int j=I; j<=data_num-1; j++)
        {
             data_buf[j-I] = data_buf[j];
        }
        data_num = data_num - I;
    }
    #endif

}

bool ok;
void U30::infoTable_1()
{

    ui->infoTable_1->setRowCount(8);
    ui->infoTable_1->setColumnCount(4);
    ui->infoTable_1->resize(520,800);

    ui->infoTable_1->setFont(QFont("wenquanyi",12,3));//设置字体
    ui->infoTable_1->horizontalHeader()->setStretchLastSection(ok); //自适应宽度
    ui->infoTable_1->resizeColumnsToContents();
    ui->infoTable_1->resizeRowsToContents();

    ui->infoTable_1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:red;}");    //设置表头颜色
    ui->infoTable_1->verticalHeader()->setStyleSheet("QHeaderView::section{background:darkGray;}");
    ui->infoTable_1->setAlternatingRowColors(1); //隔行显示颜色

//    QStringList headerList;
//    headerList<<"Version"<<"multiType"<<"cycleTime"<<"i2cError"<<"present"<<"mode";
//    ui->infoTable_1->setHorizontalHeaderLabels(headerList);

//    qDebug()<<"In headerList";

    QString s;

//    qDebug()<<"before Ver:"<<version;
    s=QString::number(U30_ident.version,10);
    ui->infoTable_1->setItem(0,0,new QTableWidgetItem("version") );
    ui->infoTable_1->setItem(0,1,new QTableWidgetItem(s) );
//    qDebug()<<"After Ver:"<<version;


//    s=QString::number(U30_ident.multiType,10);
    ui->infoTable_1->setItem(0,2,new QTableWidgetItem("multiType") );
    if(U30_ident.multiType==1)
        ui->infoTable_1->setItem(0,3,new QTableWidgetItem("tricopter") );
    if(U30_ident.multiType==2)
        ui->infoTable_1->setItem(0,3,new QTableWidgetItem("quad_+") );
    if(U30_ident.multiType==3)
        ui->infoTable_1->setItem(0,3,new QTableWidgetItem("quad_X") );



    s=QString::number(U30_ident.MSP_version,10);
    ui->infoTable_1->setItem(1,0,new QTableWidgetItem("MSP_version") );
    ui->infoTable_1->setItem(1,1,new QTableWidgetItem(s) );
    s=QString::number(multiCapability,10);
    ui->infoTable_1->setItem(1,2,new QTableWidgetItem("multiCapability") );
    ui->infoTable_1->setItem(1,3,new QTableWidgetItem(s) );

    s=QString::number(U30_ident.cycleTime,10);
//    qDebug()<<"Now Table1 U30_ident.cycleTime :"<<U30_ident.cycleTime;
    ui->infoTable_1->setItem(2,0,new QTableWidgetItem("cycleTime") );
    ui->infoTable_1->setItem(2,1,new QTableWidgetItem(s) );
    s=QString::number(U30_ident.i2cError,10);
    ui->infoTable_1->setItem(2,2,new QTableWidgetItem("i2cError") );
    ui->infoTable_1->setItem(2,3,new QTableWidgetItem(s) );
    s=QString::number(present,10);
    ui->infoTable_1->setItem(3,0,new QTableWidgetItem("present") );
    ui->infoTable_1->setItem(3,1,new QTableWidgetItem(s) );
    s=QString::number(U30_ident.mode,10);
    ui->infoTable_1->setItem(3,2,new QTableWidgetItem("mode") );
    ui->infoTable_1->setItem(3,3,new QTableWidgetItem(s) );

    s=QString::number(U30_ident.intPowerTrigger,10);
    ui->infoTable_1->setItem(4,0,new QTableWidgetItem("intPowerTrigger") );
    ui->infoTable_1->setItem(4,1,new QTableWidgetItem(s) );
    s=QString::number(U30_ident.bytevbat,10);
    ui->infoTable_1->setItem(4,2,new QTableWidgetItem("bytevbat") );
    ui->infoTable_1->setItem(4,3,new QTableWidgetItem(s) );

    s=QString::number(pMeterSum,10);
    ui->infoTable_1->setItem(5,0,new QTableWidgetItem("pMeterSum") );
    ui->infoTable_1->setItem(5,1,new QTableWidgetItem(s) );
    s=QString::number(U30_ident.alt/100.0f ,'g',6);
    ui->infoTable_1->setItem(5,2,new QTableWidgetItem("U30 Alt") );
    ui->infoTable_1->setItem(5,3,new QTableWidgetItem(s) );

    s=QString::number(debug1,'g',6);
    ui->infoTable_1->setItem(6,0,new QTableWidgetItem("debug1") );
    ui->infoTable_1->setItem(6,1,new QTableWidgetItem(s) );
    s=QString::number(debug2,'g',6);
    ui->infoTable_1->setItem(6,2,new QTableWidgetItem("debug2") );
    ui->infoTable_1->setItem(6,3,new QTableWidgetItem(s) );
    s=QString::number(debug3,'g',6);
    ui->infoTable_1->setItem(7,0,new QTableWidgetItem("debug3") );
    ui->infoTable_1->setItem(7,1,new QTableWidgetItem(s) );
    s=QString::number(debug4,'g',6);
    ui->infoTable_1->setItem(7,2,new QTableWidgetItem("debug4") );
    ui->infoTable_1->setItem(7,3,new QTableWidgetItem(s) );
//    qDebug()<<"Show debug1 :"<<debug1;

}

void U30::infoTable_2()
{
    int i;

    ui->infoTable_2->setRowCount(10);
    ui->infoTable_2->setColumnCount(3);
    ui->infoTable_2->resize(300,500);

    ui->infoTable_2->setFont(QFont("wenquanyi",12,3));//设置字体
    ui->infoTable_2->horizontalHeader()->setStretchLastSection(ok); //自适应宽度
    ui->infoTable_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background:red;}");    //设置表头颜色
    ui->infoTable_2->verticalHeader()->setStyleSheet("QHeaderView::section{background:darkGray;}");
    ui->infoTable_2->setAlternatingRowColors(1); //隔行显示颜色

    QStringList headerList;
    headerList<<"P"<<"I"<<"D";
    ui->infoTable_2->setHorizontalHeaderLabels(headerList);

    QString s;

    for(i=0;i<=3;i++)
    {
        s=QString::number(byteP[i]/10.0f,'g',6);
        ui->infoTable_2->setItem(i,0,new QTableWidgetItem(s) );

        s=QString::number(byteI[i]/100.0f,'g',6);
        ui->infoTable_2->setItem(i,1,new QTableWidgetItem(s) );

        s=QString::number(byteD[i],'g',6);
        ui->infoTable_2->setItem(i,2,new QTableWidgetItem(s) );

//        qDebug()<<"Table 2 Now byteI"<<i<<":"<<byteI[i];
    }

    for(i=7;i<=9;i++)
    {

        s=QString::number(byteP[i]/10.0f,'g',6);
        ui->infoTable_2->setItem(i,0,new QTableWidgetItem(s) );
        s=QString::number(byteI[i]/1000.0f,'g',6);
        ui->infoTable_2->setItem(i,1,new QTableWidgetItem(s) );
        s=QString::number(byteD[i],'g',6);
        ui->infoTable_2->setItem(i,2,new QTableWidgetItem(s) );

        if(i==8)
        {
            byteI[i]=0;
            byteD[i]=0;

            s=QString::number(byteI[i],'g',6);
            ui->infoTable_2->setItem(i,1,new QTableWidgetItem(s) );

            s=QString::number(byteD[i],'g',6);
            ui->infoTable_2->setItem(i,2,new QTableWidgetItem(s) );
        }
    }

    for(i=4;i<=6;i++)
    {

        s=QString::number(byteP[i]/100.0f,'g',6);
        ui->infoTable_2->setItem(i,0,new QTableWidgetItem(s) );

        s=QString::number(byteI[i]/100.0f,'g',6);
        ui->infoTable_2->setItem(i,1,new QTableWidgetItem(s) );

        s=QString::number(byteD[i]/1000.0f,'g',6);
        ui->infoTable_2->setItem(i,2,new QTableWidgetItem(s) );

        if(i==5 || i==6)
        {
            s=QString::number(byteP[i]/10.0f,'g',6);
            ui->infoTable_2->setItem(i,0,new QTableWidgetItem(s) );
        }
    }

//    qDebug()<<"ShowU30Nums:"<<ShowU30Nums;
//    for(i=0;i<10;i++)
//          qDebug()<<"Now byteD"<<i<<":"<<byteD[i];

}
