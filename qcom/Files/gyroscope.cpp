#include "gyroscope.h"
#include "ui_gyroscope.h"

#include "qcustom/qcustomplot.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "Files/ConfigData.h"

extern Sensor Gyro;
extern Sensor Acc;
extern U18Data U18;


GyroScope::GyroScope(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GyroScope)
{
    ui->setupUi(this);
//    this->show();
    this->setWindowFlags(Qt::SplashScreen);

    start_Init();

    GyroScopeTimer=new QTimer(this);
    connect(GyroScopeTimer, SIGNAL(timeout()), this, SLOT(ShowDataSlot()));
    GyroScopeTimer->start(0);
}

GyroScope::~GyroScope()
{
    delete GyroScopeTimer;
    delete ui;

}



void GyroScope::start_Init()
{

    ShowAcc_X_DataCurve(ui->Acc_X_customPlot);
    ShowAcc_Y_DataCurve(ui->Acc_Y_customPlot);
    ShowAcc_Z_DataCurve(ui->Acc_Z_customPlot);

    ShowGyro_X_DataCurve(ui->Gyro_X_customPlot);
    ShowGyro_Y_DataCurve(ui->Gyro_Y_customPlot);
    ShowGyro_X_DataCurve(ui->Gyro_Z_customPlot);

//    ui->Acc_X_customPlot->replot();
//    ui->Acc_Y_customPlot->replot();
//    ui->Acc_Z_customPlot->replot();

//    ui->Gyro_X_customPlot->replot();
//    ui->Gyro_Y_customPlot->replot();
//    ui->Gyro_Z_customPlot->replot();
}


void GyroScope::ShowSensorData(void)
{
    QString s;
    QTextCursor cursor;
    QString Acc_X_buf,Acc_Y_buf,Acc_Z_buf;
    QString Gyro_X_Buf,Gyro_Y_Buf,Gyro_Z_Buf;

    s=QString::number(Acc.X,10);
    Acc_X_buf+=s;
    s=QString::number(Acc.Y,10);
    Acc_Y_buf+=s;
    s=QString::number(Acc.Z,10);
    Acc_Z_buf+=s;

    s=QString::number(Gyro.X,10);
    Gyro_X_Buf+=s;
    s=QString::number(Gyro.Y,10);
    Gyro_Y_Buf+=s;
    s=QString::number(Gyro.Z,10);
    Gyro_Z_Buf+=s;

    ui->textBrowser->setText(ui->textBrowser->document()->toPlainText()+ Acc_X_buf+' ,' + Acc_Y_buf+' ,' + Acc_Z_buf+' ,' + Gyro_X_Buf+' ,' + Gyro_Y_Buf+' ,' + Gyro_Z_Buf+"\n");
    cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}

void GyroScope::ShowDataSlot()
{

    Acc_X_DataSlot();
    Acc_Y_DataSlot();
    Acc_Z_DataSlot();
    Gyro_X_DataSlot();
    Gyro_Y_DataSlot();
    Gyro_Z_DataSlot();

    ShowSensorData();

}


/*******************************
 *
 *实时显示数据
 *
********************************/
/*******
 * Acc.X
*******/
void GyroScope::ShowAcc_X_DataCurve(QCustomPlot *Acc_X_customPlot)
{

  Acc_X_customPlot->addGraph(); // blue line
  Acc_X_customPlot->graph(0)->setPen(QPen(Qt::blue));
  Acc_X_customPlot->graph(0)->setName("Gyro.Y");

  Acc_X_customPlot->graph(0)->setAntialiasedFill(false);
  Acc_X_customPlot->addGraph(); // red line
  Acc_X_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_X_customPlot->graph(0)->setChannelFillGraph(Acc_X_customPlot->graph(1));
  Acc_X_customPlot->graph(1)->setName("Gyro.Z");

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


}

