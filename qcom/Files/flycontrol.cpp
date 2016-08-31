#include "flycontrol.h"
#include "ui_flycontrol.h"
#include <QLabel>
#include <QConicalGradient>
#include <QLinearGradient>
#include <QPixmap>
#include <QPainter>
#include <qapplication.h>
#include <qtimer.h>
#include <qmovie.h>
#include "Files/ConfigData.h"
#include <QWidget>

#define PI 3.1415926
extern Sensor Gyro;
extern Sensor Acc;
extern Sensor Mag;
extern status Status;

FlyControl::FlyControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlyControl)
{
    ui->setupUi(this);

}

FlyControl::~FlyControl()
{
    delete ui;
}

void FlyControl::LoadPicture(void)
{
//    ui->AirSpeed->setPixmap(QPixmap("./Flycontrol_Pic/AirSpeedIndicator_Background.png"));
//    ui->AirSpeedNeedl->setPixmap(QPixmap("./Flycontrol_Pic/AirSpeedNeedle.png"));

    QPixmap Add_Pic;
    Add_Pic.load("images/add.png");
    ui->Test_ShowPic->setPixmap(Add_Pic);

//    QPainter painter(this);
//    painter.drawPixmap(0,0,Add_Pic);

}

void FlyControl::updatePic(void)
{
//    QLinearGradient::QLinearGradient ( 10, 10, 100, 100 );
}


void FlyControl::RotatePic(void)
{
    QPixmap Horizon_Background_Path;
    Horizon_Background_Path.load("./Flycontrol_Pic/Horizon_Background.png");

    QMatrix leftmatrix;

    leftmatrix.rotate(Status.Pitch);
//    leftmatrix.translate(10,20);

//    ui->AirSpeedIndicator_Background->setPixmap(QPixmap("./Flycontrol_Pic/AirSpeedNeedl.png").transformed(leftmatrix,Qt::SmoothTransformation));
//    ui->AirSpeedNeedl->setPixmap(QPixmap("./Flycontrol_Pic/AirSpeedNeedle.png").transformed(leftmatrix,Qt::SmoothTransformation));
//    ui->Horizon_Background->setScaledContents(true);
    ui->Horizon_Background->setPixmap(Horizon_Background_Path.transformed(leftmatrix,Qt::SmoothTransformation));



//
//    leftmatrix.rotate(Status.Roll);
//    ui->TurnCoordinatorAircraft->setPixmap(QPixmap("./Flycontrol_Pic/TurnCoordinatorAircraft.png").transformed(leftmatrix,Qt::SmoothTransformation));




}





