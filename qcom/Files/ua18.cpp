#include "ua18.h"
#include "ui_ua18.h"
#include <QString>

#include "Files/ConfigData.h"

extern U18Data U18;

UA18::UA18(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UA18)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SplashScreen);


    ShowBaroHigh(ui->Baro_High_customPlot);

    ShowBaroHighTimer = new QTimer(this);
    connect(ShowBaroHighTimer, SIGNAL(timeout()), this, SLOT(ShowU18DataSlot()));
    ShowBaroHighTimer->start(0);
}

UA18::~UA18()
{
    delete ShowBaroHighTimer;
    delete ui;

}


void UA18::ShowU18DataSlot()
{
    ShowBaroHighSlot();
    ShowProcessBar();
    ShowUa18Info();
    ShowMode_Sensors();
}


//气压高度曲线化
void UA18::ShowBaroHigh(QCustomPlot *Baro_High_customPlot)
{

  Baro_High_customPlot->addGraph(); // blue line
  Baro_High_customPlot->graph(0)->setPen(QPen(Qt::red));

  Baro_High_customPlot->graph(0)->setAntialiasedFill(false);

  Baro_High_customPlot->addGraph(); // red line
  Baro_High_customPlot->graph(1)->setPen(QPen(Qt::red));
  Baro_High_customPlot->graph(0)->setChannelFillGraph(Baro_High_customPlot->graph(1));

  Baro_High_customPlot->addGraph(); // blue dot
  Baro_High_customPlot->graph(2)->setPen(QPen(Qt::red));
  Baro_High_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Baro_High_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_High_customPlot->addGraph(); // red dot


  Baro_High_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Baro_High_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Baro_High_customPlot->xAxis->setAutoTickStep(false);
  Baro_High_customPlot->xAxis->setTickStep(2);
  Baro_High_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Baro_High_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Baro_High_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Baro_High_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Baro_High_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

//更新曲线的槽函数
void UA18::ShowBaroHighSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastBaroHightKey = 0;
    if (key-lastBaroHightKey > 0.01) // at most add point every 10 ms
    {
      double value0 = U18.Baro_High;


      // add data to lines:
      ui->Baro_High_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Baro_High_customPlot->graph(2)->clearData();
      ui->Baro_High_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Baro_High_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Baro_High_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastBaroHightKey = key;
    }

    //添加的鼠标缩放
//    ui->Baro_High_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Baro_High_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Baro_High_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Baro_High_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Baro_High_customPlot->xAxis->setLabel("Times");

    ui->Baro_High_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Baro_High_customPlot->yAxis->setLabel("Baro_High");

//    ui->Baro_High_customPlot->yAxis->setRange(100,400);

    ui->Baro_High_customPlot->replot();

}



//显示油门、横滚、定高油门、当前油门的状态条
void UA18::ShowProcessBar()
{

    QString s;

    ui->Throttle_progressBar->setOrientation(Qt::Vertical);
    ui->Throttle_progressBar->setRange(800,2200);
    ui->Throttle_progressBar->setValue(U18.Throttle);
    s=QString::number(U18.Throttle,10);
    ui->Throttle->setText(s);

    ui->Roll_progressBar->setOrientation(Qt::Vertical);
    ui->Roll_progressBar->setRange(800,2200);
    ui->Roll_progressBar->setValue(U18.Roll);
    s=QString::number(U18.Roll,10);
    ui->Roll->setText(s);

    ui->Thorttle_HoldStar_progressBar->setOrientation(Qt::Vertical);
    ui->Thorttle_HoldStar_progressBar->setRange(800,2200);
    ui->Thorttle_HoldStar_progressBar->setValue(U18.Thorttle_HoldStar);
    s=QString::number(U18.Thorttle_HoldStar,10);
    ui->Thorttle_HoldStar->setText(s);

    ui->Throttle_Current_progressBar->setOrientation(Qt::Vertical);
    ui->Throttle_Current_progressBar->setRange(800,2200);
    ui->Throttle_Current_progressBar->setValue(U18.Throttle_Current);
    s=QString::number(U18.Throttle_Current,10);
    ui->Throttle_Current->setText(s);


}

