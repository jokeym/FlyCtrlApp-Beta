#include "Files/serialset.h"
#include "ui_serialset.h"
#include <QVBoxLayout>
#include "Files/ConfigData.h"

extern U30_IDENT U30_ident;

SerialSet::SerialSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialSet)
{
    ui->setupUi(this);
//    this->show();
    this->setWindowFlags(Qt::SplashScreen);


    startInit();
    myCom = NULL;

    ui->portNameComboBox->addItem("COM5");
    ui->portNameComboBox->addItem("COM1");
    ui->portNameComboBox->addItem("COM2");
    ui->portNameComboBox->addItem("COM3");
    ui->portNameComboBox->addItem("COM4");
    ui->portNameComboBox->addItem("COM0");
    ui->portNameComboBox->addItem("COM6");
    ui->portNameComboBox->addItem("COM7");



}

SerialSet::~SerialSet()
{
    if(myCom != NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    delete obotimer;
    delete ui;

}


void SerialSet::startInit(){

    ui->delayspinBox->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);
//    ui->actionAdd->setEnabled(true);

//    ui->Cmd_PID_Btn->setEnabled(true);
//    ui->Cmd_Ver_Btn->setEnabled(true);

    //初始化连续发送计时器计时间隔
//    obotimerdly = OBO_TIMER_INTERVAL;
    obotimerdly=ui->delayspinBox->value();

    //设置连续发送计时器
    obotimer = new QTimer(this);
    connect(obotimer, SIGNAL(timeout()), this, SLOT(sendMsg()));

}


void SerialSet::setComboBoxEnabled(bool status)
{
    ui->portNameComboBox->setEnabled(status);
    ui->baudRateComboBox->setEnabled(status);
    ui->dataBitsComboBox->setEnabled(status);
    ui->parityComboBox->setEnabled(status);
    ui->stopBitsComboBox->setEnabled(status);
}



//打开串口
void SerialSet::on_OpenBtn_clicked()
{
    QString portName = ui->portNameComboBox->currentText();   //获取串口名

    myCom = new QextSerialPort(portName);

    connect(myCom, SIGNAL(readyRead()), this, SLOT(readMyCom()));

    //设置波特率
    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());

    //设置数据位
    myCom->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());

    //设置校验
    switch(ui->parityComboBox->currentIndex()){
    case 0:
         myCom->setParity(PAR_NONE);
         break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
        qDebug("set to default : PAR_NONE");
        break;
    }

    //设置停止位
    switch(ui->stopBitsComboBox->currentIndex()){
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
 #ifdef Q_OS_WIN
        myCom->setStopBits(STOP_1_5);
 #endif
        break;
    case 2:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
        qDebug("set to default : STOP_1");
        break;
    }

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);

    if(myCom->open(QIODevice::ReadWrite)){
//        QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口") + portName, QMessageBox::Ok);
        //界面控制
        ui->sendmsgBtn->setEnabled(true);
        setComboBoxEnabled(false);

        ui->sendMsgLineEdit->setEnabled(true);

//        ui->actionOpen->setEnabled(false);
        ui->sendMsgLineEdit->setFocus();
        ui->obocheckBox->setEnabled(true);
//        ui->actionAdd->setEnabled(false);


    }else{
        QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
        return;
    }

    ui->OpenBtn->setEnabled(false);
//    ui->statusBar->showMessage(tr("打开串口成功"));
     CmdTable();
     sendMsg();
}


//关闭串口
void SerialSet::on_CloseBtn_clicked()
{

    if(myCom != NULL){
        if(myCom->isOpen()){
            myCom->close();
        }
        delete myCom;
    }
    ui->sendmsgBtn->setEnabled(false);

    setComboBoxEnabled(true);

    ui->sendMsgLineEdit->setEnabled(false);
    ui->obocheckBox->setEnabled(false);

    ui->OpenBtn->setEnabled(true);
}

