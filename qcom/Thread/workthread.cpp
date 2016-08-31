#include "workthread.h"
#include <QtDebug>

#include "Files/ConfigData.h"
#include "Files/showsensordata.h"
#include "Files/showpid.h"
#include "Files/showwaveform.h"

#define Bufer_Size 800

extern Sensor Gyro;
extern Sensor Acc;
extern Sensor MagSensor;
extern status Status;
extern Radio RC;

extern ReadPID PID;

extern mySerialData SerialData;

//extern unsigned char *ReciveData;

WorkThread::WorkThread()
{
}

void WorkThread::run()
{
    while(true)
    {
//        AnalyseData(SerialData.RevData);
//        qDebug()<<"Thread Data :"<<SerialData.RevData;
//        qDebug("Data %x",SerialData.RevData);
//        qDebug()<<"Thread Data";

//        qDebug()<<"ReciveData "<<ReciveData;

        ShowPIDThread();
        ShowWaveThread();
        showSensorDataThread();


    }
}

void WorkThread::ShowPIDThread()
{
//    showpid.show();
    qDebug("Show PID Thread");
}

void WorkThread::ShowWaveThread()
{
//    showsensorwave.show();
    qDebug("Show Wave Thread");

}

void WorkThread::showSensorDataThread()
{
//    Showsensordata.show();
    qDebug("Show Data Thread");

}


void WorkThread::AnalyseData(unsigned char *data_buf)
{
    int I=0,sum=0,j;
    int nums;
    int data_num, n;
    int _frame_num=0 , _frame_len=0;
//    n=data_buf.length();
//    n=sizeof(*data_buf);
    n=Bufer_Size;
    QString s;
    if(n>20000)
        return ;
    data_num += n;
    while(I < data_num-4)//??????
    {

        if(data_buf[I]==0xaa && data_buf[I+1]==0xaa && data_buf[I+3]<51)
        {
            _frame_num = data_buf[I + 2];
            _frame_len = data_buf[I + 3];
            if ((data_num - I - 5) >= _frame_len) // ??????
            {

                for(j=I; j<=I+3+_frame_len; j++)//??sum
                    sum += data_buf[j];
//                if(sum==data_buf[I+4+_frame_len])//sum????
                if(sum>0)
                {

                    int j;
                    switch(_frame_num)
                    {
                        case 1://STATUS
                          {
                            j = I + 4;
                            Status.Roll = (float)((qint16)(data_buf[j] << 8 | data_buf[j + 1]))/100;
                            Status.Pitch = (float)((qint16)(data_buf[j + 2] << 8 | data_buf[j + 3]))/100;
                            Status.Yaw = (float)((qint16)(data_buf[j + 4] << 8 | data_buf[j + 5]))/100;
//                            D_alt_csb = (Int16)(data_buf[j + 6] << 8 | data_buf[j + 7]);
//                            D_alt_prs = (Int32)(data_buf[j + 8] << 24 | data_buf[j + 9] << 16 | data_buf[j + 10] << 8 | data_buf[j + 11]);

//                            Show_Status_Data();
                            }
                            break;
                        case 2://?????
                         {
                            j = I + 4;

                            Acc.X  = (qint16)(data_buf[j + 0]<< 8   | data_buf[j + 1]);
//                            qDebug("Acc.x=%d",Acc.X);
                            Acc.Y  = (qint16)(data_buf[j + 2] << 8  | data_buf[j + 3]);
//                            qDebug("Acc.Y=%d",Acc.Y);
                            Acc.Z  = (qint16)(data_buf[j + 4] << 8  | data_buf[j + 5]);
//                            qDebug("Acc.Z=%d",Acc.Z);
                            Gyro.X = (qint16)(data_buf[j + 6] << 8  | data_buf[j + 7]);
                            Gyro.Y = (qint16)(data_buf[j + 8] << 8  | data_buf[j + 9]);
                            Gyro.Z = (qint16)(data_buf[j + 10] << 8 | data_buf[j + 11]);
                            MagSensor.X  = (qint16)(data_buf[j + 12] << 8 | data_buf[j + 13]);
                            MagSensor.Y  = (qint16)(data_buf[j + 14] << 8 | data_buf[j + 15]);
                            MagSensor.Z  = (qint16)(data_buf[j + 16] << 8 | data_buf[j + 17]);

//                           Show_Sensor_Data();
                           }
                           break;
                        case 3:
                            {
                                j = I + 4;
                                RC.Throt = (qint16)(data_buf[j + 0] << 8 | data_buf[j + 1]);
                                RC.Yaw   = (qint16)(data_buf[j + 2] << 8 | data_buf[j + 3]);
                                RC.Roll  = (qint16)(data_buf[j + 4] << 8 | data_buf[j + 5]);
                                RC.Pitch = (qint16)(data_buf[j + 6] << 8 | data_buf[j + 7]);
                                RC.Aux_1 = (qint16)(data_buf[j + 8] << 8 | data_buf[j + 9]);
                                RC.Aux_2 = (qint16)(data_buf[j + 10] << 8 | data_buf[j + 11]);
                                RC.Aux_3 = (qint16)(data_buf[j + 12] << 8 | data_buf[j + 13]);
                                RC.Aux_4 = (qint16)(data_buf[j + 14] << 8 | data_buf[j + 15]);
                                RC.Aux_5 = (qint16)(data_buf[j + 16] << 8 | data_buf[j + 17]);
                                RC.Aux_6 = (qint16)(data_buf[j + 18] << 8 | data_buf[j + 19]);

                            }
                        break;

                         case 4:
                         {
                                j = I + 4;

                                PID.ROLL.P  = (data_buf[j + 0] << 8 | data_buf[j + 1])/100;
                                PID.ROLL.I  = (data_buf[j + 2] << 8 | data_buf[j + 3])/100;
                                PID.ROLL.D  = (data_buf[j + 4] << 8 | data_buf[j + 5])/100;
                                PID.PITCH.P = (data_buf[j + 6] << 8 | data_buf[j + 7])/100;
                                PID.PITCH.I = (data_buf[j + 8] << 8 | data_buf[j + 9])/100;
                                PID.PITCH.D = (data_buf[j + 10] << 8 | data_buf[j + 11])/100;
                                PID.YAW.P   = (data_buf[j + 12] << 8 | data_buf[j + 13])/100;
                                PID.YAW.I   = (data_buf[j + 14] << 8 | data_buf[j + 15])/100;
                                PID.YAW.D   = (data_buf[j + 16] << 8 | data_buf[j + 17])/100;
                          }
                          break;

                    case 5:
                    {
                        j = I + 4;

                        PID.ALT.P  = (data_buf[j + 0] << 8 | data_buf[j + 1])/100;
                        PID.ALT.I  = (data_buf[j + 2] << 8 | data_buf[j + 3])/100;
                        PID.ALT.D  = (data_buf[j + 4] << 8 | data_buf[j + 5])/100;
                        PID.PID1.P = (data_buf[j + 6] << 8 | data_buf[j + 7])/100;
                        PID.PID1.I = (data_buf[j + 8] << 8 | data_buf[j + 9])/100;
                        PID.PID1.D = (data_buf[j + 10] << 8 | data_buf[j + 11])/100;
                        PID.PID2.P = (data_buf[j + 12] << 8 | data_buf[j + 13])/100;
                        PID.PID2.I = (data_buf[j + 14] << 8 | data_buf[j + 15])/100;
                        PID.PID2.D = (data_buf[j + 16] << 8 | data_buf[j + 17])/100;
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





