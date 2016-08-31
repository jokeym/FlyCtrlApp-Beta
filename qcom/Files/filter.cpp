#include "filter.h"
#include "ui_filter.h"
#include "Files/ConfigData.h"
#include "qcustom/qcustomplot.h"



extern U30_BARO U30_Baro;
extern U18Data U18;

FILTER::FILTER(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FILTER)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen);

    ShowBaroAlt(ui->Baro_Alt_customPlot)  ;

    ShowTimer = new QTimer(this);

    connect(ShowTimer, SIGNAL(timeout()), this, SLOT(Show_Slot()));
    ShowTimer->start(0);
}

FILTER::~FILTER()
{
    delete ShowTimer;
    delete ui;
}


void FILTER::Show_Slot()
{
    Baro_Alt_Slot();
    ShowAltData();
}

void FILTER::ShowBaroAlt(QCustomPlot *Baro_Alt_customPlot)
{

  Baro_Alt_customPlot->addGraph(); // blue line
  Baro_Alt_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Baro_Alt_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  Baro_Alt_customPlot->addGraph(); // red line
  Baro_Alt_customPlot->graph(1)->setPen(QPen(Qt::red));
//  Baro_Alt_customPlot->graph(0)->setChannelFillGraph(Baro_Alt_customPlot->graph(1));
  Baro_Alt_customPlot->addGraph();
  Baro_Alt_customPlot->graph(2)->setPen(QPen(Qt::green));
  Baro_Alt_customPlot->addGraph();
  Baro_Alt_customPlot->graph(3)->setPen(QPen(Qt::yellow));


  Baro_Alt_customPlot->addGraph(); // blue dot
  Baro_Alt_customPlot->graph(4)->setPen(QPen(Qt::blue));
  Baro_Alt_customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(4)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_Alt_customPlot->addGraph(); // red dot
  Baro_Alt_customPlot->graph(5)->setPen(QPen(Qt::red));
  Baro_Alt_customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);

  Baro_Alt_customPlot->addGraph(); // green dot //添加的
  Baro_Alt_customPlot->graph(6)->setPen(QPen(Qt::green));
  Baro_Alt_customPlot->graph(6)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(6)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_Alt_customPlot->addGraph(); // green dot //添加的
  Baro_Alt_customPlot->graph(7)->setPen(QPen(QColor(88,48,194,255)));
  Baro_Alt_customPlot->graph(7)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(7)->setScatterStyle(QCPScatterStyle::ssDisc);

  Baro_Alt_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Baro_Alt_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Baro_Alt_customPlot->xAxis->setAutoTickStep(false);
  Baro_Alt_customPlot->xAxis->setTickStep(2);
  Baro_Alt_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Baro_Alt_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Alt_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Baro_Alt_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Alt_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void FILTER::Baro_Alt_Slot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Baro_Alt_PointKey = 0;
  if (key-Baro_Alt_PointKey > 0.001) // at most add point every 10 ms
  {

    double value0=U30_Baro.Original_Alt;
    double value1=U30_Baro.EKF_AltHold;
    double value2=U30_Baro.Add_AltHold;
    double value3=U30_Baro.alt;
//    double value0=U18.Baro_High;

    qDebug()<<"U30_Baro.alt :"<<U30_Baro.alt;
    qDebug()<<"U30_Baro.Original_Alt :"<<U30_Baro.Original_Alt;


    ui->Baro_Alt_customPlot->graph(0)->addData(key, value0);
    ui->Baro_Alt_customPlot->graph(1)->addData(key, value1);
    ui->Baro_Alt_customPlot->graph(2)->addData(key, value2);    //添加的
    ui->Baro_Alt_customPlot->graph(3)->addData(key ,value3);

    // set data of dots:
    ui->Baro_Alt_customPlot->graph(4)->clearData();
    ui->Baro_Alt_customPlot->graph(4)->addData(key, value0);
    ui->Baro_Alt_customPlot->graph(5)->clearData();
    ui->Baro_Alt_customPlot->graph(5)->addData(key, value1);
    ui->Baro_Alt_customPlot->graph(6)->clearData();         //添加的
    ui->Baro_Alt_customPlot->graph(6)->addData(key, value2);
    ui->Baro_Alt_customPlot->graph(7)->clearData();         //添加的
    ui->Baro_Alt_customPlot->graph(7)->addData(key, value3);

    // remove data of lines that's outside visible range:
    ui->Baro_Alt_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Baro_Alt_customPlot->graph(1)->removeDataBefore(key-8);
    ui->Baro_Alt_customPlot->graph(2)->removeDataBefore(key-8);
    ui->Baro_Alt_customPlot->graph(3)->removeDataBefore(key-8);

    // rescale value (vertical) axis to fit the current data:
    ui->Baro_Alt_customPlot->graph(0)->rescaleValueAxis();
    ui->Baro_Alt_customPlot->graph(1)->rescaleValueAxis();
    ui->Baro_Alt_customPlot->graph(2)->rescaleValueAxis();
    ui->Baro_Alt_customPlot->graph(3)->rescaleValueAxis(true);


    Baro_Alt_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Baro_Alt_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Baro_Alt_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Baro_Alt_customPlot->xAxis2->setLabel("蓝线：Original_Alt 红线：EKF_AltHold 绿线：Add_AltHold 黄线：alt");

  ui->Baro_Alt_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Alt_customPlot->yAxis->setLabel("ALT");
  ui->Baro_Alt_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Alt_customPlot->xAxis->setLabel("Times");

  ui->Baro_Alt_customPlot->yAxis->setRange(0,600);


  ui->Baro_Alt_customPlot->replot();


  // calculate frames per second:

}


void FILTER::ShowAltData()
{
    QString Str;

    Str=QString::number(U30_Baro.Original_Alt,'g',6);
    ui->Original_Alt->setText(Str+" cm");

    Str=QString::number(U30_Baro.EKF_AltHold,'g',6);
    ui->EKF_AltHold->setText(Str+" cm");

    Str=QString::number(U30_Baro.Add_AltHold,'g',6);
    ui->Add_AltHold->setText(Str+" cm");

    Str=QString::number(U30_Baro.alt,'g',6);
    ui->Alt->setText(Str+" cm");



}