void GyroScope::Acc_X_DataSlot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double lastPointKey = 0;
  if (key-lastPointKey > 0.01) // at most add point every 10 ms
  {

    double value0 = Acc.X;
//      double value0 = U18.Baro_High;


    // add data to lines:
    ui->Acc_X_customPlot->graph(0)->addData(key, value0);
//    ui->customPlot->graph(1)->addData(key, value1);
//    ui->customPlot->graph(4)->addData(key, value2);
    // set data of dots:
    ui->Acc_X_customPlot->graph(2)->clearData();
    ui->Acc_X_customPlot->graph(2)->addData(key, value0);
//    ui->customPlot->graph(3)->clearData();
//    ui->customPlot->graph(3)->addData(key, value1);

//    ui->customPlot->graph(5)->clearData();
//    ui->customPlot->graph(5)->addData(key, value2);
    // remove data of lines that's outside visible range:
    ui->Acc_X_customPlot->graph(0)->removeDataBefore(key-8);
//    ui->customPlot->graph(1)->removeDataBefore(key-8);
//    ui->customPlot->graph(4)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->Acc_X_customPlot->graph(0)->rescaleValueAxis();
//    ui->customPlot->graph(1)->rescaleValueAxis(true);
//    ui->customPlot->graph(4)->rescaleValueAxis(true);
    lastPointKey = key;
  }

  // make key axis range scroll with the data (at a constant range size of 8):
  ui->Acc_X_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

  ui->Acc_X_customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_X_customPlot->xAxis->setLabel("Times");

  ui->Acc_X_customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->Acc_X_customPlot->yAxis->setLabel("Acc.X");

//  ui->Acc_X_customPlot->yAxis->setRange(6900,7500);


  ui->Acc_X_customPlot->replot();

}

/*******
 * Acc.Y
*******/
void GyroScope::ShowAcc_Y_DataCurve(QCustomPlot *Acc_Y_customPlot)
{

  Acc_Y_customPlot->addGraph(); // blue line
  Acc_Y_customPlot->graph(0)->setPen(QPen(Qt::red));

  Acc_Y_customPlot->graph(0)->setAntialiasedFill(false);

  Acc_Y_customPlot->addGraph(); // red line
  Acc_Y_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_Y_customPlot->graph(0)->setChannelFillGraph(Acc_Y_customPlot->graph(1));

  Acc_Y_customPlot->addGraph(); // blue dot
  Acc_Y_customPlot->graph(2)->setPen(QPen(Qt::red));
  Acc_Y_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Acc_Y_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Acc_Y_customPlot->addGraph(); // red dot


  Acc_Y_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Acc_Y_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Acc_Y_customPlot->xAxis->setAutoTickStep(false);
  Acc_Y_customPlot->xAxis->setTickStep(2);
  Acc_Y_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Acc_Y_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Y_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Acc_Y_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Y_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void GyroScope::Acc_Y_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastTimetKey = 0;
    if (key-lastTimetKey > 0.01) // at most add point every 10 ms
    {
      double value0 = Acc.Y;
//      qDebug()<<"Acc.Y value0="<<value0;


      // add data to lines:
      ui->Acc_Y_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Acc_Y_customPlot->graph(2)->clearData();
      ui->Acc_Y_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Acc_Y_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Acc_Y_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastTimetKey = key;
    }

    //添加的鼠标缩放
//    ui->Acc_Y_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Acc_Y_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Acc_Y_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Acc_Y_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Acc_Y_customPlot->xAxis->setLabel("Times");

    ui->Acc_Y_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Acc_Y_customPlot->yAxis->setLabel("Acc.Y");

    ui->Acc_Y_customPlot->replot();

}

