#include "showwaveform.h"
#include "ui_showwaveform.h"

#include "qcustom/qcustomplot.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "Files/ConfigData.h"
#include "u30.h"

extern int U30_Flag;

extern Sensor Gyro;
extern Sensor Acc;

ShowWaveform::ShowWaveform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowWaveform)
{
    ui->setupUi(this);
//    this->show();

//    setGeometry(400, 250, 542, 390);
    this->setWindowFlags(Qt::SplashScreen);

    setupDemo(10);

    // setup a timer that repeatedly calls ShowWaveform::realtimeDataSlot:
//    dataTimer=new QTimer(this);
//    connect(dataTimer, SIGNAL(timeout()), this, SLOT(ShowDataSlot()));
//    dataTimer->start(10); // Interval 0 means to refresh as fast as possible

}

ShowWaveform::~ShowWaveform()
{
//    delete dataTimer;
    delete ui;

}


void ShowWaveform::setupDemo(int demoIndex)
{
  switch (demoIndex)
  {
   case 10:
      setupRealtimeDataDemo(ui->customPlot);
      ShowAcc_Y_DataCurve(ui->Acc_Y_customPlot);
      ShowAcc_Z_DataCurve(ui->Acc_Z_customPlot);

      ShowGyro_X_DataCurve(ui->Gyro_X_customPlot);
      ShowGyro_Y_DataCurve(ui->Gyro_Y_customPlot);
      ShowGyro_X_DataCurve(ui->Gyro_Z_customPlot);

      break;

  }
  setWindowTitle("QCustomPlot: "+demoName);
//  statusBar()->clearMessage();
  currentDemoIndex = demoIndex;

  ui->customPlot->replot();
  ui->Acc_Y_customPlot->replot();
  ui->Acc_Z_customPlot->replot();

  ui->Gyro_X_customPlot->replot();
  ui->Gyro_Y_customPlot->replot();
  ui->Gyro_Z_customPlot->replot();
}


void ShowWaveform::ShowSensorData(void)
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




/*******************************
 *
 *实时显示数据
 *
********************************/
/*******
 * Acc.X
*******/
void ShowWaveform::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
//  customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  customPlot->graph(0)->setName("Gyro.Y");

  customPlot->graph(0)->setAntialiasedFill(false);
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(Qt::red));
  customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
  customPlot->graph(1)->setName("Gyro.Z");
  // 自己添加的
//  customPlot->addGraph(); // black line
//  customPlot->graph(4)->setPen(QPen(Qt::black));
//  customPlot->graph(1)->setChannelFillGraph(customPlot->graph(4));
  // 自己添加的

  customPlot->addGraph(); // blue dot
  customPlot->graph(2)->setPen(QPen(Qt::blue));
  customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot->addGraph(); // red dot
  customPlot->graph(3)->setPen(QPen(Qt::red));
  customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
  // 自己添加的
//  customPlot->addGraph(); // red dot
//  customPlot->graph(5)->setPen(QPen(Qt::black));
//  customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
//  customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);
  // 自己添加的

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox();

//  customPlot->graph(0)->setName("Gyro.Y");
//  customPlot->graph(1)->setName("Gyro.Z");

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));


}

void ShowWaveform::realtimeDataSlot()
{

  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

  static double lastPointKey = 0;
  if (key-lastPointKey > 0.001) // at most add point every 10 ms
  {

    double value0 = Acc.X;
//    double value1 = Gyro.Z;
//    double value2 = Acc.Z;

//    qDebug()<<"Acc.X value0="<<value0;
//    qDebug()<<"value1="<<value1;

    // add data to lines:
    ui->customPlot->graph(0)->addData(key, value0);
//    ui->customPlot->graph(1)->addData(key, value1);
//    ui->customPlot->graph(4)->addData(key, value2);
    // set data of dots:
    ui->customPlot->graph(2)->clearData();
    ui->customPlot->graph(2)->addData(key, value0);
//    ui->customPlot->graph(3)->clearData();
//    ui->customPlot->graph(3)->addData(key, value1);

//    ui->customPlot->graph(5)->clearData();
//    ui->customPlot->graph(5)->addData(key, value2);
    // remove data of lines that's outside visible range:
    ui->customPlot->graph(0)->removeDataBefore(key-8);
//    ui->customPlot->graph(1)->removeDataBefore(key-8);
//    ui->customPlot->graph(4)->removeDataBefore(key-8);
    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis();
//    ui->customPlot->graph(1)->rescaleValueAxis(true);
//    ui->customPlot->graph(4)->rescaleValueAxis(true);
    lastPointKey = key;
  }

  //添加的鼠标缩放
//  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
//  ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);

  ui->customPlot->xAxis->setLabelColor(QColor(255,0,0));
  ui->customPlot->xAxis->setLabel("Times");

  ui->customPlot->yAxis->setLabelColor(QColor(255,0,0));
  ui->customPlot->yAxis->setLabel("Acc.X");

  ui->customPlot->replot();

}

/*******
 * Acc.Y
*******/
void ShowWaveform::ShowAcc_Y_DataCurve(QCustomPlot *Acc_Y_customPlot)
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

void ShowWaveform::Acc_Y_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastTimetKey = 0;
    if (key-lastTimetKey > 0.001) // at most add point every 10 ms
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
void ShowWaveform::ShowAcc_Z_DataCurve(QCustomPlot *Acc_Z_customPlot)
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

void ShowWaveform::Acc_Z_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastAcc_Z_Key = 0;
    if (key-lastAcc_Z_Key > 0.001) // at most add point every 10 ms
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
void ShowWaveform::ShowGyro_X_DataCurve(QCustomPlot *Gyro_X_customPlot)
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

void ShowWaveform::Gyro_X_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_X_Key = 0;
    if (key-lastGyro_X_Key > 0.001) // at most add point every 10 ms
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
void ShowWaveform::ShowGyro_Y_DataCurve(QCustomPlot *Gyro_Y_customPlot)
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

void ShowWaveform::Gyro_Y_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_Y_Key = 0;
    if (key-lastGyro_Y_Key > 0.001) // at most add point every 10 ms
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
void ShowWaveform::ShowGyro_Z_DataCurve(QCustomPlot *Gyro_Z_customPlot)
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

void ShowWaveform::Gyro_Z_DataSlot()
{

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastGyro_Z_Key = 0;
    if (key-lastGyro_Z_Key > 0.001) // at most add point every 10 ms
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


#if 1
void ShowWaveform::ShowDataSlot()
{
    static int Times=0;
    Times++;
    switch(Times)
    {
        case 1:
             qDebug()<<"Wave ACC"<<Acc.X;

            realtimeDataSlot();
            break;

        case 2:
             Acc_Y_DataSlot();
             break;

        case 3:
            Acc_Z_DataSlot();
            break;

        case 4:
            Gyro_X_DataSlot();
            break;

        case 5:
            Gyro_Y_DataSlot();
            break;
        case 6:
            Gyro_Z_DataSlot();
            break;

        case 7:
            ShowSensorData();
            break;

        default:
            break;
    }
    if( Times > 7 )
        Times=0;

}
#endif