//读取数据
extern  unsigned char *Rev_U30_Data;
void SerialSet::readMyCom()
{
    QByteArray temp = myCom->readAll();
    QString buf;

    Rev_U30_Data=(unsigned char *)temp.data();
    U30_ident.Rve_Ptr=Rev_U30_Data;
//    u30.AnalyseU30Data(Rev_U30_Data);
    U30_ident.Rev_Flag=1;

    if(!temp.isEmpty()){
            ui->textBrowser->setTextColor(Qt::black);
            if(ui->ccradioButton->isChecked()){
                buf = temp;
            }else if(ui->chradioButton->isChecked()){
                QString str;
                for(int i = 0; i < temp.count(); i++){
                    QString s;
                    s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
                    buf += s;
                }
            }

        if(!write2fileName.isEmpty()){
            QFile file(write2fileName);
            //如果打开失败则给出提示并退出函数
            if(!file.open(QFile::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this, tr("写入文件"), tr("打开文件 %1 失败, 无法写入\n%2").arg(write2fileName).arg(file.errorString()), QMessageBox::Ok);
                return;
            }
            QTextStream out(&file);
            out<<buf;
            file.close();
        }

        ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);
        QTextCursor cursor = ui->textBrowser->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(cursor);

        ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + temp.size());

        if (ui->recvbyteslcdNumber->value()>3000)
        {

            ui->textBrowser->clear();
            ui->recvbyteslcdNumber->display(0);

        }
    }
}

//发送数据按钮
void SerialSet::on_sendmsgBtn_clicked()
{

    //如果当前正在连续发送数据，暂停发送
    if(ui->sendmsgBtn->text() == tr("暂停")){
        obotimer->stop();
        ui->sendmsgBtn->setText(tr("发送"));
        ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
        return;
    }
    //如果发送数据为空，给出提示并返回
    if(ui->sendMsgLineEdit->text().isEmpty()){
        QMessageBox::information(this, tr("提示消息"), tr("没有需要发送的数据"), QMessageBox::Ok);
        return;
    }
//    obotimer->start(obotimerdly);
//    ui->sendmsgBtn->setText(tr("暂停"));
#if 1
    //如果不是连续发送
    if(!ui->obocheckBox->isChecked()){
        ui->sendMsgLineEdit->setFocus();
        //发送数据
        sendMsg();
    }else{ //连续发送
        obotimer->start(obotimerdly);
        ui->sendmsgBtn->setText(tr("暂停"));
        ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/pause.png"));
    }
#endif
}

void SerialSet::SendData()
{


}

//清空记录
void SerialSet::on_clearUpBtn_clicked()
{
    ui->textBrowser->clear();
}



//计数器清零
void SerialSet::on_CountClearBtn_clicked()
{
    if(ui->recvbyteslcdNumber->value() == 0){
        QMessageBox::information(this, tr("提示消息"), tr("貌似已经清零了呀:)"), QMessageBox::Ok);
    }else{
        ui->recvbyteslcdNumber->display(0);
    }
}

//单击连续发送checkBox
void SerialSet::on_obocheckBox_clicked()
{
    if(ui->obocheckBox->isChecked()){
        ui->delayspinBox->setEnabled(true);
        //检查是否有数据，如有则启动定时器
//        ui->statusBar->showMessage(tr("启用连续发送"));
    }else{
        ui->delayspinBox->setEnabled(false);
        //若定时器已经启动则关闭它
//        ui->statusBar->showMessage(tr("停止连续发送"));
    }
}

