#include "showsensordata.h"
#include "ui_showsensordata.h"

#include "Files/ConfigData.h"
#include <QTextCursor>
#include "qcustom/qcustomplot.h"
#include <QWidget>
#include <QTableWidget>
#include <QAbstractItemView>
#include <QStringList>
#include <QTimer>

extern Sensor Gyro;
extern Sensor Acc;
extern U30_BARO U30_Baro;


ShowSensorData::ShowSensorData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowSensorData)
{
    ui->setupUi(this);

//    this->show();
    this->setWindowFlags(Qt::SplashScreen);

    ShowDataTimer = new QTimer(this);
    connect(ShowDataTimer,SIGNAL(timeout()),this,SLOT(ShowData_Slot()));
    ShowDataTimer->start(5);

}

ShowSensorData::~ShowSensorData()
{
    delete ShowDataTimer;
    delete ui;

}

void ShowSensorData::ShowData_Slot()
{
    ui->AllData->hide();
    ShowAllSensorData();
}


QStringList list;

//显示所有的传感器数据
void ShowSensorData::ShowAllSensorData(void)
{
    static int i=0,j=0;
    int x,y,count;
    static int Nums=0;
    Nums++;
    bool ok;
    QString s;
    QTextCursor cursor;
    QString Acc_X_buf,Acc_Y_buf,Acc_Z_buf;
    QString Gyro_X_Buf,Gyro_Y_Buf,Gyro_Z_Buf;
    QString Baro_Alt_buf,Baro_Temp_buf,Baro_Press_buf;

//    QString mySensorBuf[3]={Acc_X_buf,Acc_Y_buf,Acc_Z_buf};

    s=QString::number(Acc.X,10);
    Acc_X_buf+=s;
//    mySensorBuf[0]+=s;
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

    s=QString::number(U30_Baro.Original_Alt,'g',6);
    Baro_Alt_buf+=s;
    s=QString::number(U30_Baro.Temperature,'g',6);
    Baro_Temp_buf+=s;
    s=QString::number(U30_Baro.Pressure,'g',6);
    Baro_Press_buf+=s;


//    ui->AllData->setText(ui->AllData->document()->toPlainText()+ Acc_X_buf+' ,' + Acc_Y_buf+' ,' + Acc_Z_buf+' ,' + Gyro_X_Buf+' ,' + Gyro_Y_Buf+' ,' + Gyro_Z_Buf+' ,'+ Baro_Alt_buf + ' ,' + Baro_Temp_buf + ' ,' + Baro_Press_buf "\n");
//    cursor = ui->AllData->textCursor();
//    cursor.movePosition(QTextCursor::End);
//    ui->AllData->setTextCursor(cursor);
//    ui->AllData->append(cursor);

///////////////////////数据表格化///////////////////////////

    ui->tableWidget->setRowCount(15);
    ui->tableWidget->setColumnCount(9);
//    ui->tableWidget->resize(1000, 500); //设置表格长宽
//    QStringList headTabe;
//    headTabe=
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("ACC.X")<<tr("ACC.Y")<<tr("ACC.Z")<<tr("GYRO.X")<<tr("GYRO.Y")<<tr("GYRO.Z")<<tr("Baro_High")<<tr("Baro_Temp")<<tr("Baro_Press"));
    ui->tableWidget->setFont(QFont("wenquanyi",12,3));//设置字体
    ui->tableWidget->horizontalHeader()->setStretchLastSection(ok); //自适应宽度
//    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Strtch);
//    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:red;}");    //设置表头颜色
//    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:darkGray;}");

    ui->tableWidget->setAlternatingRowColors(1); //隔行显示颜色


    i++;
//    count=ui->tableWidget->rowCount();
//    ui->tableWidget->insertRow(count);
    ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(Acc_X_buf));
    ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(Acc_Y_buf));
    ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(Acc_Z_buf));
    ui->tableWidget->setItem(i-1,3,new QTableWidgetItem(Gyro_X_Buf));
    ui->tableWidget->setItem(i-1,4,new QTableWidgetItem(Gyro_Y_Buf));
    ui->tableWidget->setItem(i-1,5,new QTableWidgetItem(Gyro_Z_Buf));
    ui->tableWidget->setItem(i-1,6,new QTableWidgetItem(Baro_Alt_buf));
    ui->tableWidget->setItem(i-1,7,new QTableWidgetItem(Baro_Temp_buf));
    ui->tableWidget->setItem(i-1,8,new QTableWidgetItem(Baro_Press_buf));


#if 0
    i++;
    SensorTemp=Acc_X_buf;
    ui->tableWidget->setItem(i,j+0,new QTableWidgetItem(SensorTemp));
    SensorTemp=Acc_Y_buf;
    ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(SensorTemp));
    SensorTemp=Acc_Z_buf;
    ui->tableWidget->setItem(i,j+2,new QTableWidgetItem(SensorTemp));
    SensorTemp=Gyro_X_Buf;
    ui->tableWidget->setItem(i,j+3,new QTableWidgetItem(SensorTemp));
    SensorTemp=Gyro_Y_Buf;
    ui->tableWidget->setItem(i,j+4,new QTableWidgetItem(SensorTemp));
    SensorTemp=Gyro_Z_Buf;
    ui->tableWidget->setItem(i,j+5,new QTableWidgetItem(SensorTemp));
    ui->tableWidget->insertRow(1);
#endif

    if(i>15)
        i=0;


}


/**************************按钮事件函数**************************************/

//显示或者隐藏传感器数据
void ShowSensorData::on_pushButton_clicked()
{
    static u8 nums=0;
    if(!nums)
    {     

        ui->AllData->hide();
        nums=!nums;
    }
    else
    {


        ui->AllData->show();
        nums=!nums;
    }
}

//保存传感器的数据
void ShowSensorData::on_SaveSensorDataBtn_clicked()
{
    if(ui->AllData->toPlainText().isEmpty()){
        QMessageBox::information(this, "TIPS", tr("No data!Please input data in the LineEdit!!!"), QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save as"), tr("SensorData.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Save Files"), tr("Open Files %1 failed, Cant't Save\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    //写数据到文件
    QTextStream out(&file);
    out<<ui->AllData->toPlainText();
    file.close();
    //修改窗口标题为保存文件路径
    setWindowTitle("saved: " + QFileInfo(filename).canonicalFilePath());
}