/*******
 * Acc.Z
*******/
void GyroScope::ShowAcc_Z_DataCurve(QCustomPlot *Acc_Z_customPlot)
{

  Acc_Z_customPlot->addGraph(); // blue line
  Acc_Z_customPlot->graph(0)->setPen(QPen(Qt::darkBlue));

  Acc_Z_customPlot->graph(0)->setAntialiasedFill(false);

  Acc_Z_customPlot->addGraph(); // red line
  Acc_Z_customPlot->graph(1)->setPen(QPen(Qt::red));
  Acc_Z_customPlot->graph(0)->setChannelFillGraph(Acc_Z_customPlot->graph(1));

  Acc_Z_customPlot->addGraph(); // blue dot
  Acc_Z_customPlot->graph(2)->setPen(QPen(Qt::darkBlue));
  Acc_Z_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Acc_Z_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Acc_Z_customPlot->addGraph(); // red dot


  Acc_Z_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Acc_Z_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Acc_Z_customPlot->xAxis->setAutoTickStep(false);
  Acc_Z_customPlot->xAxis->setTickStep(2);
  Acc_Z_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Acc_Z_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Z_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Acc_Z_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Acc_Z_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void GyroScope::Acc_Z_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastAcc_Z_Key = 0;
    if (key-lastAcc_Z_Key > 0.01) // at most add point every 10 ms
    {
      double value0 = Acc.Z;
//      qDebug()<<"Acc.Y value0="<<value0;


      // add data to lines:
      ui->Acc_Z_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Acc_Z_customPlot->graph(2)->clearData();
      ui->Acc_Z_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Acc_Z_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Acc_Z_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastAcc_Z_Key = key;
    }

    //添加的鼠标缩放
//    ui->Acc_Z_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Acc_Z_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Acc_Z_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Acc_Z_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Acc_Z_customPlot->xAxis->setLabel("Times");

    ui->Acc_Z_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Acc_Z_customPlot->yAxis->setLabel("Acc.Z");

    ui->Acc_Z_customPlot->replot();

}

/*******
 * Gyro.X
*******/
void GyroScope::ShowGyro_X_DataCurve(QCustomPlot *Gyro_X_customPlot)
{

  Gyro_X_customPlot->addGraph(); // blue line
  Gyro_X_customPlot->graph(0)->setPen(QPen(Qt::darkBlue));

  Gyro_X_customPlot->graph(0)->setAntialiasedFill(false);

  Gyro_X_customPlot->addGraph(); // red line
  Gyro_X_customPlot->graph(1)->setPen(QPen(Qt::red));
  Gyro_X_customPlot->graph(0)->setChannelFillGraph(Gyro_X_customPlot->graph(1));

  Gyro_X_customPlot->addGraph(); // blue dot
  Gyro_X_customPlot->graph(2)->setPen(QPen(Qt::darkBlue));
  Gyro_X_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Gyro_X_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Gyro_X_customPlot->addGraph(); // red dot


  Gyro_X_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Gyro_X_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Gyro_X_customPlot->xAxis->setAutoTickStep(false);
  Gyro_X_customPlot->xAxis->setTickStep(2);
  Gyro_X_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Gyro_X_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_X_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Gyro_X_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_X_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void GyroScope::Gyro_X_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_X_Key = 0;
    if (key-lastGyro_X_Key > 0.01) // at most add point every 10 ms
    {
      double value0 = Gyro.X;
//      qDebug()<<"Acc.Y value0="<<value0;


      // add data to lines:
      ui->Gyro_X_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Gyro_X_customPlot->graph(2)->clearData();
      ui->Gyro_X_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Gyro_X_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Gyro_X_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastGyro_X_Key = key;
    }

    //添加的鼠标缩放
//    ui->Gyro_X_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Gyro_X_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Gyro_X_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Gyro_X_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_X_customPlot->xAxis->setLabel("Times");

    ui->Gyro_X_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_X_customPlot->yAxis->setLabel("Gyro.X");

    ui->Gyro_X_customPlot->replot();

}

