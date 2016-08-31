#include "showpid.h"
#include "ui_showpid.h"
#include "qextserial/qextserialport.h"
#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include <QDockWidget>
#include <QUrl>
#include <QUrlQuery>

extern ReadPID PID;
unsigned char *Data_Ptr;

ShowPID::ShowPID(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowPID)
{
    ui->setupUi(this);

//    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
//    SplashScreen 点击其他地方可以隐藏窗体 主窗体退出时子窗体也会退出
    this->setWindowFlags(Qt::SplashScreen);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

}

ShowPID::~ShowPID()
{

    delete ui;
}

/*-------------------读下位机的PID------------------*/
void ShowPID::Analyse_Rev_PID_Data(unsigned char *data_buf)
{

    int I=0,sum=0,j;
    int _frame_num=0 , _frame_len=0;    //FUN LEN
    int data_num;

    data_num += 800;
    while(I < data_num-4)//遍历接收数据
    {

        if(data_buf[I]==0xaa && data_buf[I+1]==0xaa && data_buf[I+3]<51)
        {
            _frame_num = data_buf[I + 2];
            _frame_len = data_buf[I + 3];
            if ((data_num - I - 5) >= _frame_len) // 数据接收完毕
            {

                for(j=I; j<=I+3+_frame_len; j++)//计算sum
                    sum += data_buf[j];
//                if(sum==data_buf[I+4+_frame_len])//sum校验通过
                if(sum>0)
                {

                    int j;
                    switch(_frame_num)
                    {

                        case 10://传感器数据
                         {
                            j = I + 4;
                            PID.ROLL.P  = (qint16)(data_buf[j + 0]<< 8   | data_buf[j + 1]);
                            PID.ROLL.I  = (qint16)(data_buf[j + 2] << 8  | data_buf[j + 3]);
                            PID.ROLL.D  = (qint16)(data_buf[j + 4] << 8  | data_buf[j + 5]);
                            PID.PITCH.P = (qint16)(data_buf[j + 6] << 8  | data_buf[j + 7]);
                            PID.PITCH.I = (qint16)(data_buf[j + 8] << 8  | data_buf[j + 9]);
                            PID.PITCH.D = (qint16)(data_buf[j + 10] << 8 | data_buf[j + 11]);
                            PID.YAW.P   = (qint16)(data_buf[j + 12] << 8 | data_buf[j + 13]);
                            PID.YAW.I   = (qint16)(data_buf[j + 14] << 8 | data_buf[j + 15]);
                            PID.YAW.D   = (qint16)(data_buf[j + 16] << 8 | data_buf[j + 17]);

                           }
                           break;
                        case 30:
                            {
                                #if 0
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
                                #endif
                            }

                            break;
                        default:
                            break;
                    }
                I = I + 5 + _frame_len; // I指向下一帧数据
                }

                #if 1
                else//sum校验未通过
                {
                    I++;
                }
                #endif
            }

            else//HEAD FUN LEN符合要求,但是数据未接收完毕
            {
                for (j = I; j <= data_num - 1; j++)
                {
                    data_buf[j - I] = data_buf[j];
                }
                data_num = data_num - I;
                return;
            }
        }
        else//HEAD FUN LEN 不符合要求
        {
            I++;
        }
    }
    #if 1
    if (I < data_num) // 剩几字节没有处理完
    {
        for(int j=I; j<=data_num-1; j++)
        {
             data_buf[j-I] = data_buf[j];
        }
        data_num = data_num - I;
    }
    #endif

}