//显示UA18的一些信息：飞控型号、飞行模式、高度、温度……
void UA18::ShowUa18Info()
{
     QString str;


     str=QString::number(U18.FlyType,16);
     ui->FlyType->setText(str);

     str=QString::number(U18.Bat,10);
     ui->Bat->setText(str);

     str=QString::number(U18.FlyMode,16);
     ui->FlyMode->setText(str);

     str=QString::number(U18.Sensors,16);
     ui->Sensors->setText(str);

     str=QString::number(U18.Temperature/100.0f,'g',6);
     ui->Temperature->setText(str);

     str=QString::number(U18.Baro_High,10);
     ui->Baro_High->setText(str);

     str=QString::number(U18.High_HoldStar,10);
     ui->High_HoldStar->setText(str);

     str=QString::number(U18.High_Current,10);
     ui->High_Current->setText(str);

}

//显示当前飞控处于那种飞行模式和有哪些传感器数据
void UA18::ShowMode_Sensors()
{

    ui->ARMED->setRange(0,1);
    ui->ANGLE_MODE->setRange(0,1);
    ui->HORIZON_MODE->setRange(0,1);
    ui->MAG_MODE->setRange(0,1);
    ui->HEADFREE_MODE->setRange(0,1);
    ui->GPS_HOME_MODE->setRange(0,1);
    ui->BARO_MODE->setRange(0,1);
    ui->GPS_HOLD_MODE->setRange(0,1);

    ui->ARMED->setOrientation(Qt::Vertical);
    ui->ANGLE_MODE->setOrientation(Qt::Vertical);
    ui->HORIZON_MODE->setOrientation(Qt::Vertical);
    ui->MAG_MODE->setOrientation(Qt::Vertical);
    ui->HEADFREE_MODE->setOrientation(Qt::Vertical);
    ui->GPS_HOLD_MODE->setOrientation(Qt::Vertical);
    ui->BARO_MODE->setOrientation(Qt::Vertical);
    ui->GPS_HOME_MODE->setOrientation(Qt::Vertical);

    if(U18.FlyMode & 0x80 )
        ui->ARMED->setValue(1);
    else
        ui->ARMED->setValue(0);

    if(U18.FlyMode & 0X40 )
        ui->ANGLE_MODE->setValue(1);
    else
        ui->ANGLE_MODE->setValue(0);

    if(U18.FlyMode & 0X20 )
        ui->HORIZON_MODE->setValue(1);
    else
        ui->HORIZON_MODE->setValue(0);

    if(U18.FlyMode & 0X10 )
        ui->MAG_MODE->setValue(1);
    else
        ui->MAG_MODE->setValue(0);

    if(U18.FlyMode & 0X08 )
        ui->HEADFREE_MODE->setValue(1);
    else
        ui->HEADFREE_MODE->setValue(0);

    if(U18.FlyMode && 0X04 )
        ui->GPS_HOME_MODE->setValue(1);
    else
        ui->GPS_HOME_MODE->setValue(0);

    if(U18.FlyMode & 0X02 )
        ui->BARO_MODE->setValue(1);
    else
        ui->BARO_MODE->setValue(0);

    if(U18.FlyMode & 0X01 )
        ui->GPS_HOLD_MODE->setValue(1);
    else
        ui->GPS_HOLD_MODE->setValue(0);

/******************************************************************/

    ui->ACC->setRange(0,1);
    ui->BARO->setRange(0,1);
    ui->MAG->setRange(0,1);
    ui->GPS->setRange(0,1);
    ui->GYRO->setRange(0,1);

    ui->ACC->setOrientation(Qt::Vertical);
    ui->BARO->setOrientation(Qt::Vertical);
    ui->GPS->setOrientation(Qt::Vertical);
    ui->MAG->setOrientation(Qt::Vertical);
    ui->GYRO->setOrientation(Qt::Vertical);


    if(U18.Sensors & 0X80 )
        ui->ACC->setValue(1);
    else
        ui->ACC->setValue(0);

    if(U18.Sensors & 0X40 )
        ui->BARO->setValue(1);
    else
        ui->BARO->setValue(0);

    if(U18.Sensors & 0X20 )
        ui->MAG->setValue(1);
    else
        ui->MAG->setValue(0);

    if(U18.Sensors & 0X10 )
        ui->GPS->setValue(1);
    else
        ui->GPS->setValue(0);

    if(U18.Sensors & 0X08 )
        ui->GYRO->setValue(1);
    else
        ui->GYRO->setValue(0);




}