/*******
 * Gyro.Y
*******/
void GyroScope::ShowGyro_Y_DataCurve(QCustomPlot *Gyro_Y_customPlot)
{

  Gyro_Y_customPlot->addGraph(); // blue line
  Gyro_Y_customPlot->graph(0)->setPen(QPen(Qt::darkRed));

  Gyro_Y_customPlot->graph(0)->setAntialiasedFill(false);

  Gyro_Y_customPlot->addGraph(); // red line
  Gyro_Y_customPlot->graph(1)->setPen(QPen(Qt::red));
  Gyro_Y_customPlot->graph(0)->setChannelFillGraph(Gyro_Y_customPlot->graph(1));

  Gyro_Y_customPlot->addGraph(); // blue dot
  Gyro_Y_customPlot->graph(2)->setPen(QPen(Qt::darkBlue));
  Gyro_Y_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Gyro_Y_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Gyro_Y_customPlot->addGraph(); // red dot


  Gyro_Y_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Gyro_Y_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Gyro_Y_customPlot->xAxis->setAutoTickStep(false);
  Gyro_Y_customPlot->xAxis->setTickStep(2);
  Gyro_Y_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Gyro_Y_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_Y_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Gyro_Y_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_Y_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void GyroScope::Gyro_Y_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_Y_Key = 0;
    if (key-lastGyro_Y_Key > 0.01) // at most add point every 10 ms
    {
      double value0 = Gyro.Y;
//      qDebug()<<"Gyro.Y value0="<<value0;


      // add data to lines:
      ui->Gyro_Y_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Gyro_Y_customPlot->graph(2)->clearData();
      ui->Gyro_Y_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Gyro_Y_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Gyro_Y_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastGyro_Y_Key = key;
    }

    //添加的鼠标缩放
//    ui->Gyro_Y_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Gyro_Y_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Gyro_Y_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Gyro_Y_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_Y_customPlot->xAxis->setLabel("Times");

    ui->Gyro_Y_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_Y_customPlot->yAxis->setLabel("Gyro.Y");

    ui->Gyro_Y_customPlot->replot();

}

/*******
 * Gyro.z
*******/
void GyroScope::ShowGyro_Z_DataCurve(QCustomPlot *Gyro_Z_customPlot)
{

  Gyro_Z_customPlot->addGraph(); // blue line
  Gyro_Z_customPlot->graph(0)->setPen(QPen(Qt::darkBlue));

  Gyro_Z_customPlot->graph(0)->setAntialiasedFill(false);

  Gyro_Z_customPlot->addGraph(); // red line
  Gyro_Z_customPlot->graph(1)->setPen(QPen(Qt::red));
  Gyro_Z_customPlot->graph(0)->setChannelFillGraph(Gyro_Z_customPlot->graph(1));

  Gyro_Z_customPlot->addGraph(); // blue dot
  Gyro_Z_customPlot->graph(2)->setPen(QPen(Qt::darkBlue));
  Gyro_Z_customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  Gyro_Z_customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  Gyro_Z_customPlot->addGraph(); // red dot


  Gyro_Z_customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  Gyro_Z_customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  Gyro_Z_customPlot->xAxis->setAutoTickStep(false);
  Gyro_Z_customPlot->xAxis->setTickStep(2);
  Gyro_Z_customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(Gyro_Z_customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_Z_customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(Gyro_Z_customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), Gyro_Z_customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void GyroScope::Gyro_Z_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_Z_Key = 0;
    if (key-lastGyro_Z_Key > 0.01) // at most add point every 10 ms
    {
      double value0 = Gyro.Z;
//      qDebug()<<"Acc.Y value0="<<value0;


      // add data to lines:
      ui->Gyro_Z_customPlot->graph(0)->addData(key, value0);
  //    ui->customPlot->graph(1)->addData(key, value1);

      // set data of dots:
      ui->Gyro_Z_customPlot->graph(2)->clearData();
      ui->Gyro_Z_customPlot->graph(2)->addData(key, value0);

      // remove data of lines that's outside visible range:
      ui->Gyro_Z_customPlot->graph(0)->removeDataBefore(key-8);
  //    ui->customPlot->graph(1)->removeDataBefore(key-8);

      // rescale value (vertical) axis to fit the current data:
      ui->Gyro_Z_customPlot->graph(0)->rescaleValueAxis();
  //    ui->customPlot->graph(1)->rescaleValueAxis(true);

      lastGyro_Z_Key = key;
    }

    //添加的鼠标缩放
//    ui->Gyro_Z_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//    ui->Gyro_Z_customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Gyro_Z_customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

    ui->Gyro_Z_customPlot->xAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_Z_customPlot->xAxis->setLabel("Times");

    ui->Gyro_Z_customPlot->yAxis->setLabelColor(QColor(255,0,0));
    ui->Gyro_Z_customPlot->yAxis->setLabel("Gyro.Z");

    ui->Gyro_Z_customPlot->replot();

}