void ShowPID::ShowFrontPID(void)
{

    QString str;
//    AnalysePID_Data(Rev_ptr);

//    PID.PITCH.D=924;
    str=QString::number(PID.ROLL.P,10);
    ui->ROLL_P->setText(str);

    str=QString::number(PID.ROLL.I,10);
    ui->ROLL_I->setText(str);

    str=QString::number(PID.ROLL.D,10);
    ui->ROLL_D->setText(str);

    str=QString::number(PID.PITCH.P,10);
    ui->PITCH_P->setText(str);

    str=QString::number(PID.PITCH.I,10);
    ui->PITCH_I->setText(str);

    str=QString::number(PID.PITCH.D,10);
    ui->PITCH_D->setText(str);

    str=QString::number(PID.YAW.P,10);
    ui->YAW_P->setText(str);

    str=QString::number(PID.YAW.I,10);
    ui->YAW_I->setText(str);

    str=QString::number(PID.YAW.D,10);
    ui->YAW_D->setText(str);

}
void ShowPID::ShowBackPID(void)
{
    QString str;

    str=QString::number(PID.ALT.P,10);
    ui->ALT_P->setText(str);

    str=QString::number(PID.ALT.I,10);
    ui->ALT_I->setText(str);

    str=QString::number(PID.ALT.D,10);
    ui->ALT_D->setText(str);

    str=QString::number(PID.PID1.P,10);
    ui->PID1_P->setText(str);

    str=QString::number(PID.PID1.I,10);
    ui->PID1_I->setText(str);

    str=QString::number(PID.PID1.D,10);
    ui->PID1_D->setText(str);

    str=QString::number(PID.PID2.P,10);
    ui->PID2_P->setText(str);

    str=QString::number(PID.PID2.I,10);
    ui->PID2_I->setText(str);

    str=QString::number(PID.PID2.D,10);
    ui->PID2_D->setText(str);
}

void ShowPID::on_ReadPID_clicked()
{
    ShowFrontPID();
    ShowBackPID();
}
/*-------------------读下位机的PID------------------*/




/*-------------------往下位机发PID------------------*/

unsigned char Send__PID_data[18]={0};
//获取LineEdit的PID数据后转化成int类型
//unsigned char 类型的
//可以正确获取LineEdit的数据
void ShowPID::Analyse_Send_PID_Data(void)
{

    int _cnt=0,i=0;
    bool ok;
    QString PID_Str;
    unsigned char dec_PID_Data;
#if 1
    PID_Str=ui->ROLL_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->ROLL_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data> 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->ROLL_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PITCH_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PITCH_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PITCH_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->YAW_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->YAW_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->YAW_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->ALT_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->ALT_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->ALT_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID1_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID1_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID1_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID2_P->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID2_I->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    PID_Str=ui->PID2_D->text();    //获取lineEdit的数据
    dec_PID_Data=PID_Str.toInt(&ok,10); //转化成10进制的数
    if(dec_PID_Data > 0xff)
        return ;
    Send__PID_data[_cnt++]=dec_PID_Data;

    for(i=0;i<18;i++)
        qDebug()<<"Send__PID_data"<<i<<"="<<Send__PID_data[i];

#endif

}

//实际要发的也只有24字节
u8 data_to_send[30]={0};
int sum=0;
u8 PackPID_Cnt=0;
void ShowPID::PackPID(void)
{

    u8 i;
    int _cnt=0;
    data_to_send[_cnt++]=0xAA;      //封包处理
    data_to_send[_cnt++]=0xAB;
    data_to_send[_cnt++]=0xAC;
    data_to_send[_cnt++]=0;

    for(i=0;i<18;i++)
    {
        data_to_send[_cnt++]=Send__PID_data[i];
    }

    data_to_send[3] = _cnt-4;		//数据校验
    sum=0;
    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    qDebug("sum=%d",sum);
    //校验和、用两个8位数据保存
    data_to_send[_cnt++]=(sum>>8)&0xff;
    data_to_send[_cnt++]=sum&0xff;

    data_to_send[_cnt++]=0x0D;      //末尾添加回车键
    data_to_send[_cnt++]=0x0A;

    PackPID_Cnt=_cnt;

    for(i=0;i<_cnt;i++)
        qDebug()<<"data_to_send"<<i<<"="<<data_to_send[i];

}

u8 Show_PID_Flag=0;
void ShowPID::on_WritePID_clicked()
{

    Analyse_Send_PID_Data();    //获取PID数据
    PackPID();
    Show_PID_Flag=1;


}

/*-------------------往下位机发PID------------------*/