//保存textBrowser中的内容
void SerialSet::on_SaveDataBtn_clicked()
{

    if(ui->textBrowser->toPlainText().isEmpty()){
        QMessageBox::information(this, "提示消息", tr("貌似还没有数据! 您需要在发送编辑框中输入要发送的数据"), QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("保存为"), tr("未命名.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("保存文件"), tr("打开文件 %1 失败, 无法保存\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    //写数据到文件
    QTextStream out(&file);
    out<<ui->textBrowser->toPlainText();
    file.close();
    //修改窗口标题为保存文件路径
    setWindowTitle("saved: " + QFileInfo(filename).canonicalFilePath());

}


//添加串口
void SerialSet::on_AddBtn_clicked()
{
    bool ok = false;
    QString portname;

    portname = QInputDialog::getText(this, tr("添加串口"), tr("设备文件名"), QLineEdit::Normal, 0, &ok);
    if(ok && !portname.isEmpty()){
        ui->portNameComboBox->addItem(portname);
//        ui->statusBar->showMessage(tr("添加串口成功"));
    }
}

//调整连续发送时间间隔
void SerialSet::on_delayspinBox_valueChanged(int )
{

    obotimerdly = ui->delayspinBox->value();
}

//载入外部文件
void SerialSet::on_ReadFilesBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("读取文件"), 0, tr("保存为 (*.txt *.log)"));  //添加更多的文件类型
    QFile file(filename);

    //如果取消打开则退出函数
    if(file.fileName().isEmpty()){
        return;
    }

    //如果打开失败则提示并退出函数
    if(!file.open(QFile::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("打开失败"),
                             tr("抱歉! Wincom未能打开此文件, 这可能是由于没有足够的权限造成的. 您可以尝试使用chmod命令修改文件权限."),
                             QMessageBox::Ok);
        return;
    }

    //文件大小超过限制
    if(file.size() > MAX_FILE_SIZE){
        QMessageBox::critical(this, tr("载入失败"), tr("文件大小为 %1 字节, 超过限制大小 10000 字节").arg(file.size()), QMessageBox::Ok);
        return;
    }
    //文件太大时提示是否继续打开
    if(file.size() > TIP_FILE_SIZE){
        if(QMessageBox::question(this, tr("提示消息"), tr("您要打开的文件过大, 这将消耗更多一些的时间,要继续打开么?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){

            return;
        }
    }

    //读取数据并加入到发送数据编辑框
    QByteArray line;
    while(!file.atEnd()){
        line = file.readLine();
        ui->sendMsgLineEdit->setText(ui->sendMsgLineEdit->text() + tr(line));

    }
    file.close();
//    ui->statusBar->showMessage(tr("已经成功读取文件中的数据"));
}

//清空串口中的I/O数据

void SerialSet::on_ClearDataBtn_clicked()
{
    myCom->flush();
    ui->textBrowser->clear();
    ui->recvbyteslcdNumber->display(0);
}


//发送数据
void SerialSet::sendMsg()
{
    obotimer->start(obotimerdly);
    QByteArray buf;
    if(ui->sendAsHexcheckBox->isChecked()){
        QString str;
        bool ok;
        char data;
        QStringList list;

        str = ui->sendMsgLineEdit->text();

        list = str.split(" ");
        for(int i = 0; i < list.count(); i++){
            if(list.at(i) == " ")
                continue;
            if(list.at(i).isEmpty())
                continue;
            data = (char)list.at(i).toInt(&ok, 16);
            if(!ok){
                QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
                if(obotimer != NULL)
                    obotimer->stop();
                ui->sendmsgBtn->setText(tr("发送"));
                ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
                return;
            }
            buf.append(data);
        }
    }else{
#if QT_VERSION < 0x050000
        buf = ui->sendMsgLineEdit->text().toAscii();
#else
        buf = ui->sendMsgLineEdit->text().toLocal8Bit();
#endif
    }
    //发送数据
    myCom->write(buf);
//    ui->statusBar->showMessage(tr("发送数据成功"));
    //界面控制
    ui->textBrowser->setTextColor(Qt::lightGray);
}

//指令表
void SerialSet::CmdTable()
{
    int CmdTab_Ver_Status[12]     ={0x24,0x4d,0x3c,0x00,0x64,0x64,0x24,0x4d,0x3c,0x00,0x65,0x65};
    int CmdTab_IMU_SERVO[12]      ={0x24,0x4d,0x3c,0x00,0x66,0x66,0x24,0x4d,0x3c,0x00,0x67,0x67};
    int CmdTab_Motor_RC[12]       ={0x24,0x4d,0x3c,0x00,0x68,0x68,0x24,0x4d,0x3c,0x00,0x69,0x69};
    int CmdTab_GPS[12]            ={0x24,0x4d,0x3c,0x00,106,106,0x24,0x4d,0x3c,0x00,107,107};
    int CmdTab_ATTITUDE_ALT[12]   ={0x24,0x4d,0x3c,0x00,108,108,0x24,0x4d,0x3c,0x00,109,109};
    int CmdTab_ANALOG_RC[12]      ={0x24,0x4d,0x3c,0x00,110,110,0x24,0x4d,0x3c,0x00,111,111};
    int CmdTab_PID_BOX[12]        ={0x24,0x4d,0x3c,0x00,112,112,0x24,0x4d,0x3c,0x00,113,113};
    int CmdTab_MISC_MotorPins[12] ={0x24,0x4d,0x3c,0x00,114,114,0x24,0x4d,0x3c,0x00,115,115};
    int CmdTab_DEBUG[12]          ={0x24,0x4d,0x3c,0x00,253,253,0x24,0x4d,0x3c,0x00,254,254};
    int CmdTab_BARO[12]           ={0x24,0x4d,0x3c,0x00,130,130,0x24,0x4d,0x3c,0x00,131,131};
    int CmdTab_Filter[12]         ={0x24,0x4d,0x3c,0x00,230,230,0x24,0x4d,0x3c,0x00,231,231};

    int i;
    QString str,s;
    QString Ver_Status_buf,PID_Box_buf,Motor_RC_buf;
    QString GPS_buf,Attitude_Alt_buf,Analog_RC_buf;
    QString Misc_MotorPins_buf,Debug_buf,IMU_Servo_buf;
    QString Baro_buf,Filter_buf;

    for(i=0;i<12;i++)
    {
        s=QString::number(CmdTab_Ver_Status[i],16);
        Ver_Status_buf+=s;
        Ver_Status_buf+=' ';

        s=QString::number(CmdTab_IMU_SERVO[i],16);
        IMU_Servo_buf+=s;
        IMU_Servo_buf+=' ';

        s=QString::number(CmdTab_Motor_RC[i],16);
        Motor_RC_buf +=s;
        Motor_RC_buf +=' ';

        s=QString::number(CmdTab_GPS[i],16);
        GPS_buf +=s;
        GPS_buf +=' ';

        s=QString::number(CmdTab_ATTITUDE_ALT[i],16);
        Attitude_Alt_buf +=s;
        Attitude_Alt_buf +=' ';

        s=QString::number(CmdTab_ANALOG_RC[i],16);
        Analog_RC_buf +=s;
        Analog_RC_buf +=' ';

        s=QString::number(CmdTab_PID_BOX[i],16);
        PID_Box_buf +=s;
        PID_Box_buf +=' ';

        s=QString::number(CmdTab_MISC_MotorPins[i],16);
        Misc_MotorPins_buf +=s;
        Misc_MotorPins_buf +=' ';

        s=QString::number(CmdTab_DEBUG[i],16);
        Debug_buf +=s;
        Debug_buf +=' ';

        s=QString::number(CmdTab_BARO[i],16);
        Baro_buf +=s;
        Baro_buf +=' ';


        s=QString::number(CmdTab_Filter[i],16);
        Filter_buf +=s;
        Filter_buf +=' ';
    }

    ui->CmdTab_Version->setText(Ver_Status_buf);
    ui->CmdTab_IMU->setText(IMU_Servo_buf);
    ui->CmdTab_PID->setText(PID_Box_buf);


    Cmd_Str=Ver_Status_buf+PID_Box_buf+Motor_RC_buf+GPS_buf+Attitude_Alt_buf+Analog_RC_buf;
    Cmd_Str=Cmd_Str+Misc_MotorPins_buf+Debug_buf+IMU_Servo_buf+Baro_buf+Filter_buf;

    ui->sendMsgLineEdit->setText(Cmd_Str);


}












