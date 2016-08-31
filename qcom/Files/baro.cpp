#include "baro.h"
#include "ui_baro.h"
#include <QString>
#include "Files/ConfigData.h"
#include "qcustom/qcustomplot.h"

extern Sensor Acc;
extern U30_BARO U30_Baro;


Baro::Baro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Baro)
{
    ui->setupUi(this);
//    this->show();
    this->setWindowFlags(Qt::SplashScreen);

    Start_Init();

    BaroTimer = new QTimer(this);
    connect(BaroTimer, SIGNAL(timeout()), this, SLOT(ShowDataSlot()));
    BaroTimer->start(1);



}

Baro::~Baro()
{
    delete BaroTimer;
    delete ui;

}


void Baro::Start_Init()
{
    ShowAcc_X_Curve(ui->Acc_X_customPlot);
    ShowAcc_Y_Curve(ui->Acc_Y_customPlot);
    ShowAcc_Z_Curve(ui->Acc_Z_customPlot);

    ShowBaro_Alt_Curve(ui->Baro_Alt_customPlot);
    ShowBaro_Pres_Curve(ui->Baro_Pres_customPlot);
    ShowBaro_Temp_Curve(ui->Baro_Temp_customPlot);


}


void Baro::ShowDataSlot()
{
    ShowBaroData();
    Acc_X_DataSlot();
    Acc_Y_DataSlot();
    Acc_Z_DataSlot();

    Baro_Alt_Slot();
    Baro_Pres_Slot();
    Baro_Temp_Slot();
}

void Baro::ShowBaroData()
{

    QString s;
    QTextCursor cursor;
    QString Acc_X_buf,Acc_Y_buf,Acc_Z_buf;
    QString Baro_Alt_buf,Baro_Pres_buf,Baro_Temp_buf;

    s=QString::number(Acc.X,10);
    Acc_X_buf+=s;
    s=QString::number(Acc.Y,10);
    Acc_Y_buf+=s;
    s=QString::number(Acc.Z,10);
    Acc_Z_buf+=s;

    s=QString::number(U30_Baro.alt,'g',6);
    Baro_Alt_buf+=s;
    s=QString::number(U30_Baro.Pressure,'g',6);
    Baro_Pres_buf+=s;
    s=QString::number(U30_Baro.Temperature,'g',6);
    Baro_Temp_buf+=s;

    ui->textBrowser->setText(ui->textBrowser->document()->toPlainText()+ Acc_X_buf+' ,' + Acc_Y_buf+' ,' + Acc_Z_buf+' ,' + Baro_Alt_buf+' ,' + Baro_Pres_buf+' ,' + Baro_Temp_buf+"\n");
    cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}


/*************
 *
 * 显示曲线
 *
************/

