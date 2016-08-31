#include <QDebug>
#include "aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Files/ConfigData.h"
#include <QString>
#include <QLineEdit>
#include <QProgressBar>
#include <QProcess>
#include <QTimer>
#include "Files/flycontrol.h"
#include <QColor>
#include "Thread/workthread.h"

#define Bufer_Size 800
static int Send_Nums=0;
Coordinate Coord;
SensorFlag ShowFlag;

extern U30_IDENT U30_ident;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startInit();
}

MainWindow::~MainWindow()
{

    delete Send_Data_Timer;
    delete GetCoordTimer;
    delete AnalyseU30Timer;
    delete ui;


}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::startInit()
{

    //Send  Recive Data Timer
    Send_Data_Timer = new QTimer(this);
    connect(Send_Data_Timer, SIGNAL(timeout()), this, SLOT(Show_Rev_Data()));
    Send_Data_Timer->start(5);
//    connect(Send_Data_Timer, SIGNAL(timeout()), this, SLOT(FlyStatus::RotatePic()));

    GetCoordTimer = new QTimer(this);
    connect(GetCoordTimer,SIGNAL(timeout()),this,SLOT(RealTimeGetCoord()));
    GetCoordTimer->start(100);

    AnalyseU30Timer = new QTimer(this);
    connect(AnalyseU30Timer,SIGNAL(timeout()),this,SLOT(AnalyseU30Slot()));
    AnalyseU30Timer->start(6);

}

void MainWindow::AnalyseU30Slot()
{
    if(U30_ident.Rev_Flag )
    {
//        qDebug()<<"Rev_Flag :"<<U30_ident.Rev_Flag;

        u30.AnalyseU30Data(U30_ident.Rve_Ptr);

         U30_ident.Rev_Flag=0;
    }
    else
        ;

}

void MainWindow::RealTimeGetCoord(void)
{
    Coord.x=this->pos().rx();
    Coord.y=this->pos().ry();
    Coord.Width=this->width();
    Coord.Height=this->height();

    myShowsensor.move(Coord.x+8,Coord.y+110);
    myShowsensor.resize(Coord.Width,Coord.Height-110);

    ShowSensorWave.move(Coord.x+8,Coord.y+110);
    ShowSensorWave.resize(Coord.Width,Coord.Height-110);

    showpid.move(Coord.x+8,Coord.y+110);
    showpid.resize(Coord.Width,Coord.Height-110);

    serialset.move(Coord.x+8,Coord.y+110);
    serialset.resize(Coord.Width,Coord.Height-110);

    u30.move(Coord.x+8,Coord.y+110);
    u30.resize(Coord.Width,Coord.Height-110);

    gyroscope.move(Coord.x+8,Coord.y+110);
    gyroscope.resize(Coord.Width,Coord.Height-110);

    baro.move(Coord.x+8,Coord.y+110);
    baro.resize(Coord.Width,Coord.Height-110);

    mag.move(Coord.x+8,Coord.y+110);
    mag.resize(Coord.Width,Coord.Height-110);

    remotectrl.move(Coord.x+8,Coord.y+110);
    remotectrl.resize(Coord.Width,Coord.Height-110);

    Filter.move(Coord.x+8,Coord.y+110);
    Filter.resize(Coord.Width,Coord.Height-110);

    ua18.move(Coord.x+8,Coord.y+110);
    ua18.resize(Coord.Width,Coord.Height-110);

}

void MainWindow::Show_Rev_Data(void)
{

    ShowU30_info();
    ShowU30_Motor();

}


//Sensor Gyro;
//Sensor Acc;
//Sensor MagSensor;
//status Status;
//Radio RC;
//ReadPID PID;
//MyGPS GPS;

unsigned char *Rev_U30_Data=0;

extern U30_Motor Motor;
extern U30_IDENT U30_ident;

bool B_armed = true;

static int PIDITEMS=10;
static int CHECKBOXITEMS=0;


void MainWindow::ShowU30_info()
{
    QString s;

    s=QString::number(U30_ident.version,10);
    ui->Version->setText(s);

//    qDebug()<<"Main Function U30_ident.version :"<<U30_ident.version;

    s=QString::number(U30_ident.multiType,10);
    ui->multiType->setText(s);

    s=QString::number(U30_ident.intPowerTrigger,10);
    ui->InputPower->setText(s);

    s=QString::number(U30_ident.bytevbat,10);
    ui->Bat->setText(s);


}

void MainWindow::ShowU30_Motor()
{

    QString s;
//    int i ;
//    for(i=1;i<=4;i++)
//        Motor.mot[i]=1200;

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

/********************************显示收到的下位机数据****************************************/

/********************************显示收到的下位机数据****************************************/

//关于
void MainWindow::on_actionAbout_triggered()
{
    aboutdlg.show();
    // ????????
    int x =this->x() + (this->width() - aboutdlg.width()) / 2;
    int y =this->y() + (this->height() - aboutdlg.height()) / 2;
    aboutdlg.move(x, y);
    ui->statusBar->showMessage(tr("??Wincom"));
}

/****************************************************************
 *
 * 事件按钮函数
 *
****************************************************************/

void MainWindow::on_actionPID_triggered()
{
    showpid.show();
    myShowsensor.close();
    ShowSensorWave.close();
    serialset.close();
    u30.close();
    gyroscope.close();
    Filter.close();
    ua18.close();

}

void MainWindow::on_actionCurve_triggered()
{
#if 0
    ShowSensorWave.show();
    showpid.close();
    myShowsensor.close();
    serialset.close();
    u30.close();
    gyroscope.close();
    remotectrl.close();
    baro.close();
    mag.close();
    Filter.close();
    ua18.close();
#endif

}

void MainWindow::on_actionSensorData_triggered()
{
    myShowsensor.show();
    showpid.close();
    ShowSensorWave.close();
    serialset.close();
    u30.close();
    gyroscope.close();
    remotectrl.close();
    baro.close();
    mag.close();
    Filter.close();
    ua18.close();
}

void MainWindow::on_actionU30_triggered()
{
    u30.show();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    serialset.close();
    gyroscope.close();
    remotectrl.close();
    baro.close();
    mag.close();
    Filter.close();
    ua18.close();


}

void MainWindow::on_actionSerialSet_triggered()
{
    serialset.show();

    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    gyroscope.close();
    remotectrl.close();
    baro.close();
    mag.close();
    Filter.close();
    ua18.close();


}

void MainWindow::on_actionGyro_triggered()
{
    gyroscope.show();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    baro.close();
    mag.close();
    remotectrl.close();
    Filter.close();
    ua18.close();


}

void MainWindow::on_actionBaro_triggered()
{
    baro.show();

    gyroscope.close();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    mag.close();
    remotectrl.close();
    Filter.close();
    ua18.close();


}

void MainWindow::on_actionMag_triggered()
{
    mag.show();
    gyroscope.close();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    baro.close();
    remotectrl.close();
    Filter.close();
    ua18.close();

}

void MainWindow::on_actionRC_triggered()
{
    remotectrl.show();
    mag.close();
    gyroscope.close();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    baro.close();
    Filter.close();
    ua18.close();

}

void MainWindow::on_actionBaro_Alt_triggered()
{
    Filter.show();
    remotectrl.close();
    mag.close();
    gyroscope.close();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    baro.close();
    ua18.close();
}

void MainWindow::on_actionUA18_triggered()
{

    ua18.show();
    Filter.close();
    remotectrl.close();
    mag.close();
    gyroscope.close();
    serialset.close();
    u30.close();
    myShowsensor.close();
    showpid.close();
    ShowSensorWave.close();
    baro.close();
}
