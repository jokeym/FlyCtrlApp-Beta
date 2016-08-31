#include "remotectrl.h"
#include "ui_remotectrl.h"

#include "Files/ConfigData.h"
#include <QString>

extern Radio RC;
extern U30_Motor Motor;
extern U30_RC_TUNING Rc_Tuning;

RemoteCtrl::RemoteCtrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteCtrl)
{
    ui->setupUi(this);
//    this->show();
    this->setWindowFlags(Qt::SplashScreen);


    ShowDataTimer = new QTimer(this);
    connect(ShowDataTimer,SIGNAL(timeout()),this,SLOT(ShowData_Slot()));
    ShowDataTimer->start(0);
}

RemoteCtrl::~RemoteCtrl()
{
    delete ShowDataTimer;
    delete ui;

}


void RemoteCtrl::ShowData_Slot()
{
    Show_RC_Value();
    Show_Motor();
    Show_RC_Rate();
}


void RemoteCtrl::Show_RC_Value()
{
    QString value;

    ui->Throt_progressBar->setRange(700,2500);
    ui->Throt_progressBar->setValue(RC.Throt);
    value=QString::number(RC.Throt,10);
    ui->RC_Throt_Value->setText(value);

    ui->Yaw_progressBar->setRange(700,2500);
    ui->Yaw_progressBar->setValue(RC.Yaw);
    value=QString::number(RC.Yaw,10);
    ui->RC_Yaw_Value->setText(value);

    ui->Roll_progressBar->setRange(700,2500);
    ui->Roll_progressBar->setValue(RC.Roll);
    value=QString::number(RC.Roll,10);
    ui->RC_Roll_Value->setText(value);

    ui->Pitch_progressBar->setRange(700,2500);
    ui->Pitch_progressBar->setValue(RC.Pitch);
    value=QString::number(RC.Pitch,10);
    ui->RC_Pitch_Value->setText(value);

    ui->Aux1_progressBar->setRange(700,2500);
    ui->Aux1_progressBar->setValue(RC.Aux_1);
    value=QString::number(RC.Aux_1,10);
    ui->RC_Aux1_Value->setText(value);


    ui->Aux2_progressBar->setRange(700,2500);
    ui->Aux2_progressBar->setValue(RC.Aux_2);
    value=QString::number(RC.Aux_2,10);
    ui->RC_Aux2_Value->setText(value);

    ui->Aux3_progressBar->setRange(700,2500);
    ui->Aux3_progressBar->setValue(RC.Aux_3);
    value=QString::number(RC.Aux_3,10);
    ui->RC_Aux3_Value->setText(value);

    ui->Aux4_progressBar->setRange(700,2500);
    ui->Aux4_progressBar->setValue(RC.Aux_4);   
    value=QString::number(RC.Aux_4,10);
    ui->RC_Aux4_Value->setText(value);


}

void RemoteCtrl::Show_Motor(void)
{
    QString s;
    int i ;
//    for(i=1;i<=4;i++)
//        Motor.mot[i]=1500;

    ui->M_1_progressBar->setOrientation(Qt::Vertical);
    ui->M_1_progressBar->setRange(0,1000);
    ui->M_1_progressBar->setValue(Motor.mot[1]-1000);
    s=QString::number(Motor.mot[1]-1000,'g',6);
    ui->M_1_value->setText(s);

    ui->M_2_progressBar->setOrientation(Qt::Vertical);
    ui->M_2_progressBar->setRange(0,1000);
    ui->M_2_progressBar->setValue(Motor.mot[2]-1000);
    s=QString::number(Motor.mot[2]-1000,'g',6);
    ui->M_2_value->setText(s);

    ui->M_3_progressBar->setOrientation(Qt::Vertical);
    ui->M_3_progressBar->setRange(0,1000);
    ui->M_3_progressBar->setValue(Motor.mot[3]-1000);
    s=QString::number(Motor.mot[3]-1000,'g',6);
    ui->M_3_value->setText(s);

    ui->M_4_progressBar->setOrientation(Qt::Vertical);
    ui->M_4_progressBar->setRange(0,1000);
    ui->M_4_progressBar->setValue(Motor.mot[4]-1000);
    s=QString::number(Motor.mot[4]-1000,'g',6);
    ui->M_4_value->setText(s);


}

void RemoteCtrl::Show_RC_Rate()
{
    QString s;

    s=QString::number(Rc_Tuning.byteRC_RATE/100.0f,'g',6);
    ui->RC_RATE->setText(s);

    s=QString::number(Rc_Tuning.byteRC_EXPO/100.0f,'g',6);
    ui->RC_EXPO->setText(s);

    s=QString::number(Rc_Tuning.byteRollPitchRate/100.0f,'g',6);
    ui->RollPitchRate->setText(s);

    s=QString::number(Rc_Tuning.byteYawRate/100.0f,'g',6);
    ui->YawRate->setText(s);

    s=QString::number(Rc_Tuning.byteDynThrPID/100.0f,'g',6);
    ui->DynThrPID->setText(s);

    s=QString::number(Rc_Tuning.byteThrottle_MID/100.0f,'g',6);
    ui->Throttle_MID->setText(s);

    s=QString::number(Rc_Tuning.byteThrottle_EXPO/100.0f,'g',6);
    ui->Throttle_EXPO->setText(s);

}