void Baro::ShowAcc_X_Curve(QCustomPlot *Acc_X_customPlot)
{

  Acc_X_customPlot->addGraph(); // blue line
  Acc_X_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Acc_X_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Acc_X_customPlot->graph(0)->setAntialiasedFill(false);
  Acc_X_customPlot->addGraph(); // red line
  Acc_X_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_X_customPlot->graph(0)->setChannelFillGraph(Acc_X_customPlot->graph(1));

  Acc_X_customPlot->addGraph(); // blue dot
  Acc_X_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Acc_X_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Acc_X_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Acc_X_customPlot->addGraph(); // red dot
  Acc_X_customPlot->graph(3)->setPen(QPen(Qt::red));
  Acc_X_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Acc_X_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Acc_X_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Acc_X_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Acc_X_customPlot->xAxis->setAutoTickStep(false);
  Acc_X_customPlot->xAxis->setTickStep(2);
  Acc_X_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Acc_X_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Acc_X_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Acc_X_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Acc_X_customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void Baro::Acc_X_DataSlot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Acc_X_PointKey = 0;
  if (key-Acc_X_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

    double value0=Acc.X;
    double value1=Acc.X+2;

    ui->Acc_X_customPlot->graph(0)->addData(key, value0);
    ui->Acc_X_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Acc_X_customPlot->graph(2)->clearData();
    ui->Acc_X_customPlot->graph(2)->addData(key, value0);
    ui->Acc_X_customPlot->graph(3)->clearData();
    ui->Acc_X_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Acc_X_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Acc_X_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Acc_X_customPlot->graph(0)->rescaleValueAxis();
    ui->Acc_X_customPlot->graph(1)->rescaleValueAxis(true);
    Acc_X_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Acc_X_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Acc_X_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Acc_X_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Acc_X_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_X_customPlot->yAxis->setLabel("Acc.X");
  ui->Acc_X_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_X_customPlot->xAxis->setLabel("Times");

  ui->Acc_X_customPlot->replot();

}

void Baro::ShowAcc_Y_Curve(QCustomPlot *Acc_Y_customPlot)
{

  Acc_Y_customPlot->addGraph(); // blue line
  Acc_Y_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Acc_Y_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Acc_Y_customPlot->graph(0)->setAntialiasedFill(false);
  Acc_Y_customPlot->addGraph(); // red line
  Acc_Y_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_Y_customPlot->graph(0)->setChannelFillGraph(Acc_Y_customPlot->graph(1));

  Acc_Y_customPlot->addGraph(); // blue dot
  Acc_Y_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Acc_Y_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Acc_Y_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Acc_Y_customPlot->addGraph(); // red dot
  Acc_Y_customPlot->graph(3)->setPen(QPen(Qt::red));
  Acc_Y_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Acc_Y_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Acc_Y_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Acc_Y_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Acc_Y_customPlot->xAxis->setAutoTickStep(false);
  Acc_Y_customPlot->xAxis->setTickStep(2);
  Acc_Y_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Acc_Y_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Y_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Acc_Y_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Y_customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void Baro::Acc_Y_DataSlot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Acc_Y_PointKey = 0;
  if (key-Acc_Y_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

    double value0=Acc.Y;
    double value1=Acc.Y+2;

    ui->Acc_Y_customPlot->graph(0)->addData(key, value0);
    ui->Acc_Y_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Acc_Y_customPlot->graph(2)->clearData();
    ui->Acc_Y_customPlot->graph(2)->addData(key, value0);
    ui->Acc_Y_customPlot->graph(3)->clearData();
    ui->Acc_Y_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Acc_Y_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Acc_Y_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Acc_Y_customPlot->graph(0)->rescaleValueAxis();
    ui->Acc_Y_customPlot->graph(1)->rescaleValueAxis(true);
    Acc_Y_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Acc_Y_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Acc_Y_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Acc_Y_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Acc_Y_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_Y_customPlot->yAxis->setLabel("Acc.Y");
  ui->Acc_Y_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_Y_customPlot->xAxis->setLabel("Times");


  ui->Acc_Y_customPlot->replot();


  // calculate frames per second:

}

void Baro::ShowAcc_Z_Curve(QCustomPlot *Acc_Z_customPlot)
{

  Acc_Z_customPlot->addGraph(); // blue line
  Acc_Z_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Acc_X_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Acc_X_customPlot->graph(0)->setAntialiasedFill(false);
  Acc_Z_customPlot->addGraph(); // red line
  Acc_Z_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_Z_customPlot->graph(0)->setChannelFillGraph(Acc_Z_customPlot->graph(1));

  Acc_Z_customPlot->addGraph(); // blue dot
  Acc_Z_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Acc_Z_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Acc_Z_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Acc_Z_customPlot->addGraph(); // red dot
  Acc_Z_customPlot->graph(3)->setPen(QPen(Qt::red));
  Acc_Z_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Acc_Z_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Acc_Z_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Acc_Z_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Acc_Z_customPlot->xAxis->setAutoTickStep(false);
  Acc_Z_customPlot->xAxis->setTickStep(2);
  Acc_Z_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Acc_Z_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Z_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Acc_Z_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Z_customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void Baro::Acc_Z_DataSlot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Acc_Z_PointKey = 0;
  if (key-Acc_Z_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

    double value0=Acc.X;
    double value1=Acc.X+2;

    ui->Acc_Z_customPlot->graph(0)->addData(key, value0);
    ui->Acc_Z_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Acc_Z_customPlot->graph(2)->clearData();
    ui->Acc_Z_customPlot->graph(2)->addData(key, value0);
    ui->Acc_Z_customPlot->graph(3)->clearData();
    ui->Acc_Z_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Acc_Z_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Acc_Z_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Acc_Z_customPlot->graph(0)->rescaleValueAxis();
    ui->Acc_Z_customPlot->graph(1)->rescaleValueAxis(true);
    Acc_Z_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Acc_Z_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Acc_Z_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Acc_Z_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Acc_Z_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_Z_customPlot->yAxis->setLabel("Acc.Z");
  ui->Acc_Z_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_Z_customPlot->xAxis->setLabel("Times");



  ui->Acc_Z_customPlot->replot();


  // calculate frames per second:

}

void Baro::ShowBaro_Alt_Curve(QCustomPlot *Baro_Alt_customPlot)
{

  Baro_Alt_customPlot->addGraph(); // blue line
  Baro_Alt_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Acc_X_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Acc_X_customPlot->graph(0)->setAntialiasedFill(false);
  Baro_Alt_customPlot->addGraph(); // red line
  Baro_Alt_customPlot->graph(1)->setPen(QPen(Qt::red));
  Baro_Alt_customPlot->graph(0)->setChannelFillGraph(Baro_Alt_customPlot->graph(1));

  Baro_Alt_customPlot->addGraph(); // blue dot
  Baro_Alt_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Baro_Alt_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_Alt_customPlot->addGraph(); // red dot
  Baro_Alt_customPlot->graph(3)->setPen(QPen(Qt::red));
  Baro_Alt_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Baro_Alt_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Baro_Alt_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Baro_Alt_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Baro_Alt_customPlot->xAxis->setAutoTickStep(false);
  Baro_Alt_customPlot->xAxis->setTickStep(2);
  Baro_Alt_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Baro_Alt_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Alt_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Baro_Alt_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Alt_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void Baro::Baro_Alt_Slot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Baro_Alt_PointKey = 0;
  if (key-Baro_Alt_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

    double value0=U30_Baro.Original_Alt;
//    double value1=U30_Baro.alt;
    double value1=U30_Baro.EKF_AltHold;

//    qDebug()<<"U30_Baro.alt :"<<U30_Baro.alt;

    ui->Baro_Alt_customPlot->graph(0)->addData(key, value0);
    ui->Baro_Alt_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Baro_Alt_customPlot->graph(2)->clearData();
    ui->Baro_Alt_customPlot->graph(2)->addData(key, value0);
    ui->Baro_Alt_customPlot->graph(3)->clearData();
    ui->Baro_Alt_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Baro_Alt_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Baro_Alt_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Baro_Alt_customPlot->graph(0)->rescaleValueAxis();
    ui->Baro_Alt_customPlot->graph(1)->rescaleValueAxis(true);
    Baro_Alt_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Baro_Alt_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Baro_Alt_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Baro_Alt_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Baro_Alt_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Alt_customPlot->yAxis->setLabel("ALT");
  ui->Baro_Alt_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Alt_customPlot->xAxis->setLabel("Times");


  ui->Baro_Alt_customPlot->replot();


  // calculate frames per second:

}

void Baro::ShowBaro_Pres_Curve(QCustomPlot *Baro_Pres_customPlot)
{

  Baro_Pres_customPlot->addGraph(); // blue line
  Baro_Pres_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Acc_X_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Acc_X_customPlot->graph(0)->setAntialiasedFill(false);
  Baro_Pres_customPlot->addGraph(); // red line
  Baro_Pres_customPlot->graph(1)->setPen(QPen(Qt::red));
  Baro_Pres_customPlot->graph(0)->setChannelFillGraph(Baro_Pres_customPlot->graph(1));

  Baro_Pres_customPlot->addGraph(); // blue dot
  Baro_Pres_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Baro_Pres_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Baro_Pres_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_Pres_customPlot->addGraph(); // red dot
  Baro_Pres_customPlot->graph(3)->setPen(QPen(Qt::red));
  Baro_Pres_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Baro_Pres_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Baro_Pres_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Baro_Pres_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Baro_Pres_customPlot->xAxis->setAutoTickStep(false);
  Baro_Pres_customPlot->xAxis->setTickStep(2);
  Baro_Pres_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Baro_Pres_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Pres_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Baro_Pres_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Pres_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void Baro::Baro_Pres_Slot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Baro_Pres_PointKey = 0;
  if (key-Baro_Pres_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

//    U30_Baro.Pressure = 2;

    double value0=U30_Baro.Pressure;
    double value1=U30_Baro.Pressure+2;

    ui->Baro_Pres_customPlot->graph(0)->addData(key, value0);
    ui->Baro_Pres_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Baro_Pres_customPlot->graph(2)->clearData();
    ui->Baro_Pres_customPlot->graph(2)->addData(key, value0);
    ui->Baro_Pres_customPlot->graph(3)->clearData();
    ui->Baro_Pres_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Baro_Pres_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Baro_Pres_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Baro_Pres_customPlot->graph(0)->rescaleValueAxis();
    ui->Baro_Pres_customPlot->graph(1)->rescaleValueAxis(true);
    Baro_Pres_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Baro_Pres_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Baro_Pres_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Baro_Pres_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Baro_Pres_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Pres_customPlot->yAxis->setLabel("Pressure");
  ui->Baro_Pres_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Pres_customPlot->xAxis->setLabel("Times");


  ui->Baro_Pres_customPlot->replot();


  // calculate frames per second:

}

void Baro::ShowBaro_Temp_Curve(QCustomPlot *Baro_Temp_customPlot)
{

  Baro_Temp_customPlot->addGraph(); // blue line
  Baro_Temp_customPlot->graph(0)->setPen(QPen(Qt::blue));
//  Baro_Temp_customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
//  Baro_Temp_customPlot->graph(0)->setAntialiasedFill(false);
  Baro_Temp_customPlot->addGraph(); // red line
  Baro_Temp_customPlot->graph(1)->setPen(QPen(Qt::red));
  Baro_Temp_customPlot->graph(0)->setChannelFillGraph(Baro_Temp_customPlot->graph(1));

  Baro_Temp_customPlot->addGraph(); // blue dot
  Baro_Temp_customPlot->graph(2)->setPen(QPen(Qt::blue));
  Baro_Temp_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Baro_Temp_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Baro_Temp_customPlot->addGraph(); // red dot
  Baro_Temp_customPlot->graph(3)->setPen(QPen(Qt::red));
  Baro_Temp_customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  Baro_Temp_customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  Baro_Temp_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Baro_Temp_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Baro_Temp_customPlot->xAxis->setAutoTickStep(false);
  Baro_Temp_customPlot->xAxis->setTickStep(2);
  Baro_Temp_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Baro_Temp_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Temp_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Baro_Temp_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Baro_Temp_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void Baro::Baro_Temp_Slot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double Baro_Alt_PointKey = 0;
  if (key-Baro_Alt_PointKey > 0.01) // at most add point every 10 ms
  {
//    double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
//    double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
    // add data to lines:

//    U30_Baro.Temperature=4;
    double value0=U30_Baro.Temperature;
    double value1=U30_Baro.Temperature+0.02;

    ui->Baro_Temp_customPlot->graph(0)->addData(key, value0);
    ui->Baro_Temp_customPlot->graph(1)->addData(key, value1);

    // set data of dots:
    ui->Baro_Temp_customPlot->graph(2)->clearData();
    ui->Baro_Temp_customPlot->graph(2)->addData(key, value0);
    ui->Baro_Temp_customPlot->graph(3)->clearData();
    ui->Baro_Temp_customPlot->graph(3)->addData(key, value1);

    // remove data of lines that's outside visible range:
    ui->Baro_Temp_customPlot->graph(0)->removeDataBefore(key-8);
    ui->Baro_Temp_customPlot->graph(1)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Baro_Temp_customPlot->graph(0)->rescaleValueAxis();
    ui->Baro_Temp_customPlot->graph(1)->rescaleValueAxis(true);
    Baro_Alt_PointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Baro_Temp_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

//  ui->Acc_X_customPlot->yAxis2
  ui->Baro_Temp_customPlot->xAxis2->setLabelColor(QColor(255,0,255));
  ui->Baro_Temp_customPlot->xAxis2->setLabel("蓝线：滤波前 红线：滤波后");

  ui->Baro_Temp_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Temp_customPlot->yAxis->setLabel("Temperature");
  ui->Baro_Temp_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Baro_Temp_customPlot->xAxis->setLabel("Times");


  ui->Baro_Temp_customPlot->replot();


  // calculate frames per second:

}












